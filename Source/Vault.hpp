#pragma once

#include "Vault.h"
#include "VaultRecordSet.h"
#include "VaultParamInput.hpp"

// This file contains an implementation of the Vault template methods
namespace mvlt
{
    template <class T>
    VaultOperationResult Vault::SetDataToRecord(VaultRecord* dataRecord, const std::string& key, const T& data, bool isCalledFromVault)
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
            RecursiveReadWriteMtx.WriteUnlock();
            return res;
        }

        // Check types
        if (res.SavedType != res.RequestedType)
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::WrongType;
            RecursiveReadWriteMtx.WriteUnlock();
            return res;
        }


        // Get std::unordered_multimap with T key and VaultRecord* value
        VaultHashMapStructure.GetData(key, TtoVaultRecordHashMap);

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


        // Get std::multimap with T key and VaultRecord* value
        VaultMapStructure.GetData(key, TtoVaultRecordMap);

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


        // Check if this method was original called. That mean that this method called not from next if statement.
        // It is required since VaultRecordSet child of Vault and may also call this method
        if (isCalledFromVault)
        {
            // Update data inside VaultRecord pointer inside VaultRecordRef and Vault
            dataRecord->SetData(key, data);
        
            // Update all dependent VaultRecordSets structures
            for (VaultRecordSet* vaultRecordSet : dataRecord->dependentVaultRecordSets)
                vaultRecordSet->SetDataToRecord(dataRecord, key, data, false);
        }

        res.IsOperationSuccess = true;
        res.ResultCode = VaultOperationResultCode::Success;

        // Unlock Vault
        RecursiveReadWriteMtx.WriteUnlock();
        return res;
    }

    template <class T>
    VaultOperationResult Vault::RemoveRecord(bool isDelete, const std::string& key, const T& keyValue)
    {
        // Fill res info known at start
        VaultOperationResult res;
        res.Key = key;
        res.RequestedType = typeid(T);

        RecursiveReadWriteMtx.ReadLock();

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

        // Pointer to store map inside VaultStructureHashMap
        std::unordered_multimap<T, VaultRecord*>* TtoVaultRecordHashMap = nullptr;

        // Checking whether such a key exists
        VaultHashMapStructure.GetData(key, TtoVaultRecordHashMap);
        
        // Iterator to element with T type and keyValue value
        auto TtoVaultRecordIt = TtoVaultRecordHashMap->find(keyValue);
        if (TtoVaultRecordIt != TtoVaultRecordHashMap->end())
        {
            VaultRecord* tmpRec = TtoVaultRecordIt->second;

            for (auto& eraser : VaultRecordErasers)
                eraser.second(tmpRec);

            RecordsSet.erase(tmpRec);
            
            // Check if it is delete. By now it is deletion when EraseRecord called from vault.
            // It is not deleting when RemoveRecord called from VaultRecordSet
            if (isDelete) tmpRec->Invalidate();

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
    VaultOperationResult Vault::RemoveRecords(bool isDelete, const std::string& key, const T& keyValue, const std::size_t& amountOfRecords)
    {
        // Fill res info known at start
        VaultOperationResult res;
        res.Key = key;
        res.RequestedType = typeid(T);

        RecursiveReadWriteMtx.ReadLock();

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

        // Pointer to store map inside VaultStructureHashMap
        std::unordered_multimap<T, VaultRecord*>* TtoVaultRecordHashMap = nullptr;

        // Checking whether such a key exists
        VaultHashMapStructure.GetData(key, TtoVaultRecordHashMap);
        
        // Pair with begin and end iterator with T type and keyValue value
        auto equalRange = TtoVaultRecordHashMap->equal_range(keyValue);
        if (equalRange.first != TtoVaultRecordHashMap->end())
        {
            std::size_t counter = 0;
            for (auto it = equalRange.first; it != equalRange.second;)
            {
                // Save next iterator
                auto next = it;
                ++next;

                ++counter;

                VaultRecord* tmpRec = it->second;
                for (auto& eraser : VaultRecordErasers)
                    eraser.second(tmpRec);

                RecordsSet.erase(tmpRec);

                // Check if it is delete. By now it is deletion when EraseRecord called from vault.
                // It is not deleting when RemoveRecord called from VaultRecordSet
                if (isDelete) tmpRec->Invalidate();
                
                if (counter >= amountOfRecords) break;
                
                it = next;
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
    void Vault::SetKey(const std::string& key, const T& defaultKeyValue)
    {
        RecursiveReadWriteMtx.WriteLock();

        // If the key was added earlier, then it must be deleted
        if (KeysTypes.find(key) != KeysTypes.end())
            RemoveKey(key);

        // Add key type to hash map with keys types
        KeysTypes.emplace(key, typeid(T)); 

        // Add data to template
        RecordTemplate.SetData(key, defaultKeyValue);

        // Create new hash map to store data with template T key
        std::unordered_multimap<T, VaultRecord*>* TtoVaultRecordHashMap = new std::unordered_multimap<T, VaultRecord*>;
        VaultHashMapStructure.SetData(key, TtoVaultRecordHashMap, [](const void* ptr)
            {
                delete* (std::unordered_multimap<T, VaultRecord*>**)ptr;
            }
        );

        // Create new map to store data with template T key
        std::multimap<T, VaultRecord*>* TtoVaultRecordMap = new std::multimap<T, VaultRecord*>;
        VaultMapStructure.SetData(key, TtoVaultRecordMap, [](const void* ptr)
            {
                delete* (std::multimap<T, VaultRecord*>**)ptr;
            }
        );

        // Add function to VaultRecord creation
        VaultRecordAdders.emplace(key, [=](VaultRecord* newRecord)
            {
                // Make temporary variable to store key value
                T value = defaultKeyValue;
                // Try to get key value from new record. If it is not value inside then defaultKeyValue will be used
                newRecord->GetData(key, value);
                TtoVaultRecordHashMap->emplace(value, newRecord);
                TtoVaultRecordMap->emplace(value, newRecord);
            }
        );

        // Add function to TtoVaultRecordHashMap cleareing
        VaultRecordClearers.emplace(key, [=]()
            {
                TtoVaultRecordHashMap->clear();
                TtoVaultRecordMap->clear();
            }
        );

        // Add function to erase newRecord from TtoVaultRecordHashMap
        VaultRecordErasers.emplace(key, [=](VaultRecord* newRecord)
            {
                // Get T type data with key key
                T recordTData;
                newRecord->GetData(key, recordTData);

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

        VaultRecordSorters.emplace(key, [=](std::function<bool(const VaultRecordRef&)> functionToSortedData, bool isReverse)
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
        
        VaultKeyCopiers.emplace(key, [=](VaultRecordSet& vaultRecordSet)
        {
            vaultRecordSet.SetKey(key, defaultKeyValue);
        });  


        // Add new data to record set
        for (auto& it : RecordsSet)
        {
            it->SetData(key, defaultKeyValue);
            TtoVaultRecordHashMap->emplace(defaultKeyValue, it);
            TtoVaultRecordMap->emplace(defaultKeyValue, it);
        }

        RecursiveReadWriteMtx.WriteUnlock();
    }

    template <class T>
    VaultOperationResult Vault::GetKeyValue(const std::string& key, T& defaultKeyValue) const
    {
        VaultOperationResult res;
        res.Key = key;
        res.RequestedType = typeid(T);

        RecursiveReadWriteMtx.ReadLock();

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

        RecordTemplate.GetData(key, defaultKeyValue);
        
        res.IsOperationSuccess = true;
        res.ResultCode = VaultOperationResultCode::Success;
        res.SavedType = res.RequestedType;

        RecursiveReadWriteMtx.ReadUnlock();
        return res;
    }

    template <class T>
    VaultOperationResult Vault::GetRecord(const std::string& key, const T& keyValue, VaultRecordRef& vaultRecordRef) const
    {
        // Fill res info known at start
        VaultOperationResult res;
        res.Key = key;
        res.RequestedType = typeid(T);

        vaultRecordRef.Mtx.lock();
        RecursiveReadWriteMtx.ReadLock();

        // If key not exist
        if(!GetKeyType(key, res.SavedType))
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::WrongKey;
            RecursiveReadWriteMtx.ReadUnlock();
            vaultRecordRef.Mtx.unlock();
            return res;
        }

        // Check types
        if (res.SavedType != res.RequestedType)
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::WrongType;
            RecursiveReadWriteMtx.ReadUnlock();
            vaultRecordRef.Mtx.unlock();
            return res;
        }

        // Pointer to store map inside VaultStructureHashMap
        std::unordered_multimap<T, VaultRecord*>* TtoVaultRecordHashMap = nullptr;

        // Checking whether such a key exists
        VaultHashMapStructure.GetData(key, TtoVaultRecordHashMap);
        
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
        vaultRecordRef.Mtx.unlock();
        return res;
    }

    template <class T>
    VaultOperationResult Vault::GetRecords(const std::string& key, const T& keyValue, std::vector<VaultRecordRef>& recordsRefs, const std::size_t& amountOfRecords) const
    {
        // Fill res info known at start
        VaultOperationResult res;
        res.Key = key;
        res.RequestedType = typeid(T);

        RecursiveReadWriteMtx.ReadLock();

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

        // Pointer to store map inside VaultStructureHashMap
        std::unordered_multimap<T, VaultRecord*>* TtoVaultRecordHashMap = nullptr;

        // Checking whether such a key exists
        VaultHashMapStructure.GetData(key, TtoVaultRecordHashMap);
        
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
    VaultOperationResult Vault::RequestRecords(const std::string& key, const T& keyValue, VaultRecordSet& vaultRecordSet, const std::size_t& amountOfRecords) const
    {
        // Fill res info known at start
        VaultOperationResult res;
        res.Key = key;
        res.RequestedType = typeid(T);
        
        vaultRecordSet.RecursiveReadWriteMtx.WriteLock();
        RecursiveReadWriteMtx.ReadLock();

        // If key not exist
        if(!GetKeyType(key, res.SavedType))
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::WrongKey;
            RecursiveReadWriteMtx.ReadUnlock();
            vaultRecordSet.RecursiveReadWriteMtx.WriteUnlock();

            return res;
        }

        // Check types
        if (res.SavedType != res.RequestedType)
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::WrongType;
            RecursiveReadWriteMtx.ReadUnlock();
            vaultRecordSet.RecursiveReadWriteMtx.WriteUnlock();
            return res;
        }
        
        // Save vaultRecordSet
        const_cast<std::unordered_set<VaultRecordSet*>*>(&RecordSetsSet)->emplace(&vaultRecordSet);

        // Set new parent vault to vaultRecordSet
        vaultRecordSet.ParentVault = const_cast<Vault*>(this);
        vaultRecordSet.IsParentVaultValid = true;
        
        // Copy keys from this to vaultRecordSet
        for (auto& keyCopierIt : VaultKeyCopiers)
            keyCopierIt.second(vaultRecordSet);
        
        // Pointer to store map inside VaultStructureHashMap
        std::unordered_multimap<T, VaultRecord*>* TtoVaultRecordHashMap = nullptr;

        // Get map
        VaultHashMapStructure.GetData(key, TtoVaultRecordHashMap);
        
        // Pair with begin and end iterator with T type and keyValue value
        auto equalRange = TtoVaultRecordHashMap->equal_range(keyValue);
        if (equalRange.first != TtoVaultRecordHashMap->end())
        {
            std::size_t counter = 0;
            for (auto it = equalRange.first; it != equalRange.second; ++it)
            {
                // Add pointer to record from this to vaultRecordSet
                vaultRecordSet.RecordsSet.emplace(it->second);
                
                // Add pointer to record from this to vaultRecordSet structure
                for (auto& adder : vaultRecordSet.VaultRecordAdders)
                    adder.second(it->second);

                // Lock VaultRecord to thread safety add new dependent VaultRecordSet
                it->second->Mtx.lock();
                it->second->dependentVaultRecordSets.emplace(&vaultRecordSet);
                it->second->Mtx.unlock();

                ++counter;
                if (counter > amountOfRecords) break;
            }
        }

        RecursiveReadWriteMtx.ReadUnlock();
        vaultRecordSet.RecursiveReadWriteMtx.WriteUnlock();

        res.ResultCode = VaultOperationResultCode::Success;
        res.SavedType = res.RequestedType;
        res.IsOperationSuccess = true;

        return res;
    }

    template <class T>
    VaultOperationResult Vault::EraseRecord(const std::string& key, const T& keyValue)
    {
        return RemoveRecord(true, key, keyValue);
    }

    template <class T>
    VaultOperationResult Vault::EraseRecords(const std::string& key, const T& keyValue, const std::size_t& amountOfRecords)
    {
        return RemoveRecords(true, key, keyValue, amountOfRecords);
    }

    template<class F>
    void Vault::SortBy(const std::string& key, const F&& func, const bool& isReverse, const std::size_t& amountOfRecords) const
    {
        std::size_t counter = 0;

        RecursiveReadWriteMtx.ReadLock();
        
        auto f = VaultRecordSorters.find(key);
        if (f != VaultRecordSorters.end())
        {
            VaultRecordSorters.find(key)->second([&](const VaultRecordRef& vaultRecordRef)
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