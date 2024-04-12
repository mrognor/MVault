#pragma once

#include "Vault.h"
#include "VaultRecordSet.h"
#include "VaultParamInput.hpp"

// This file contains an implementation of the Vault template methods
namespace mvlt
{
    template <class T>
    VaultOperationResult Vault::SetDataToRecord(VaultRecord* dataRecord, const std::string& key, const T& data, const bool& isCalledFromVault)
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
    VaultOperationResult Vault::RemoveRecord(const std::string& key, const T& keyValue, const bool& isCalledFromVault)
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
            if (isCalledFromVault) 
            {
                tmpRec->Mtx.lock();
                for (VaultRecordSet* set : tmpRec->dependentVaultRecordSets)
                {
                    set->RecursiveReadWriteMtx.WriteLock();
                    static_cast<Vault*>(set)->RemoveRecord(tmpRec, false);
                    set->RecursiveReadWriteMtx.WriteUnlock();
                }
                tmpRec->Mtx.unlock();

                tmpRec->Invalidate();
            }

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
    VaultOperationResult Vault::RemoveRecords(const std::string& key, const T& keyValue, const std::size_t& amountOfRecords, const bool& isCalledFromVault)
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
                if (isCalledFromVault) 
                {
                    tmpRec->Mtx.lock();
                    for (VaultRecordSet* set : tmpRec->dependentVaultRecordSets)
                    {
                        set->RecursiveReadWriteMtx.WriteLock();
                        static_cast<Vault*>(set)->RemoveRecord(tmpRec, false);
                        set->RecursiveReadWriteMtx.WriteUnlock();
                    }
                    tmpRec->Mtx.unlock();
                    tmpRec->Invalidate();
                }
                
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
    VaultOperationResult Vault::RequestRecords(const VaultRequestType& requestType, const std::string& key, const T& beginKeyValue,
        const T& endKeyValue, VaultRecordSet& vaultRecordSet, const bool& isIncludeBeginKeyValue, 
        const bool& isIncludeEndKeyValue, const std::size_t& amountOfRecords) const
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
        RecordSetsSet.emplace(&vaultRecordSet);

        // Set new parent vault to vaultRecordSet
        vaultRecordSet.ParentVault = const_cast<Vault*>(this);
        vaultRecordSet.IsParentVaultValid = true;
        
        // Copy keys from this to vaultRecordSet
        for (auto& keyCopierIt : VaultKeyCopiers)
            keyCopierIt.second(vaultRecordSet);
        
        // Check if request type is equal
        if (requestType == VaultRequestType::Equal)
        {
            // Pointer to store hash map inside VaultStructureHashMap
            std::unordered_multimap<T, VaultRecord*>* TtoVaultRecordHashMap = nullptr;

            // Get hash map
            VaultHashMapStructure.GetData(key, TtoVaultRecordHashMap);
            
            // Pair with begin and end iterator with T type and beginKeyValue value
            auto equalRange = TtoVaultRecordHashMap->equal_range(beginKeyValue);
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
        }
        else 
        {
            // Pointer to store map inside VaultStructureHashMap
            std::multimap<T, VaultRecord*>* TtoVaultRecordMap = nullptr;

            // Get map
            VaultMapStructure.GetData(key, TtoVaultRecordMap);
            
            // Iterator to set it in switch
            decltype(TtoVaultRecordMap->end()) startIt, endIt;

            // Lambda function for finding greater value iterator
            auto findGreater = [&]()
                {
                    bool flag = false;
                    startIt = TtoVaultRecordMap->lower_bound(beginKeyValue);

                    // If it is not beginKeyValue in map and it is value bigger than beginKeyValue in map
                    if (startIt != TtoVaultRecordMap->end() && startIt->first > beginKeyValue)
                        return;
                        
                    // Iterate to the next value
                    for (auto it = startIt; it != TtoVaultRecordMap->end(); ++it)
                    {
                        if (it->first != startIt->first) 
                        {
                            startIt = it;
                            flag = true;
                            break;
                        }
                    }
                
                    // Check if it is any record with key value greater than beginKeyValue 
                    if (!flag) startIt = TtoVaultRecordMap->end();
                };

            // Lambda function for finding less value iterator
            auto findLess = [&]()
                {
                    endIt = TtoVaultRecordMap->upper_bound(endKeyValue);

                    // Check if TtoVaultRecordMap size not zero and endIt not last iterator
                    if (endIt != TtoVaultRecordMap->begin() && TtoVaultRecordMap->size() > 0)
                    {
                        // Upper bound return next iterator with value greater then endKeyValue
                        --endIt;

                        // Iterate to previous value
                        while(endIt != TtoVaultRecordMap->begin() && endIt->first == endKeyValue)
                            --endIt;
                        
                        // Increase iterator to add later last element in vaultRecordSet 
                        ++endIt;
                    }
                };

            // Switch by request type
            switch (requestType) 
            {
            case VaultRequestType::GreaterOrEqual:
                startIt = TtoVaultRecordMap->lower_bound(beginKeyValue);
                endIt = TtoVaultRecordMap->end();
                break;
            
            case VaultRequestType::Greater:
                findGreater();
                endIt = TtoVaultRecordMap->end();
                break;
            
            case VaultRequestType::Less:
                startIt = TtoVaultRecordMap->begin();
                findLess();
                break;
            
            case VaultRequestType::LessOrEqual:
                startIt = TtoVaultRecordMap->begin();
                endIt = TtoVaultRecordMap->upper_bound(beginKeyValue);
                break;
            
            case VaultRequestType::Interval:
                if (beginKeyValue > endKeyValue || (beginKeyValue == endKeyValue && (isIncludeBeginKeyValue == false || isIncludeEndKeyValue == false)))
                {
                    startIt = TtoVaultRecordMap->end();
                    endIt = TtoVaultRecordMap->end();
                    break;
                }

                if(isIncludeBeginKeyValue) startIt = TtoVaultRecordMap->lower_bound(beginKeyValue);
                else findGreater();
                if(isIncludeEndKeyValue) endIt = TtoVaultRecordMap->upper_bound(endKeyValue);
                else findLess();
                break;

            case VaultRequestType::Equal:
                break;
            }

            std::size_t counter = 0;
            for (auto it = startIt; it != endIt; ++it)
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
                if (counter >= amountOfRecords) break;
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
    bool Vault::AddKey(const std::string& key, const T& defaultKeyValue, const bool& isCalledFromVault)
    {
        RecursiveReadWriteMtx.WriteLock();

        // If the key was added earlier, then it must be deleted
        if (KeysTypes.find(key) != KeysTypes.end())
        {
            RecursiveReadWriteMtx.WriteUnlock();
            return false;
        }

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
            vaultRecordSet.AddKey(key, defaultKeyValue);
        });  


        if (isCalledFromVault)
        {
            // Add new data to record set
            for (auto& it : RecordsSet)
            {
                it->SetData(key, defaultKeyValue);
                TtoVaultRecordHashMap->emplace(defaultKeyValue, it);
                TtoVaultRecordMap->emplace(defaultKeyValue, it);
            }

            for (VaultRecordSet* set : RecordSetsSet)
            {
                set->RecursiveReadWriteMtx.WriteLock();
                set->AddKey(key, defaultKeyValue, false);
                set->RecursiveReadWriteMtx.WriteUnlock();
            }
        }

        RecursiveReadWriteMtx.WriteUnlock();
        return true;
    }

    template <class T>
    bool Vault::AddKey(const std::string& key, const T& defaultKeyValue)
    {
        return AddKey(key, defaultKeyValue, true);
    }

    template <class T>
    VaultOperationResult Vault::UpdateKey(const std::string& key, const T& defaultKeyValue)
    {
        VaultOperationResult res;
        res.Key = key;
        res.RequestedType = typeid(defaultKeyValue);
        
        RecursiveReadWriteMtx.WriteLock();

        // If the key was not added earlier, then it can not be updated
        std::unordered_map<std::string, std::type_index>::iterator keyTypeIt = KeysTypes.find(key);
        if (keyTypeIt == KeysTypes.end())
        {
            RecursiveReadWriteMtx.WriteUnlock();
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::WrongKey;
            return res;
        }

        // Check it is trying to set coorect type 
        if (res.RequestedType != keyTypeIt->second)
        {
            RecursiveReadWriteMtx.WriteUnlock();
            res.RequestedType = keyTypeIt->second;
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::WrongType;
            return res;
        }

        // Change data in template
        RecordTemplate.SetData(key, defaultKeyValue);

        RecursiveReadWriteMtx.WriteUnlock();

        return res;
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
    VaultOperationResult Vault::RequestEqual(const std::string& key, const T& keyValue, VaultRecordSet& vaultRecordSet, const std::size_t& amountOfRecords) const
    {
        return RequestRecords(VaultRequestType::Equal, key, keyValue, keyValue, vaultRecordSet, false, false, amountOfRecords);
    }

    template <class T>
    VaultOperationResult Vault::RequestGreater(const std::string& key, const T& keyValue, VaultRecordSet& vaultRecordSet, const std::size_t& amountOfRecords) const
    {
        return RequestRecords(VaultRequestType::Greater, key, keyValue, keyValue, vaultRecordSet, false, false, amountOfRecords);
    }

    template <class T>
    VaultOperationResult Vault::RequestGreaterOrEqual(const std::string& key, const T& keyValue, VaultRecordSet& vaultRecordSet, const std::size_t& amountOfRecords) const
    {
        return RequestRecords(VaultRequestType::GreaterOrEqual, key, keyValue, keyValue, vaultRecordSet, false, false, amountOfRecords);
    }

    template <class T>
    VaultOperationResult Vault::RequestLess(const std::string& key, const T& keyValue, VaultRecordSet& vaultRecordSet, const std::size_t& amountOfRecords) const
    {
        return RequestRecords(VaultRequestType::Less, key, keyValue, keyValue, vaultRecordSet, false, false, amountOfRecords);
    }

    template <class T>
    VaultOperationResult Vault::RequestLessOrEqual(const std::string& key, const T& keyValue, VaultRecordSet& vaultRecordSet, const std::size_t& amountOfRecords) const
    {
        return RequestRecords(VaultRequestType::LessOrEqual, key, keyValue, keyValue, vaultRecordSet, false, false, amountOfRecords);
    }

    template <class T>
    VaultOperationResult Vault::RequestInterval(const std::string& key, const T& beginKeyValue,
        const T& endKeyValue, VaultRecordSet& vaultRecordSet, const bool& isIncludeBeginKeyValue, 
        const bool& isIncludeEndKeyValue, const std::size_t& amountOfRecords) const
    {
        return RequestRecords(VaultRequestType::Interval, key, beginKeyValue, 
            endKeyValue, vaultRecordSet, isIncludeBeginKeyValue, isIncludeEndKeyValue, 
            amountOfRecords);
    }

    template <VaultRequestType Type>
    void Vault::Request(const VaultRequest<Type>& request, VaultRecordSet& vaultRecordSet) const
    {
        request.Request(const_cast<Vault*>(this), vaultRecordSet);
    }

    template <class T>
    VaultOperationResult Vault::EraseRecord(const std::string& key, const T& keyValue)
    {
        return RemoveRecord(key, keyValue, true);
    }

    template <class T>
    VaultOperationResult Vault::EraseRecords(const std::string& key, const T& keyValue, const std::size_t& amountOfRecords)
    {
        return RemoveRecords(key, keyValue, amountOfRecords, true);
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
                    
                    if (!func(vaultRecordRef)) return false;

                    ++counter;
                    return true;
                }, isReverse);
        }
        RecursiveReadWriteMtx.ReadUnlock();
    }
}