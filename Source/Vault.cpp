#include "Vault.h"

namespace mvlt
{
    Vault::Vault() {}

    bool Vault::IsKeyExist(const std::string& keyName) const
    {
        bool res;
        RecursiveReadWriteMtx.ReadLock();
        res = RecordTemplate.IsData(keyName);
        RecursiveReadWriteMtx.ReadUnlock();
        return res;
    }

    void Vault::RemoveKey(const std::string& keyName)
    {
        RecursiveReadWriteMtx.WriteLock();
        if (!IsKeyExist(keyName)) 
        {
            RecursiveReadWriteMtx.WriteUnlock();
            return;
        }

        // Erase key from record template
        RecordTemplate.EraseData(keyName);

        // Erase key from VaultHashMapStructure
        VaultHashMapStructure.EraseData(keyName);

        // Erase key from VaultMapStructure
        VaultMapStructure.EraseData(keyName);

        // Erase key from all maps
        VaultRecordAdders.erase(keyName);
        VaultRecordClearers.erase(keyName);
        VaultRecordErasers.erase(keyName);
        VaultRecordSorters.erase(keyName);

        // Erase key data from all records
        for (auto& it : RecordsSet)
            it->EraseData(keyName);
        
        RecursiveReadWriteMtx.WriteUnlock();
    }

    VaultRecordRef Vault::CreateRecord()
    {
        RecursiveReadWriteMtx.WriteLock();

        // Create new record
        VaultRecord* newData = new VaultRecord(RecordTemplate);
        // Add new record to set
        RecordsSet.emplace(newData);

        // Add new record to every maps inside VaultStructureHashMap
        for (auto& it : VaultRecordAdders)
            it.second(newData);

        VaultRecordRef res(newData, &VaultHashMapStructure, &VaultMapStructure, &RecursiveReadWriteMtx);

        RecursiveReadWriteMtx.WriteUnlock();

        return res;
    }

    VaultRecordRef Vault::CreateRecord(const std::vector<std::pair<std::string, DataSaver>>& params)
    {
        RecursiveReadWriteMtx.WriteLock();

        // Create new record
        VaultRecord* newData = new VaultRecord(RecordTemplate);

        // Copy data from function parametrs
        for (auto& it : params)
            if (newData->IsData(it.first))
                newData->SetDataFromDataSaver(it.first, it.second);

        // Add new record to set
        RecordsSet.emplace(newData);

        // Add new record to every maps inside VaultStructureHashMap
        for (auto& it : VaultRecordAdders)
            it.second(newData);
        
        VaultRecordRef res(newData, &VaultHashMapStructure, &VaultMapStructure, &RecursiveReadWriteMtx);
        
        RecursiveReadWriteMtx.WriteUnlock();

        return res;
    }

    void Vault::DropVault()
    {
        RecursiveReadWriteMtx.WriteLock();

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

        // Clear VaultMapStructure
        VaultMapStructure.Clear();


        // Clear all maps with functions
        VaultRecordAdders.clear();
        VaultRecordClearers.clear();
        VaultRecordErasers.clear();
        VaultRecordSorters.clear();

        // Delete all Records
        for (auto& it : RecordsSet)
            it->Invalidate();

        // Clear RecordsSet
        RecordsSet.clear();

        RecursiveReadWriteMtx.WriteUnlock();
    }

    void Vault::DropData()
    {
        RecursiveReadWriteMtx.WriteLock();

        // Call functions to clear VaultHashMapStructure without
        for (auto& it : VaultRecordClearers)
            it.second();

        // Delete all Records
        for (auto& it : RecordsSet)
            it->Invalidate();

        // Clear RecordsSet
        RecordsSet.clear();

        RecursiveReadWriteMtx.WriteUnlock();
    }

    void Vault::EraseRecord(const VaultRecordRef& recordRefToErase)
    {
        RecursiveReadWriteMtx.WriteLock();

        if (RecordsSet.count(recordRefToErase.DataRecord) == 0)
        {
            RecursiveReadWriteMtx.WriteUnlock();
            return;
        }

        // Get pointer to record from record ref
        VaultRecord* tmpRec = recordRefToErase.DataRecord;
        // Iterate over all VaultRecordErasers and call function to erase record from structure
        for (auto& erasers : VaultRecordErasers)
            erasers.second(tmpRec);

        RecordsSet.erase(tmpRec);
        tmpRec->Invalidate();

        RecursiveReadWriteMtx.WriteUnlock();
    }

    std::size_t Vault::Size() const
    {
        std::size_t res;
        RecursiveReadWriteMtx.ReadLock();
        res = RecordsSet.size();
        RecursiveReadWriteMtx.ReadUnlock();
        return res;
    }

    std::vector<std::string> Vault::GetKeys() const
    {
        std::vector<std::string> res;

        RecursiveReadWriteMtx.ReadLock();
        for (const auto& it : VaultMapStructure) res.emplace_back(it.first);
        RecursiveReadWriteMtx.ReadUnlock();
        
        return res;
    }

    std::vector<VaultRecordRef> Vault::GetSortedRecords(const std::string& keyName, const bool& isReverse, const std::size_t& amountOfRecords) const
    {
        std::vector<VaultRecordRef> res;
        std::size_t counter = 0;

        RecursiveReadWriteMtx.ReadLock();
        
        /// \todo Проверка
        VaultRecordSorters.find(keyName)->second([&](const VaultRecordRef& vaultRecordRef)
            {
                if (counter >= amountOfRecords)  return false;
                
                res.emplace_back(vaultRecordRef);
                ++counter;
                return true;
            }, isReverse);

        RecursiveReadWriteMtx.ReadUnlock();
        
        return res;
    }

    void Vault::PrintVault(const std::size_t amountOfRecords) const
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

    void Vault::PrintAsTable(const std::size_t amountOfRecords) const
    {
        RecursiveReadWriteMtx.ReadLock();
        
        std::vector<std::string> keys = GetKeys();
        std::vector<std::size_t> maxLengths(keys.size());

        // Fill each max length using key length
        for (std::size_t i = 0; i < keys.size(); ++i)
            maxLengths[i] = keys[i].length();
        
        // Find each real max length
        for (std::size_t i = 0; i < keys.size(); ++i)
        {
            for (const auto& record : RecordsSet)
            {
                DataSaver dataSaver;
                record->GetDataSaver(keys[i], dataSaver);
                if (dataSaver.Str().length() > maxLengths[i]) maxLengths[i] = dataSaver.Str().length();
            }
        }

        // Print splitter
        std::cout << "+";
        for (std::size_t i = 0; i < keys.size(); ++i)
        {
            std::cout << "-";
            for (std::size_t j = 0; j < maxLengths[i]; ++j) std::cout << "-";
            std::cout << "-+";
        }
        std::cout << std::endl;

        // Print header
        std::cout << "|";
        for (std::size_t i = 0; i < keys.size(); ++i)
        {
            std::cout << " " << keys[i];
            for (std::size_t j = keys[i].length(); j < maxLengths[i]; ++j) std::cout << " ";
            std::cout << " |";
        }
        std::cout << std::endl;

        // Print splitter
        std::cout << "+";
        for (std::size_t i = 0; i < keys.size(); ++i)
        {
            std::cout << "-";
            for (std::size_t j = 0; j < maxLengths[i]; ++j) std::cout << "-";
            std::cout << "-+";
        }
        std::cout << std::endl;

        // Print records
        std::size_t counter = 0;
        for (const auto& record : RecordsSet)
        {
            std::cout << "|";
            for (std::size_t i = 0; i < keys.size(); ++i)
            {
                DataSaver dataSaver;
                record->GetDataSaver(keys[i], dataSaver);
                std::cout << " " << dataSaver.Str();
                for (std::size_t j = dataSaver.Str().length(); j < maxLengths[i]; ++j) std::cout << " ";
                std::cout << " |";
            }
            std::cout << std::endl;
            
            ++counter;
            if (counter == amountOfRecords) break;
        }

        // Print splitter
        std::cout << "+";
        for (std::size_t i = 0; i < keys.size(); ++i)
        {
            std::cout << "-";
            for (std::size_t j = 0; j < maxLengths[i]; ++j) std::cout << "-";
            std::cout << "-+";
        }
        std::cout << std::endl;

        std::cout << " (" << Size() << " records)" << std::endl;

        RecursiveReadWriteMtx.ReadUnlock();
    }

    Vault::~Vault()
    {
        RecursiveReadWriteMtx.WriteLock();
        
        // Clear VaultHashMapStructure
        for (auto& it : VaultHashMapStructure)
            it.second.ResetData();

        // Clear VaultMapStructure
        for (auto& it : VaultMapStructure)
            it.second.ResetData();

        // Clear all records
        for (auto& it : RecordsSet)
            it->Invalidate();

        RecursiveReadWriteMtx.WriteUnlock();
    }
}