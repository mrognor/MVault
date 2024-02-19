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

    bool Vault::RemoveKey(const std::string& keyName)
    {
        RecursiveReadWriteMtx.WriteLock();
        if (!IsKeyExist(keyName)) 
        {
            RecursiveReadWriteMtx.WriteUnlock();
            return false;
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
        return true;
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
                if (counter >= amountOfRecords) return false;
                
                res.emplace_back(vaultRecordRef);
                ++counter;
                return true;
            }, isReverse);

        RecursiveReadWriteMtx.ReadUnlock();
        
        return res;
    }

    void Vault::PrintVault(const std::size_t& amountOfRecords) const
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

    void Vault::PrintAsTable(const std::size_t& amountOfRecords, const std::vector<std::string> keys) const
    {
        RecursiveReadWriteMtx.ReadLock();
        if (keys.empty())
        {
            if (VaultMapStructure.Size() == 0)
            {
                std::cout << "Vault does not contain keys!" << std::endl;
                std::cout << " (" << RecordsSet.size() << " records)" << std::endl;
            }
            else
                PrintContainerAsTable(RecordsSet, amountOfRecords, GetKeys());
        }
        else
            PrintContainerAsTable(RecordsSet, amountOfRecords, keys);
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