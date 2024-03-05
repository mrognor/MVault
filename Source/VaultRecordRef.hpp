#pragma once

#include "VaultRecordRef.h"

// This file contains an implementation of the VaultRecordRef template methods
namespace mvlt 
{
    template <class T>
    bool VaultRecordRef::SetData(const std::string& key, const T& data)
    {
        // A pointer for storing a std::unordered_multimap in which a template data type is used as a key, 
        // and a pointer to the DataHashMap is used as a value
        std::unordered_multimap<T, VaultRecord*>* TtoVaultRecordHashMap = nullptr;

        // A pointer for storing a std::multimap in which a template data type is used as a key, 
        // and a pointer to the DataHashMap is used as a value
        std::multimap<T, VaultRecord*>* TtoVaultRecordMap = nullptr;

        // Lock Vault to write
        Vlt->RecursiveReadWriteMtx.WriteLock();

        // Get std::unordered_multimap with T key and VaultRecord* value
        if (!Vlt->VaultHashMapStructure.GetData(key, TtoVaultRecordHashMap)) 
        {
            Vlt->RecursiveReadWriteMtx.WriteUnlock();
            return false;
        }

        // Get std::multimap with T key and VaultRecord* value
        if (!Vlt->VaultMapStructure.GetData(key, TtoVaultRecordMap))
        {
            Vlt->RecursiveReadWriteMtx.WriteUnlock();
            return false;
        }

        if (DataRecord == nullptr || !DataRecord->GetIsValid())
        {
            Vlt->RecursiveReadWriteMtx.WriteUnlock();
            return false;
        }

        // Get the current value of the key key inside the VaultRecordRef and save it for further work
        T oldData;
        DataRecord->GetData(key, oldData);

        // Remove oldData from TtoVaultRecordHashMap from VaultHashMapStructure
        auto FirstAndLastIteratorsWithKeyOnHashMap = TtoVaultRecordHashMap->equal_range(oldData);

        // Iterate over all data records with oldData key
        for (auto& it = FirstAndLastIteratorsWithKeyOnHashMap.first; it != FirstAndLastIteratorsWithKeyOnHashMap.second; ++it)
        {
            // Find required data record
            if (it->second == DataRecord)
            {
                TtoVaultRecordHashMap->erase(it);
                break;
            }
        }

        // Add new data to TtoVaultRecordHashMap to Vault VaultHashMapStructure
        TtoVaultRecordHashMap->emplace(data, DataRecord);

        // Remove oldData from TtoVaultRecordMap from VaultMapStructure
        auto FirstAndLastIteratorsWithKeyOnMap = TtoVaultRecordMap->equal_range(oldData);

        // Iterate over all data records with oldData key
        for (auto& it = FirstAndLastIteratorsWithKeyOnMap.first; it != FirstAndLastIteratorsWithKeyOnMap.second; ++it)
        {
            // Find required data record
            if (it->second == DataRecord)
            {
                TtoVaultRecordMap->erase(it);
                break;
            }
        }

        // Add new data to TtoVaultRecordMap to Vault VaultMapStructure
        TtoVaultRecordMap->emplace(data, DataRecord);

        // Update data inside VaultRecord pointer inside VaultRecordRef and Vault
        DataRecord->SetData(key, data);

        // Unlock Vault
        Vlt->RecursiveReadWriteMtx.WriteUnlock();
        return true;
    }

    template <class T>
    bool VaultRecordRef::GetData(const std::string& key, T& data) const
    {
        bool res;
        Mtx.lock();
        Vlt->RecursiveReadWriteMtx.ReadLock();
        if (DataRecord->GetIsValid()) res = DataRecord->GetData(key, data);
        Vlt->RecursiveReadWriteMtx.ReadUnlock();
        Mtx.unlock();
        return res;
    }
}