#pragma once

#include "Vault.h"

// This file contains an implementation of the Vault template methods
namespace mvlt
{
    template <class T>
    VaultOperationResult Vault::SetDataToRecord(VaultRecord* dataRecord, const std::string& key, const T& data)
    {
        // Fill res info known at start
        VaultOperationResult res;
        res.Key = key;
        res.RequestedType = typeid(T);

        // A pointer for storing a std::unordered_multimap in which a template data type is used as a key, 
        // and a pointer to the DataHashMap is used as a value
        std::unordered_multimap<T, VaultRecord*>* TtoVaultRecordHashMap = nullptr;

        // A pointer for storing a std::multimap in which a template data type is used as a key, 
        // and a pointer to the DataHashMap is used as a value
        std::multimap<T, VaultRecord*>* TtoVaultRecordMap = nullptr;

        // Lock Vault to write
        RecursiveReadWriteMtx.WriteLock();

        // Check if dataRecord valid
        if (dataRecord == nullptr || !dataRecord->GetIsValid())
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::DataRecordNotValid;
            RecursiveReadWriteMtx.WriteUnlock();
            return res;
        }

        // If key not exist
        if(!GetKeyType(key, res.SavedType))
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::WrongKey;
            RecursiveReadWriteMtx.ReadUnlock();
            return res;
        }

        // Check types
        if (res.SavedType != res.RequestedType)
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::WrongType;
            RecursiveReadWriteMtx.ReadUnlock();
            return res;
        }

        // Get std::unordered_multimap with T key and VaultRecord* value
        VaultHashMapStructure.GetData(key, TtoVaultRecordHashMap);

        // Get std::multimap with T key and VaultRecord* value
        VaultMapStructure.GetData(key, TtoVaultRecordMap);

        // Get the current value of the key key inside the VaultRecordRef and save it for further work
        T oldData;
        dataRecord->GetData(key, oldData);

        // Remove oldData from TtoVaultRecordHashMap from VaultHashMapStructure
        auto FirstAndLastIteratorsWithKeyOnHashMap = TtoVaultRecordHashMap->equal_range(oldData);

        // Iterate over all data records with oldData key
        for (auto& it = FirstAndLastIteratorsWithKeyOnHashMap.first; it != FirstAndLastIteratorsWithKeyOnHashMap.second; ++it)
        {
            // Find required data record
            if (it->second == dataRecord)
            {
                TtoVaultRecordHashMap->erase(it);
                break;
            }
        }

        // Add new data to TtoVaultRecordHashMap to Vault VaultHashMapStructure
        TtoVaultRecordHashMap->emplace(data, dataRecord);

        // Remove oldData from TtoVaultRecordMap from VaultMapStructure
        auto FirstAndLastIteratorsWithKeyOnMap = TtoVaultRecordMap->equal_range(oldData);

        // Iterate over all data records with oldData key
        for (auto& it = FirstAndLastIteratorsWithKeyOnMap.first; it != FirstAndLastIteratorsWithKeyOnMap.second; ++it)
        {
            // Find required data record
            if (it->second == dataRecord)
            {
                TtoVaultRecordMap->erase(it);
                break;
            }
        }

        // Add new data to TtoVaultRecordMap to Vault VaultMapStructure
        TtoVaultRecordMap->emplace(data, dataRecord);

        // Update data inside VaultRecord pointer inside VaultRecordRef and Vault
        dataRecord->SetData(key, data);
        res.IsOperationSuccess = true;
        res.ResultCode = VaultOperationResultCode::Success;

        // Unlock Vault
        RecursiveReadWriteMtx.WriteUnlock();
        return res;
    }

    template <class T>
    void Vault::SetKey(const std::string& keyName, const T& defaultKeyValue)
    {
        RecursiveReadWriteMtx.WriteLock();

        // If the key was added earlier, then it must be deleted
        if (KeysTypes.find(keyName) != KeysTypes.end())
            RemoveKey(keyName);

        // Add key type to hash map with keys types
        KeysTypes.emplace(keyName, typeid(T)); 

        // Add data to template
        RecordTemplate.SetData(keyName, defaultKeyValue);

        // Create new hash map to store data with template T key
        std::unordered_multimap<T, VaultRecord*>* TtoVaultRecordHashMap = new std::unordered_multimap<T, VaultRecord*>;
        VaultHashMapStructure.SetData(keyName, TtoVaultRecordHashMap, [](const void* ptr)
            {
                delete* (std::unordered_multimap<T, VaultRecord*>**)ptr;
            }
        );

        // Create new map to store data with template T key
        std::multimap<T, VaultRecord*>* TtoVaultRecordMap = new std::multimap<T, VaultRecord*>;
        VaultMapStructure.SetData(keyName, TtoVaultRecordMap, [](const void* ptr)
            {
                delete* (std::multimap<T, VaultRecord*>**)ptr;
            }
        );

        // Add function to VaultRecord creation
        VaultRecordAdders.emplace(keyName, [=](VaultRecord* newRecord)
            {
                // Make temporary variable to store key value
                T value = defaultKeyValue;
                // Try to get key value from new record. If it is not value inside then defaultKeyValue will be used
                newRecord->GetData(keyName, value);
                TtoVaultRecordHashMap->emplace(value, newRecord);
                TtoVaultRecordMap->emplace(value, newRecord);
            }
        );

        // Add function to TtoVaultRecordHashMap cleareing
        VaultRecordClearers.emplace(keyName, [=]()
            {
                TtoVaultRecordHashMap->clear();
                TtoVaultRecordMap->clear();
            }
        );

        // Add function to erase newRecord from TtoVaultRecordHashMap
        VaultRecordErasers.emplace(keyName, [=](VaultRecord* newRecord)
            {
                // Get T type data with keyName key
                T recordTData;
                newRecord->GetData(keyName, recordTData);

                // Find all elements on multi_map with recordTData value
                auto FirstAndLastIteratorsWithKeyOnHashMap = TtoVaultRecordHashMap->equal_range(recordTData);

                // Find newRecord and erase it from TtoVaultRecordHashMap
                for (auto& it = FirstAndLastIteratorsWithKeyOnHashMap.first; it != FirstAndLastIteratorsWithKeyOnHashMap.second; ++it)
                {
                    if (it->second == newRecord)
                    {
                        TtoVaultRecordHashMap->erase(it);
                        break;
                    }
                }

                // Find all elements on map with recordTData value
                auto FirstAndLastIteratorsWithKeyOnMap = TtoVaultRecordMap->equal_range(recordTData);
                // Find newRecord and erase it from TtoVaultRecordHashMap
                for (auto& it = FirstAndLastIteratorsWithKeyOnMap.first; it != FirstAndLastIteratorsWithKeyOnMap.second; ++it)
                {
                    if (it->second == newRecord)
                    {
                        TtoVaultRecordMap->erase(it);
                        break;
                    }
                }
            }
        );

        VaultRecordSorters.emplace(keyName, [=](std::function<bool(const VaultRecordRef&)> functionToSortedData, bool isReverse)
        {
            if (!isReverse)
            {
                for (const auto& it : *TtoVaultRecordMap)
                    if(!functionToSortedData(VaultRecordRef(it.second, this)))
                        break;
            }
            else
            {
                for (auto it = TtoVaultRecordMap->rbegin(); it != TtoVaultRecordMap->rend(); ++it)
                    if(!functionToSortedData(VaultRecordRef(it->second, this)))
                        break;
            }
        });
        
        // Add new data to record set
        for (auto& it : RecordsSet)
        {
            it->SetData(keyName, defaultKeyValue);
            TtoVaultRecordHashMap->emplace(defaultKeyValue, it);
            TtoVaultRecordMap->emplace(defaultKeyValue, it);
        }

        RecursiveReadWriteMtx.WriteUnlock();
    }

    template <class T>
    bool Vault::GetKeyValue(const std::string& keyName, T& defaultKeyValue) const
    {
        bool res;
        RecursiveReadWriteMtx.ReadLock();
        res = RecordTemplate.GetData(keyName, defaultKeyValue);
        RecursiveReadWriteMtx.ReadUnlock();
        return res;
    }

    template <class T>
    VaultOperationResult Vault::GetRecord(const std::string& keyName, const T& keyValue, VaultRecordRef& vaultRecordRef) const
    {
        // Fill res info known at start
        VaultOperationResult res;
        res.Key = keyName;
        res.RequestedType = typeid(T);

        RecursiveReadWriteMtx.ReadLock();

        // If key not exist
        if(!GetKeyType(keyName, res.SavedType))
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::WrongKey;
            RecursiveReadWriteMtx.ReadUnlock();
            return res;
        }

        // Check types
        if (res.SavedType != res.RequestedType)
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::WrongType;
            RecursiveReadWriteMtx.ReadUnlock();
            return res;
        }

        // Pointer to store map inside VaultStructureHashMap
        std::unordered_multimap<T, VaultRecord*>* TtoVaultRecordHashMap = nullptr;

        // Checking whether such a key exists
        VaultHashMapStructure.GetData(keyName, TtoVaultRecordHashMap);
        
        // Iterator to element with T type and keyValue value
        auto TtoVaultRecordIt = TtoVaultRecordHashMap->find(keyValue);
        if (TtoVaultRecordIt != TtoVaultRecordHashMap->end())
        {
            vaultRecordRef.SetRecord(TtoVaultRecordIt->second, const_cast<Vault*>(this));
            res.IsOperationSuccess = true;
            res.ResultCode = VaultOperationResultCode::Success;
        }
        else
        {
            vaultRecordRef.SetRecord(nullptr, const_cast<Vault*>(this));
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::WrongValue;
        }

        RecursiveReadWriteMtx.ReadUnlock();
        return res;
    }

    template <class T>
    VaultOperationResult Vault::GetRecords(const std::string& keyName, const T& keyValue, std::vector<VaultRecordRef>& recordsRefs, const std::size_t& amountOfRecords) const
    {
        // Fill res info known at start
        VaultOperationResult res;
        res.Key = keyName;
        res.RequestedType = typeid(T);

        RecursiveReadWriteMtx.ReadLock();

        // If key not exist
        if(!GetKeyType(keyName, res.SavedType))
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::WrongKey;
            RecursiveReadWriteMtx.ReadUnlock();
            return res;
        }

        // Check types
        if (res.SavedType != res.RequestedType)
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::WrongType;
            RecursiveReadWriteMtx.ReadUnlock();
            return res;
        }

        // Pointer to store map inside VaultStructureHashMap
        std::unordered_multimap<T, VaultRecord*>* TtoVaultRecordHashMap = nullptr;

        // Checking whether such a key exists
        VaultHashMapStructure.GetData(keyName, TtoVaultRecordHashMap);
        
        // Pair with begin and end iterator with T type and keyValue value
        auto equalRange = TtoVaultRecordHashMap->equal_range(keyValue);
        if (equalRange.first != TtoVaultRecordHashMap->end())
        {
            std::size_t counter = 0;
            for (auto it = equalRange.first; it != equalRange.second; ++it)
            {
                ++counter;
                recordsRefs.emplace_back(VaultRecordRef(it->second, const_cast<Vault*>(this)));
                if (counter >= amountOfRecords) break;
            }
        }
        else
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::WrongValue;
        }

        RecursiveReadWriteMtx.ReadUnlock();
        return res;
    }

    template <class T>
    VaultOperationResult Vault::EraseRecord(const std::string& keyName, const T& keyValue)
    {
        // Fill res info known at start
        VaultOperationResult res;
        res.Key = keyName;
        res.RequestedType = typeid(T);

        RecursiveReadWriteMtx.ReadLock();

        // If key not exist
        if(!GetKeyType(keyName, res.SavedType))
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::WrongKey;
            RecursiveReadWriteMtx.ReadUnlock();
            return res;
        }

        // Check types
        if (res.SavedType != res.RequestedType)
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::WrongType;
            RecursiveReadWriteMtx.ReadUnlock();
            return res;
        }

        // Pointer to store map inside VaultStructureHashMap
        std::unordered_multimap<T, VaultRecord*>* TtoVaultRecordHashMap = nullptr;

        // Checking whether such a key exists
        VaultHashMapStructure.GetData(keyName, TtoVaultRecordHashMap);
        
        // Iterator to element with T type and keyValue value
        auto TtoVaultRecordIt = TtoVaultRecordHashMap->find(keyValue);
        if (TtoVaultRecordIt != TtoVaultRecordHashMap->end())
        {
            VaultRecord* tmpRec = TtoVaultRecordIt->second;

            for (auto& eraser : VaultRecordErasers)
                eraser.second(tmpRec);

            RecordsSet.erase(tmpRec);
            tmpRec->Invalidate();

            res.IsOperationSuccess = true;
            res.ResultCode = VaultOperationResultCode::Success;
        }
        else
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::WrongValue;
        }

        RecursiveReadWriteMtx.ReadUnlock();
        return res;
    }

    template <class T>
    VaultOperationResult Vault::EraseRecords(const std::string& keyName, const T& keyValue, const std::size_t& amountOfRecords)
    {
        // Fill res info known at start
        VaultOperationResult res;
        res.Key = keyName;
        res.RequestedType = typeid(T);

        RecursiveReadWriteMtx.ReadLock();

        // If key not exist
        if(!GetKeyType(keyName, res.SavedType))
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::WrongKey;
            RecursiveReadWriteMtx.ReadUnlock();
            return res;
        }

        // Check types
        if (res.SavedType != res.RequestedType)
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::WrongType;
            RecursiveReadWriteMtx.ReadUnlock();
            return res;
        }

        // Pointer to store map inside VaultStructureHashMap
        std::unordered_multimap<T, VaultRecord*>* TtoVaultRecordHashMap = nullptr;

        // Checking whether such a key exists
        VaultHashMapStructure.GetData(keyName, TtoVaultRecordHashMap);
        
        // Pair with begin and end iterator with T type and keyValue value
        auto equalRange = TtoVaultRecordHashMap->equal_range(keyValue);
        if (equalRange.first != TtoVaultRecordHashMap->end())
        {
            std::size_t counter = 0;
            for (auto it = equalRange.first; it != equalRange.second; ++it)
            {
                ++counter;

                for (auto& eraser : VaultRecordErasers)
                    eraser.second(it->second);

                RecordsSet.erase(it->second);

                if (counter >= amountOfRecords) break;
            }
        }
        else
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::WrongValue;
        }

        RecursiveReadWriteMtx.ReadUnlock();
        return res;
    }

    template<class F>
    void Vault::SortBy(const std::string& keyName, const F&& func, const bool& isReverse, const std::size_t& amountOfRecords) const
    {
        std::size_t counter = 0;

        RecursiveReadWriteMtx.ReadLock();
        
        auto f = VaultRecordSorters.find(keyName);
        if (f != VaultRecordSorters.end())
        {
            VaultRecordSorters.find(keyName)->second([&](const VaultRecordRef& vaultRecordRef)
                {
                    if (counter >= amountOfRecords) return false;
                    
                    func(vaultRecordRef);
                    ++counter;
                    return true;
                }, isReverse);
        }
        RecursiveReadWriteMtx.ReadUnlock();
    }
}