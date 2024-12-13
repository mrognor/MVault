#include "Vault.h"

#include "Vault.hpp"
#include "VaultRequest.hpp"
#include "VaultRecordRef.hpp"

namespace mvlt
{
    std::unordered_set<VaultRecord*>::iterator Vault::RemoveRecord(VaultRecord* recordToErase, bool* wasDeleted) noexcept
    {
        // Lock Vault to write
        WriteLock<RecursiveReadWriteMutex> writeLock(RecursiveReadWriteMtx);

        std::unordered_set<VaultRecord*>::iterator dataIt = RecordsSet.find(recordToErase);
        if (dataIt == RecordsSet.end())
        {
            if (wasDeleted != nullptr) *wasDeleted = false;
            return dataIt;
        }

        // Iterate over all VaultRecordErasers and call function to erase record from structure
        for (auto& eraser : VaultRecordErasers)
            eraser.second(recordToErase);

        dataIt = RecordsSet.erase(dataIt);

        if (VaultDerivedClass == VaultDerivedClasses::VaultBase)
        {
            // In VaultRecordSet "if" before this line will not called because type saved in VaultDerivedClass variable
            recordToErase->RemoveFromDependentSets();
            recordToErase->Invalidate();
        }

        if (wasDeleted != nullptr) *wasDeleted = true;
        return dataIt;
    }

    bool Vault::ReadFile(const std::string& fileName, const bool& isPreprocessRecord, std::function<void (const std::vector<std::string>&, std::vector<std::string>&)> recordHandler, 
            const char& separator, const bool& isLoadKeys, const std::vector<std::string> userKeys) noexcept
    {
        // Open and parse file
        CsvParser parser;
        if (!parser.OpenFile(fileName)) return false;

        // Write lock because new records will be added
        WriteLock<RecursiveReadWriteMutex> writeLock(RecursiveReadWriteMtx);

        std::vector<std::string> keys;

        InvalidFileRecords.clear();

        std::size_t lineCounter = 0;

        // Vector with record keys
        std::vector<std::string> record;

        if (isLoadKeys) parser.GetNextVector(keys, separator);

        // Read new record from parser
        while(parser.GetNextVector(record, separator))
        {
            ++lineCounter;

            // Record to set to it data from file and store in Vault
            VaultRecord* newRecord = new VaultRecord(RecordTemplate);

            bool isCorrectRecordInFile = false;

            if (isLoadKeys)
            {
                if (isPreprocessRecord)
                    recordHandler(keys, record);

                for (std::size_t i = 0; i < keys.size(); ++i)
                {
                    if (i < record.size())
                    {
                        isCorrectRecordInFile = newRecord->SetDataFromString(keys[i], record[i]);

                        if (!isCorrectRecordInFile) 
                        {
                            InvalidFileRecords.emplace_back(std::pair<std::size_t, std::string>(lineCounter - 1, keys[i]));
                            break;
                        }
                    }
                }
            }
            else
            {
                if (userKeys.empty())
                {
                    // Iteraror to load keys
                    auto keyOrderIt = KeysOrder.begin();

                    // Store all keys values to record
                    for (const auto& recordPole : record)
                    {
                        // Set last record pole
                        isCorrectRecordInFile = newRecord->SetDataFromString(*keyOrderIt, recordPole);

                        if (!isCorrectRecordInFile) 
                        {
                            InvalidFileRecords.emplace_back(std::pair<std::size_t, std::string>(lineCounter - 1, *keyOrderIt));
                            break;
                        }

                        ++keyOrderIt;
                    }
                }
                else 
                {
                    // Iteraror to load keys
                    auto keyOrderIt = userKeys.begin();

                    // Store all keys values to record
                    for (const auto& recordPole : record)
                    {
                        // Set last record pole
                        isCorrectRecordInFile = newRecord->SetDataFromString(*keyOrderIt, recordPole);

                        if (!isCorrectRecordInFile) 
                        {
                            InvalidFileRecords.emplace_back(std::pair<std::size_t, std::string>(lineCounter - 1, *keyOrderIt));
                            break;
                        }

                        ++keyOrderIt;
                        if (keyOrderIt == userKeys.end()) break;
                    }
                }
            }

            if (!isCorrectRecordInFile) 
            {
                delete newRecord;
                record.clear();
                continue;
            }

            std::vector<std::string> addedUniqueKeys;

            // Adding unique keys
            auto uniqueKeyIt = UniqueKeys.begin();
            for (; uniqueKeyIt != UniqueKeys.end(); ++uniqueKeyIt)
            {
                // Find key adder and try to emplace data and if add failed, then break cycle
                if (!VaultRecordAdders.find(*uniqueKeyIt)->second(newRecord))
                    break;

                addedUniqueKeys.emplace_back(*uniqueKeyIt);
            }

            // If at least one unique key adding fail
            if (addedUniqueKeys.size() != UniqueKeys.size())
            {
                // Removed added unique keys
                for (const std::string& uniqueKey : addedUniqueKeys)
                    VaultRecordErasers.find(uniqueKey)->second(newRecord);
                
                InvalidFileRecords.emplace_back(lineCounter, *uniqueKeyIt);
                delete newRecord;
            }
            else
            {
                // Add new record to set
                RecordsSet.emplace(newRecord);

                // Add new record to every maps inside VaultStructureHashMap
                for (const auto& vaultRecordAddersIt : VaultRecordAdders)
                    vaultRecordAddersIt.second(newRecord);
            }

            record.clear();
        }

        return true;
    }

    Vault::Vault() noexcept 
    {
        VaultDerivedClass = VaultDerivedClasses::VaultBase;
    }

    Vault::Vault(const Vault& other) noexcept
    {
        ReadLock<RecursiveReadWriteMutex> readLock(other.RecursiveReadWriteMtx);

        VaultDerivedClass = VaultDerivedClasses::VaultBase;

        // Copy keys
        for (auto& keyCopierIt : other.VaultKeyCopiers)
            keyCopierIt.second(this);

        // Set proper key order
        KeysOrder = other.KeysOrder;

        // Set unique keys
        UniqueKeys = other.UniqueKeys;

        for (VaultRecord* recordPtr : other.RecordsSet)
        {
            // Allocate space to new record and copy data to it
            VaultRecord* newRecord = new VaultRecord(*recordPtr);
            
            // Add new record to set
            RecordsSet.emplace(newRecord);

            // Add new record to every maps inside VaultStructureHashMap
            for (const auto& vaultRecordAddersIt : VaultRecordAdders)
                vaultRecordAddersIt.second(newRecord);
        }
    }

    Vault& Vault::operator=(const Vault& other) noexcept
    {
        if (&other != this)
        {
            DropVault();

            ReadLock<RecursiveReadWriteMutex> readLock(RecursiveReadWriteMtx);
            ReadLock<RecursiveReadWriteMutex> otherReadLock(other.RecursiveReadWriteMtx);

            VaultDerivedClass = VaultDerivedClasses::VaultBase;

            // Copy keys
            for (auto& keyCopierIt : other.VaultKeyCopiers)
                keyCopierIt.second(this);

            // Set proper key order
            KeysOrder = other.KeysOrder;

            // Set unique keys
            UniqueKeys = other.UniqueKeys;

            for (VaultRecord* recordPtr : other.RecordsSet)
            {
                // Allocate space to new record and copy data to it
                VaultRecord* newRecord = new VaultRecord(*recordPtr);
                
                // Add new record to set
                RecordsSet.emplace(newRecord);

                // Add new record to every maps inside VaultStructureHashMap
                for (const auto& vaultRecordAddersIt : VaultRecordAdders)
                    vaultRecordAddersIt.second(newRecord);
            }
        }

        return *this;
    }

    Vault::Vault(Vault&& other) noexcept
    {
        ReadLock<RecursiveReadWriteMutex> otherReadLock(other.RecursiveReadWriteMtx);

        VaultDerivedClass = VaultDerivedClasses::VaultBase;

        RecordTemplate = std::move(other.RecordTemplate);
        VaultHashMapStructure = std::move(other.VaultHashMapStructure);
        VaultMapStructure = std::move(other.VaultMapStructure);
        KeysTypes = std::move(other.KeysTypes);
        VaultRecordAdders = std::move(other.VaultRecordAdders);
        VaultRecordClearers = std::move(other.VaultRecordClearers);
        VaultRecordErasers = std::move(other.VaultRecordErasers);
        VaultRecordSorters = std::move(other.VaultRecordSorters);
        VaultKeyCopiers = std::move(other.VaultKeyCopiers);
        KeysOrder = std::move(other.KeysOrder);
        UniqueKeys = std::move(other.UniqueKeys);
        InvalidFileRecords = std::move(other.InvalidFileRecords);
        RecordsSet = std::move(other.RecordsSet);
        RecordSetsSet = std::move(other.RecordSetsSet);
    }

    Vault& Vault::operator=(Vault&& other) noexcept
    {
        if (this != &other)
        {
            DropVault();

            ReadLock<RecursiveReadWriteMutex> otherReadLock(other.RecursiveReadWriteMtx);

            VaultDerivedClass = VaultDerivedClasses::VaultBase;

            RecordTemplate = std::move(other.RecordTemplate);
            VaultHashMapStructure = std::move(other.VaultHashMapStructure);
            VaultMapStructure = std::move(other.VaultMapStructure);
            KeysTypes = std::move(other.KeysTypes);
            VaultRecordAdders = std::move(other.VaultRecordAdders);
            VaultRecordClearers = std::move(other.VaultRecordClearers);
            VaultRecordErasers = std::move(other.VaultRecordErasers);
            VaultRecordSorters = std::move(other.VaultRecordSorters);
            VaultKeyCopiers = std::move(other.VaultKeyCopiers);
            KeysOrder = std::move(other.KeysOrder);
            UniqueKeys = std::move(other.UniqueKeys);
            InvalidFileRecords = std::move(other.InvalidFileRecords);
            RecordsSet = std::move(other.RecordsSet);
            RecordSetsSet = std::move(other.RecordSetsSet);
        }

        return *this;
    }

    bool Vault::IsKeyExist(const std::string& key) const noexcept
    {
        bool res;
        ReadLock<RecursiveReadWriteMutex> readLock(RecursiveReadWriteMtx);
        res = RecordTemplate.IsData(key);
        return res;
    }

    bool Vault::GetKeyType(const std::string& key, std::type_index& keyType) const noexcept
    {
        bool res = true;
        ReadLock<RecursiveReadWriteMutex> readLock(RecursiveReadWriteMtx);

        auto findResIt = KeysTypes.find(key);
        if (findResIt != KeysTypes.end())
            keyType = findResIt->second;
        else
            res = false;

        return res;
    }

    std::vector<std::string> Vault::GetKeys() const noexcept
    {
        std::vector<std::string> res;

        ReadLock<RecursiveReadWriteMutex> readLock(RecursiveReadWriteMtx);
        for (const auto& vaultMapStructureIt : VaultMapStructure) res.emplace_back(vaultMapStructureIt.first);

        return res;
    }

    bool Vault::RemoveKey(const std::string& key) noexcept
    {
        WriteLock<RecursiveReadWriteMutex> writeLock(RecursiveReadWriteMtx);

        // Find key in hash map
        auto foundedKeyInHashMapIt = KeysTypes.find(key);

        // If key was not find return false
        if (foundedKeyInHashMapIt == KeysTypes.end())
            return false;

        // Try to erase unique key
        UniqueKeys.erase(key);

        // Remove key from hash map with keys types
        KeysTypes.erase(foundedKeyInHashMapIt);

        // Remove key from list with key order
        for (auto keysOrderIt = KeysOrder.begin(); keysOrderIt != KeysOrder.end(); ++keysOrderIt)
        {
            if (key == *keysOrderIt)
            {
                KeysOrder.erase(keysOrderIt);
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
            for (const auto& recordsSetIt : RecordsSet)
                recordsSetIt->EraseData(key);

            for (VaultRecordSet* set : RecordSetsSet)
                set->RemoveKey(key);
        }

        return true;
    }

    VaultOperationResult Vault::CreateRecord(const std::vector<std::pair<std::string, VaultParamInput>>& params) noexcept
    {
        VaultRecordRef tmp;
        return CreateRecord(tmp, params);
    }

    VaultOperationResult Vault::CreateRecord(VaultRecordRef& vaultRecordRef, const std::vector<std::pair<std::string, VaultParamInput>>& params) noexcept
    {
        VaultOperationResult res;

        // Lock Vault to write
        WriteLock<RecursiveReadWriteMutex> writeLock(RecursiveReadWriteMtx);

        // Variable to check params vec correctness
        bool isCorrectParams = true;

        // Create new record
        VaultRecord* newRecord = new VaultRecord(RecordTemplate);

        for (const auto& paramsIt : params)
        {
            DataSaver ds;

            res.Key = paramsIt.first;
            res.RequestedType = paramsIt.second.GetDataType();

            // Check if key exist in record template
            if (newRecord->GetDataSaver(paramsIt.first, ds))
            {
                res.RequestedType = KeysTypes.find(paramsIt.first)->second;

                // Check if type in params match type in record template
                if (ds.GetDataType() == paramsIt.second.GetDataType())
                {
                    paramsIt.second.SetDataToRecord(paramsIt.first, newRecord);
                }
                else
                {   // If the type in param not match type in record template
                    res.IsOperationSuccess = false;
                    res.SavedType = paramsIt.second.GetDataType();
                    res.ResultCode = VaultOperationResultCode::WrongType;
                    isCorrectParams = false;
                    break;
                }
            }
            else
            {   // If key not exist then stop set data from params
                res.IsOperationSuccess = false;
                res.SavedType = typeid(void);
                res.RequestedType = typeid(void);
                res.ResultCode = VaultOperationResultCode::WrongKey;
                isCorrectParams = false;
                break;
            }
        }

        // If param vec correct than add new record
        if (isCorrectParams)
        {
            std::vector<std::string> addedUniqueKeys;
            std::string incorrectUniqueKey;

            // Adding unique keys
            for (auto uniqueKeyIt = UniqueKeys.begin(); uniqueKeyIt != UniqueKeys.end(); ++uniqueKeyIt)
            {
                // Find key adder and try to emplace data and if add failed, then break cycle
                if (!VaultRecordAdders.find(*uniqueKeyIt)->second(newRecord))
                {
                    incorrectUniqueKey = *uniqueKeyIt;
                    break;
                }

                addedUniqueKeys.emplace_back(*uniqueKeyIt);
            }

            // If at least one unique key adding fail
            if (addedUniqueKeys.size() != UniqueKeys.size())
            {
                // Removed added unique keys
                for (const std::string& uniqueKey : addedUniqueKeys)
                    VaultRecordErasers.find(uniqueKey)->second(newRecord);

                res.IsOperationSuccess = false;
                res.Key = incorrectUniqueKey;
                res.SavedType = KeysTypes.find(incorrectUniqueKey)->second;
                res.RequestedType = res.SavedType;
                res.ResultCode = VaultOperationResultCode::UniqueKeyValueAlredyInSet;
                isCorrectParams = true;
                delete newRecord;
            }
            else
            {
                // Add new record to set
                RecordsSet.emplace(newRecord);

                // Add new record to every maps inside VaultStructureHashMap
                for (const auto& vaultRecordAddersIt : VaultRecordAdders)
                    vaultRecordAddersIt.second(newRecord);

                vaultRecordRef.SetRecord(newRecord, this);

                res.IsOperationSuccess = true;
                res.ResultCode = VaultOperationResultCode::Success;
            }
        }
        else delete newRecord;

        return res;
    }

    void Vault::DropVault() noexcept
    {
        // Lock Vault to write
        WriteLock<RecursiveReadWriteMutex> writeLock(RecursiveReadWriteMtx);

        // Invalidate VaultRecordSets dependent from records from this
        for (auto recordSetsSetIt = RecordSetsSet.begin(); recordSetsSetIt != RecordSetsSet.end();)
        {
            auto itToDelete = recordSetsSetIt;
            ++recordSetsSetIt;
            (*itToDelete)->Reset();
        }

        // Clear unique keys vector
        UniqueKeys.clear();

        // Clear last readed file errors
        InvalidFileRecords.clear();

        // Invalidate all records
        for (const auto& recordsSetIt : RecordsSet) 
            recordsSetIt->Invalidate();

        // Clear record template
        RecordTemplate.Clear();

        // Delete all unordered maps inside VaultHashMapStructure
        for (auto& vaultHashMapStructureIt : VaultHashMapStructure)
            vaultHashMapStructureIt.second.ResetData();

        // Clear VaultHashMapStructure
        VaultHashMapStructure.Clear();

        // Delete all maps inside VaultMapStructure
        for (auto& vaultMapStructureIt : VaultMapStructure)
            vaultMapStructureIt.second.ResetData();

        // Clear VaultMapStructure
        VaultMapStructure.Clear();

        // Clear hash map with keys types
        KeysTypes.clear();

        // Clear all maps with functions
        VaultRecordAdders.clear();
        VaultRecordClearers.clear();
        VaultRecordErasers.clear();
        VaultRecordSorters.clear();
        VaultKeyCopiers.clear();

        // Clear key order 
        KeysOrder.clear();

        // Clear set with all invalidated records
        RecordsSet.clear();

        // Clear set with all VaultRecordSet`s
        RecordSetsSet.clear();
    }

    void Vault::DropData() noexcept
    {
        // Lock Vault to write
        WriteLock<RecursiveReadWriteMutex> writeLock(RecursiveReadWriteMtx);

        // Invalidate VaultRecordSets dependent from records from this
        for (const auto& recordSetsSetIt : RecordSetsSet)
            recordSetsSetIt->Clear();

        // Invalidate all records
        for (const auto& recordsSetIt : RecordsSet) 
            recordsSetIt->Invalidate();
        
        // Clear structure
        for (const auto& vaultRecordClearersIt : VaultRecordClearers)
            vaultRecordClearersIt.second();

        // Clear RecordsSet
        RecordsSet.clear();
    }

    bool Vault::EraseRecord(const VaultRecordRef& recordRefToErase) noexcept
    {
        bool res;

        WriteLock<RecursiveReadWriteMutex> writeLock(RecursiveReadWriteMtx);

        RemoveRecord(recordRefToErase.VaultRecordPtr, &res);

        return res;
    }

    std::size_t Vault::Size() const noexcept
    {
        std::size_t res;
        ReadLock<RecursiveReadWriteMutex> readLock(RecursiveReadWriteMtx);

        res = RecordsSet.size();

        return res;
    }

    std::vector<VaultRecordRef> Vault::GetSortedRecords(const std::string& key, const bool& isReverse, const std::size_t& amountOfRecords) const noexcept
    {
        std::vector<VaultRecordRef> res;
        std::size_t counter = 0;

        // Lock Vault to read
        ReadLock<RecursiveReadWriteMutex> readLock(RecursiveReadWriteMtx);

        auto findResIt = VaultRecordSorters.find(key);
        if (findResIt != VaultRecordSorters.end())
        {
            findResIt->second([&](const VaultRecordRef& vaultRecordRef)
                {
                    if (counter >= amountOfRecords) return false;

                    res.emplace_back(vaultRecordRef);
                    ++counter;
                    return true;
                }, const_cast<Vault*>(this), isReverse);
        }

        return res;
    }

    void Vault::PrintVault(const std::size_t& amountOfRecords) const noexcept
    {
        // Lock Vault to read
        ReadLock<RecursiveReadWriteMutex> readLock(RecursiveReadWriteMtx);

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
    }

    void Vault::PrintAsTable(bool isPrintId, const std::size_t& amountOfRecords, std::string primaryKey, const bool& isReverse,
        const std::list<std::string> keys) const noexcept
    {
        // Lock Vault to read
        ReadLock<RecursiveReadWriteMutex> readLock(RecursiveReadWriteMtx);

        if (VaultMapStructure.Size() == 0)
        {
            switch (VaultDerivedClass)
            {
            case VaultDerivedClasses::VaultBase:
                std::cout << "Vault does not contain keys!" << std::endl;
                break;

            case VaultDerivedClasses::VaultRecordSetDerived:
                std::cout << "VaultRecordSet does not contain keys!" << std::endl;
                break;
            }

            std::cout << " (" << RecordsSet.size() << " records)" << std::endl;
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
    }

    bool Vault::SaveToFile(const std::string& fileName, const std::vector<std::string> keys, const std::string& separator, const bool& isSaveKey) const noexcept
    {
        // Lock Vault to read
        ReadLock<RecursiveReadWriteMutex> readLock(RecursiveReadWriteMtx);

        // Open or create file to save data
        std::ofstream csvFile(fileName, std::ios::binary);

        // Checking whether the file was opened successfully
        if (!csvFile.is_open())
        {
            return false;
        }

        // C array to store correct new line characters. 
        const char endOfLine[2] = {13, 10}; // 13 - CR or 0d. 10 - LF or 0a.

        // Save keys to file
        if (isSaveKey)
        {
            if (keys.empty())
            {
                auto keysOrderIt = KeysOrder.cbegin();
                csvFile << *keysOrderIt;
                ++keysOrderIt;
                for (;keysOrderIt != KeysOrder.cend(); ++keysOrderIt) csvFile << separator << *keysOrderIt;
                csvFile.write(endOfLine, 2);
            }
            else 
            {
                auto keysOrderIt = keys.cbegin();
                csvFile << *keysOrderIt;
                ++keysOrderIt;
                for (;keysOrderIt != keys.cend(); ++keysOrderIt) csvFile << separator << *keysOrderIt;
                csvFile.write(endOfLine, 2);
            }
        }

        if (keys.empty())
        {
            for (const auto& record : RecordsSet)
            {
                // Getting the number of items in a record 
                std::size_t counter = record->Size();
                for (const std::string& key : KeysOrder)
                {
                    std::string data;
                    record->GetDataAsString(key, data);
                    csvFile << FormatStringToCsv(data);

                    // Checking not to write a comma to the end of the line
                    if (counter == 1) break;

                    csvFile << separator;
                    --counter;
                }
                csvFile.write(endOfLine, 2);
            }
        }
        else 
        {
            for (const auto& record : RecordsSet)
            {
                std::size_t counter = keys.size();
                for (const std::string& key : keys)
                {
                    std::string data;
                    record->GetDataAsString(key, data);
                    csvFile << FormatStringToCsv(data);

                    // Checking not to write a comma to the end of the line
                    if (counter == 1) break;

                    csvFile << separator;
                    --counter;
                }

                csvFile.write(endOfLine, 2);
            }
        }

        csvFile.close();

        return true;
    }

    bool Vault::ReadFile(const std::string& fileName, const char& separator, const bool& isLoadKeys, const std::vector<std::string> keys) noexcept
    {
        return ReadFile(fileName, false, [](const std::vector<std::string>&, std::vector<std::string>&) {}, separator, isLoadKeys, keys);
    }

    bool Vault::ReadFile(const std::string& fileName, const char& separator, const bool& isLoadKeys, std::function<void (const std::vector<std::string>&, std::vector<std::string>&)> recordHandler) noexcept
    {
        return ReadFile(fileName, true, recordHandler, separator, isLoadKeys, {});
    }

    std::vector<std::pair<std::size_t, std::string>> Vault::GetErrorsInLastReadedFile() const noexcept
    {
        return InvalidFileRecords;
    }

    Vault::~Vault() noexcept
    {
        DropVault();
    }
}