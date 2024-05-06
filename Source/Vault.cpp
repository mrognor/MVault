#include "Vault.h"

#include "Vault.hpp"
#include "VaultRequest.hpp"
#include "VaultRecordRef.hpp"

namespace mvlt
{
    std::unordered_set<VaultRecord*>::iterator Vault::RemoveRecord(VaultRecord* recordToErase, bool* wasDeleted) noexcept
    {
        RecursiveReadWriteMtx.WriteLock();

        std::unordered_set<VaultRecord*>::iterator dataIt = RecordsSet.find(recordToErase);
        if (dataIt == RecordsSet.end())
        {
            RecursiveReadWriteMtx.WriteUnlock();
            if (wasDeleted != nullptr) *wasDeleted = false;
            return dataIt;
        }

        // Iterate over all VaultRecordErasers and call function to erase record from structure
        for (auto& eraser : VaultRecordErasers)
            eraser.second(recordToErase);

        dataIt = RecordsSet.erase(dataIt);

        if (VaultDerivedClass == VaultDerivedClasses::VaultBase)
        {
            recordToErase->Invalidate();

            recordToErase->Mtx.lock();
            for (VaultRecordSet* set : recordToErase->dependentVaultRecordSets)
            {
                set->RecursiveReadWriteMtx.WriteLock();
                // "If" before this line will not called because type saved in VaultDerivedClass variable
                static_cast<Vault*>(set)->RemoveRecord(recordToErase, nullptr);
                set->RecursiveReadWriteMtx.WriteUnlock();
            }
            recordToErase->Mtx.unlock();

        }

        RecursiveReadWriteMtx.WriteUnlock();
        if (wasDeleted != nullptr) *wasDeleted = true;
        return dataIt;
    }

    Vault::Vault() noexcept 
    {
        VaultDerivedClass = VaultDerivedClasses::VaultBase;
    }

    bool Vault::IsKeyExist(const std::string& key) const noexcept
    {
        bool res;
        RecursiveReadWriteMtx.ReadLock();
        res = RecordTemplate.IsData(key);
        RecursiveReadWriteMtx.ReadUnlock();
        return res;
    }

    bool Vault::GetKeyType(const std::string& key, std::type_index& keyType) const noexcept
    {
        bool res = true;
        RecursiveReadWriteMtx.ReadLock();
        auto f = KeysTypes.find(key);
        if (f != KeysTypes.end())
            keyType = f->second;
        else
            res = false;

        RecursiveReadWriteMtx.ReadUnlock();
        return res;
    }

    bool Vault::RemoveKey(const std::string& key) noexcept
    {
        RecursiveReadWriteMtx.WriteLock();

        // Find key in hash map
        auto foundedKeyInHashMapIt = KeysTypes.find(key);

        // If key was not find return false
        if (foundedKeyInHashMapIt == KeysTypes.end())
        {
            RecursiveReadWriteMtx.WriteUnlock();
            return false;
        }

        // Remove key from hash map with keys types
        KeysTypes.erase(foundedKeyInHashMapIt);

        // Remove key from list with key order
        for (auto it = KeysOrder.begin(); it != KeysOrder.end(); ++it)
        {
            if (key == *it)
            {
                KeysOrder.erase(it);
                break;
            }
        }

        // Erase key from record template
        RecordTemplate.EraseData(key);

        // Erase key from VaultHashMapStructure
        VaultHashMapStructure.EraseData(key);

        // Erase key from VaultMapStructure
        VaultMapStructure.EraseData(key);

        // Erase key from all maps
        VaultRecordAdders.erase(key);
        VaultRecordClearers.erase(key);
        VaultRecordErasers.erase(key);
        VaultRecordSorters.erase(key);
        VaultKeyCopiers.erase(key);

        if (VaultDerivedClass == VaultDerivedClasses::VaultBase)
        {
            // Erase key data from all records
            for (auto& it : RecordsSet)
                it->EraseData(key);

            for (VaultRecordSet* set : RecordSetsSet)
            {
                set->RecursiveReadWriteMtx.WriteLock();
                set->RemoveKey(key);
                set->RecursiveReadWriteMtx.WriteUnlock();
            }
        }

        RecursiveReadWriteMtx.WriteUnlock();

        return true;
    }

    VaultRecordRef Vault::CreateRecord() noexcept
    {
        RecursiveReadWriteMtx.WriteLock();

        // Create new record
        VaultRecord* newData = new VaultRecord(RecordTemplate);
        // Add new record to set
        RecordsSet.emplace(newData);

        // Add new record to every maps inside VaultStructureHashMap
        for (auto& it : VaultRecordAdders)
            it.second(newData);

        VaultRecordRef res(newData, this);

        RecursiveReadWriteMtx.WriteUnlock();

        return res;
    }

    VaultOperationResult Vault::CreateRecord(const std::vector<std::pair<std::string, VaultParamInput>>& params) noexcept
    {
        VaultRecordRef tmp;
        return CreateRecord(tmp, params);
    }

    VaultOperationResult Vault::CreateRecord(VaultRecordRef& vaultRecordRef, const std::vector<std::pair<std::string, VaultParamInput>>& params) noexcept
    {
        VaultOperationResult res;

        RecursiveReadWriteMtx.WriteLock();

        // Create new record
        VaultRecord* newData = new VaultRecord(RecordTemplate);

        for (const auto& it : params)
        {
            DataSaver ds;

            res.Key = it.first;
            res.RequestedType = it.second.GetDataType();

            // Check if key exist in record template
            if (newData->GetDataSaver(it.first, ds))
            {
                res.RequestedType = KeysTypes.find(it.first)->second;

                // Check if type in params match type in record template
                if (ds.GetDataType() == it.second.GetDataType())
                {
                    it.second.SetDataToRecord(it.first, newData);

                    // If key in record template and key tipy match type in param
                    res.IsOperationSuccess = true;
                    res.SavedType = it.second.GetDataType();
                    res.ResultCode = VaultOperationResultCode::Success;
                }
                else
                {   // If the type in param not match type in record template
                    res.IsOperationSuccess = false;
                    res.SavedType = it.second.GetDataType();
                    res.ResultCode = VaultOperationResultCode::WrongType;
                    break;
                }
            }
            else
            {   // If key not exist then stop set data from params
                res.IsOperationSuccess = false;
                res.SavedType = typeid(void);
                res.RequestedType = typeid(void);
                res.ResultCode = VaultOperationResultCode::WrongKey;
                break;
            }
        }

        // Add new record to set
        RecordsSet.emplace(newData);

        // Add new record to every maps inside VaultStructureHashMap
        for (auto& it : VaultRecordAdders)
            it.second(newData);

        vaultRecordRef.SetRecord(newData, this);

        RecursiveReadWriteMtx.WriteUnlock();

        return res;
    }

    void Vault::DropVault() noexcept
    {
        RecursiveReadWriteMtx.WriteLock();

        for (auto& it : RecordSetsSet)
        {
            it->RecursiveReadWriteMtx.WriteLock();
            it->IsParentVaultValid = false;
            it->Clear();
            it->RecursiveReadWriteMtx.WriteUnlock();
        }

        RecordSetsSet.clear();

        // Clear record template
        RecordTemplate.Clear();

        // Delete all unordered maps inside VaultHashMapStructure
        for (auto& it : VaultHashMapStructure)
            it.second.ResetData();

        // Clear VaultHashMapStructure
        VaultHashMapStructure.Clear();

        // Delete all maps inside VaultMapStructure
        for (auto& it : VaultMapStructure)
            it.second.ResetData();

        // Clear hash map with keys types
        KeysTypes.clear();

        // Clear VaultMapStructure
        VaultMapStructure.Clear();

        // Clear all maps with functions
        VaultRecordAdders.clear();
        VaultRecordClearers.clear();
        VaultRecordErasers.clear();
        VaultRecordSorters.clear();
        VaultKeyCopiers.clear();

        // Delete all Records
        for (auto& it : RecordsSet) 
            it->Invalidate();

        // Clear RecordsSet
        RecordsSet.clear();

        KeysOrder.clear();
        
        RecursiveReadWriteMtx.WriteUnlock();
    }

    void Vault::DropData() noexcept
    {
        RecursiveReadWriteMtx.WriteLock();

        // Call functions to clear VaultHashMapStructure without
        for (auto& it : VaultRecordClearers)
            it.second();

        // Delete all Records
        for (auto& it : RecordsSet)
        {
            it->Mtx.lock();
            for (VaultRecordSet* set : it->dependentVaultRecordSets)
            {
                set->RecursiveReadWriteMtx.WriteLock();
                static_cast<Vault*>(set)->RemoveRecord(it, nullptr);
                set->RecursiveReadWriteMtx.WriteUnlock();
            }
            it->Mtx.unlock();
            it->Invalidate();
        }

        // Clear RecordsSet
        RecordsSet.clear();

        RecursiveReadWriteMtx.WriteUnlock();
    }

    bool Vault::EraseRecord(const VaultRecordRef& recordRefToErase) noexcept
    {
        bool res;

        recordRefToErase.Mtx.lock();
        RecursiveReadWriteMtx.WriteLock();

        RemoveRecord(recordRefToErase.DataRecord, &res);

        RecursiveReadWriteMtx.WriteUnlock();
        recordRefToErase.Mtx.unlock();
        return res;
    }

    std::size_t Vault::Size() const noexcept
    {
        std::size_t res;
        RecursiveReadWriteMtx.ReadLock();
        res = RecordsSet.size();
        RecursiveReadWriteMtx.ReadUnlock();
        return res;
    }

    std::vector<std::string> Vault::GetKeys() const noexcept
    {
        std::vector<std::string> res;

        RecursiveReadWriteMtx.ReadLock();
        for (const auto& it : VaultMapStructure) res.emplace_back(it.first);
        RecursiveReadWriteMtx.ReadUnlock();

        return res;
    }

    std::vector<VaultRecordRef> Vault::GetSortedRecords(const std::string& key, const bool& isReverse, const std::size_t& amountOfRecords) const noexcept
    {
        std::vector<VaultRecordRef> res;
        std::size_t counter = 0;

        RecursiveReadWriteMtx.ReadLock();

        auto f = VaultRecordSorters.find(key);
        if (f != VaultRecordSorters.end())
        {
            f->second([&](const VaultRecordRef& vaultRecordRef)
                {
                    if (counter >= amountOfRecords) return false;

                    res.emplace_back(vaultRecordRef);
                    ++counter;
                    return true;
                }, isReverse);
        }

        RecursiveReadWriteMtx.ReadUnlock();

        return res;
    }

    void Vault::PrintVault(const std::size_t& amountOfRecords) const noexcept
    {
        RecursiveReadWriteMtx.ReadLock();

        std::vector<std::string> keys = GetKeys();
        std::size_t counter = 0;

        for (const auto& record : RecordsSet)
        {
            std::cout << "Vault record " << record << ":" << std::endl;

            for (const std::string& key : keys)
            {
                DataSaver dataSaver;
                record->GetDataSaver(key, dataSaver);

                std::cout << "\t" << key << " = " << dataSaver.Str() << std::endl;
            }

            ++counter;
            if (counter == amountOfRecords) break;
        }

        RecursiveReadWriteMtx.ReadUnlock();
    }

    void Vault::PrintAsTable(bool isPrintId, const std::size_t& amountOfRecords, std::string primaryKey, const bool& isReverse,
        const std::list<std::string> keys) const noexcept
    {
        RecursiveReadWriteMtx.ReadLock();

        if (VaultMapStructure.Size() == 0)
        {
            std::cout << "Vault does not contain keys!" << std::endl;
            std::cout << " (" << RecordsSet.size() << " records)" << std::endl;
            RecursiveReadWriteMtx.ReadUnlock();
            return;
        }

        // Set proper list with all keys
        const std::list<std::string>* keysList;
        if (keys.size() != 0)
            keysList = &keys;
        else
            keysList = &KeysOrder;

        // Counter for iteration cycles
        std::size_t counter = 0;

        // Vector for maximal cell length
        std::vector<std::size_t> maxLengths(keysList->size());

        // Fill each max length using key length
        for (const std::string& key : *keysList)
        {
            maxLengths[counter] = key.length();
            ++counter;
        }

        // Find each real max length
        std::string dataString;
        counter = 0;
        for (const std::string& key : *keysList)
        {
            for (const auto& record : RecordsSet)
            {
                record->GetDataAsString(key, dataString);
                if (dataString.length() > maxLengths[counter]) maxLengths[counter] = dataString.length();   
            }
            ++counter;
        }

        // Print splitter
        counter = 0;
        std::cout << "+";
        for (std::size_t i = 0; i < keysList->size(); ++i)
        {
            std::cout << "-";
            for (std::size_t j = 0; j < maxLengths[counter]; ++j) std::cout << "-";
            std::cout << "-+";
            ++counter;
        }
        std::cout << std::endl;

        // Print header
        counter = 0;
        std::cout << "|";
        for (const std::string& key : *keysList)
        {
            std::cout << " " << key;
            for (std::size_t j = key.length(); j < maxLengths[counter]; ++j) std::cout << " ";
            std::cout << " |";
            ++counter;
        }
        std::cout << std::endl;
        
        // Print splitter
        counter = 0;
        std::cout << "+";
        for (std::size_t i = 0; i < keysList->size(); ++i)
        {
            std::cout << "-";
            for (std::size_t j = 0; j < maxLengths[counter]; ++j) std::cout << "-";
            std::cout << "-+";
            ++counter;
        }
        std::cout << std::endl;

        // Print records
        counter = 0;
        if (primaryKey.empty()) primaryKey = keysList->front();
        SortBy(primaryKey, [&](const VaultRecordRef& record)
            {
                counter = 0;
                std::cout << "|";
                for (const std::string& key : *keysList)
                {
                    record.GetDataAsString(key, dataString);
                    std::cout << " " << dataString;
                    for (std::size_t j = dataString.length(); j < maxLengths[counter]; ++j) std::cout << " ";
                        std::cout << " |";
                    ++counter;
                }
                if (isPrintId)
                    std::cout << " " << record.GetRecordUniqueId() << std::endl;
                else 
                    std::cout << std::endl;
                return true;
            }, isReverse, amountOfRecords);

        // Print splitter
        if (amountOfRecords >= RecordsSet.size())
        {
            std::cout << "+";
            for (std::size_t i = 0; i < keysList->size(); ++i)
            {
                std::cout << "-";
                for (std::size_t j = 0; j < maxLengths[i]; ++j) std::cout << "-";
                std::cout << "-+";
            }
            std::cout << std::endl;
        }
        else
        {
            std::cout << "$";
            for (std::size_t i = 0; i < keysList->size(); ++i)
            {
                std::cout << "~";
                for (std::size_t j = 0; j < maxLengths[i]; ++j) std::cout << "~";
                std::cout << "~$";
            }
            std::cout << std::endl;
        }

        std::cout << " (" << RecordsSet.size() << " records)" << std::endl;
   

        RecursiveReadWriteMtx.ReadUnlock();
    }

    bool Vault::SaveToFile(const std::string& fileName, const std::string& separator, const bool& isSaveKey) const noexcept
    {
        RecursiveReadWriteMtx.ReadLock();

        // Open or create file to save data
        std::ofstream csvFile(fileName, std::ios::binary);

        // Checking whether the file was opened successfully
        if (!csvFile.is_open())
        {
            RecursiveReadWriteMtx.ReadUnlock();
            return false;
        }

        // C array to store correct new line characters. 
        const char endOfLine[2] = {13, 10}; // 13 - CR or 0d. 10 - LF or 0a.

        // Save keys to file
        if (isSaveKey)
        {
            auto it = KeysOrder.cbegin();
            csvFile << *it;
            ++it;
            for (;it != KeysOrder.cend(); ++it) csvFile << separator << *it;
            csvFile.write(endOfLine, 2);
        }

        for (const auto& record : RecordsSet)
        {
            // Getting the number of items in a record 
            std::size_t counter = record->Size();
            for (const auto& recordKeyValue : *record)
            {
                // Calling the string formatting function for the csv format
                csvFile << FormatStringToCsv(recordKeyValue.second.Str());

                // Checking not to write a comma to the end of the line
                if (counter == 1) break;

                csvFile << separator;
                --counter;
            }
            csvFile.write(endOfLine, 2);
        }

        csvFile.close();

        RecursiveReadWriteMtx.ReadUnlock();
        return true;
    }

    bool Vault::ReadFile(const std::string& fileName, const char& separator, const bool& isLoadKeys) noexcept
    {
        // Open and parse file
        CsvParser parser;
        parser.OpenFile(fileName);

        // Write lock because new records will be added
        RecursiveReadWriteMtx.WriteLock();

        std::vector<std::string> keys;

        // Vector with record keys
        std::vector<std::string> record;

        if (isLoadKeys) parser.GetNextVector(keys, separator);
        
        // Read new record from parser
        while(parser.GetNextVector(record, separator))
        {
            // Record to set to it data from file and store in Vault
            VaultRecord* newRecord = new VaultRecord(RecordTemplate);
        
            if (isLoadKeys)
            {
                for (std::size_t i = 0; i < keys.size(); ++i)
                    if (i < record.size())
                        newRecord->SetDataFromString(keys[i], record[i]);
            }
            else
            {
                // Iteraror to load keys
                auto keyOrderIt = KeysOrder.begin();

                // Store all keys values to record
                for (const auto& recordPole : record)
                {
                    // Set last record pole
                    newRecord->SetDataFromString(*keyOrderIt, recordPole);

                    ++keyOrderIt;
                }
            }

            // Add new record to set
            RecordsSet.emplace(newRecord);

            // Add new record to every maps inside VaultStructureHashMap
            for (auto& it : VaultRecordAdders)
                it.second(newRecord);

            record.clear();
        }

        RecursiveReadWriteMtx.WriteUnlock();

        return true;
    }

    Vault::~Vault() noexcept
    {
        for (auto& it : RecordSetsSet)
        {
            it->RecursiveReadWriteMtx.WriteLock();
            it->IsParentVaultValid = false;
            it->Clear();
            it->RecursiveReadWriteMtx.WriteUnlock();
        }

        // Clear all records
        for (auto& it : RecordsSet)
            it->Invalidate();

        // Clear VaultHashMapStructure
        for (auto& it : VaultHashMapStructure)
            it.second.ResetData();

        // Clear VaultMapStructure
        for (auto& it : VaultMapStructure)
            it.second.ResetData();
    }
}