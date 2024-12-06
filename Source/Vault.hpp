#pragma once

#include "Vault.h"
#include "VaultRecordSet.h"

#include "VaultRecord.hpp"
#include "VaultParamInput.hpp"

// This file contains an implementation of the Vault template methods
namespace mvlt
{
    template <class T>
    VaultOperationResult Vault::SetDataToRecord(VaultRecord* dataRecord, const std::string& key, const T& data) noexcept
    {
        // Fill res info known at start
        VaultOperationResult res;
        res.Key = key;
        res.RequestedType = typeid(T);

        // A pointer for storing a hash table with T type key and vault record as value
        UnorderedMap<T, VaultRecord*>* TtoVaultRecordHashMap = nullptr;

        // A pointer for storing a binary tree with T type key and vault record as value
        Map<T, VaultRecord*>* TtoVaultRecordMap = nullptr;

        // Lock Vault to write
        WriteLock<RecursiveReadWriteMutex> writeLock(RecursiveReadWriteMtx);

        // Check if dataRecord valid
        if (dataRecord == nullptr || !dataRecord->GetIsValid())
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::DataRecordNotValid;
            return res;
        }

        // If key not exist
        if(!GetKeyType(key, res.SavedType))
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::WrongKey;
            return res;
        }

        // Check types
        if (res.SavedType != res.RequestedType)
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::WrongType;
            return res;
        }


        // Get std::unordered_multimap with T key and VaultRecord* value
        VaultHashMapStructure.GetData(key, TtoVaultRecordHashMap);

        // Get the current value of the key key inside the VaultRecordRef and save it for further work
        T oldData;
        dataRecord->GetData(key, oldData);

        // Remove oldData from TtoVaultRecordHashMap from VaultHashMapStructure
        auto FirstAndLastIteratorsWithKeyOnHashMap = TtoVaultRecordHashMap->EqualRange(oldData);

        // Iterate over all data records with oldData key
        for (auto& pairIt = FirstAndLastIteratorsWithKeyOnHashMap.first; pairIt != FirstAndLastIteratorsWithKeyOnHashMap.second; ++pairIt)
        {
            // Find required data record
            if ((*pairIt).second == dataRecord)
            {
                TtoVaultRecordHashMap->Erase((*pairIt).first);
                break;
            }
        }

        // Add new data to TtoVaultRecordHashMap to Vault VaultHashMapStructure
        TtoVaultRecordHashMap->Emplace(data, dataRecord);


        // Get std::multimap with T key and VaultRecord* value
        VaultMapStructure.GetData(key, TtoVaultRecordMap);

        // Remove oldData from TtoVaultRecordMap from VaultMapStructure
        auto FirstAndLastIteratorsWithKeyOnMap = TtoVaultRecordMap->EqualRange(oldData);

        // Iterate over all data records with oldData key
        for (auto& pairIt = FirstAndLastIteratorsWithKeyOnMap.first; pairIt != FirstAndLastIteratorsWithKeyOnMap.second; ++pairIt)
        {
            // Find required data record
            if ((*pairIt).second == dataRecord)
            {
                TtoVaultRecordMap->Erase((*pairIt).first);
                break;
            }
        }

        // Add new data to TtoVaultRecordMap to Vault VaultMapStructure
        TtoVaultRecordMap->Emplace(data, dataRecord);


        // Check if this method was original called. That mean that this method called not from next if statement.
        // It is required since VaultRecordSet child of Vault and may also call this method
        if (VaultDerivedClass == VaultDerivedClasses::VaultBase)
        {
            // Update all dependent VaultRecordSets
            dataRecord->UpdateDependentSets(key, data);

            // Update data inside VaultRecord pointer inside VaultRecordRef and Vault
            dataRecord->SetData(key, data);
        }

        res.IsOperationSuccess = true;
        res.ResultCode = VaultOperationResultCode::Success;

        return res;
    }

    template <class T>
    VaultOperationResult Vault::RequestRecordsSet(const VaultRequestType& requestType, const std::string& key, const T& beginKeyValue,
        const T& endKeyValue, std::unordered_set<VaultRecord*>& vaultRecords, const bool& isIncludeBeginKeyValue, 
        const bool& isIncludeEndKeyValue, const std::size_t& amountOfRecords, const std::function<bool(const VaultRecordRef&)>& requestPredicat) const noexcept
    {
        // Fill res info known at start
        VaultOperationResult res;
        res.Key = key;
        res.RequestedType = typeid(T);
        
        // Lock Vault to read
        ReadLock<RecursiveReadWriteMutex> readLock(RecursiveReadWriteMtx);

        // If key not exist
        if(!GetKeyType(key, res.SavedType))
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::WrongKey;
            return res;
        }

        // Check types
        if (res.SavedType != res.RequestedType)
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::WrongType;
            return res;
        }
        
        // Check if request type is equal
        if (requestType == VaultRequestType::Equal)
        {
            // Pointer to store hash map inside VaultStructureHashMap
            UnorderedMap<T, VaultRecord*>* TtoVaultRecordHashMap = nullptr;

            // Get hash map
            VaultHashMapStructure.GetData(key, TtoVaultRecordHashMap);
            
            // Pair with begin and end iterator with T type and beginKeyValue value
            auto equalRange = TtoVaultRecordHashMap->EqualRange(beginKeyValue);
            if (equalRange.first != TtoVaultRecordHashMap->End())
            {
                std::size_t counter = 0;
                for (auto equalRangeIt = equalRange.first; equalRangeIt != equalRange.second; ++equalRangeIt)
                {
                    // Check added records count
                    if (counter >= amountOfRecords) break;

                    // Check if it is not default predicate
                    if (&requestPredicat != &DefaultRequestPredicat)
                    {
                        if (requestPredicat(VaultRecordRef((*equalRangeIt).second, const_cast<Vault*>(this))))
                            vaultRecords.emplace((*equalRangeIt).second);
                    }
                    else
                        vaultRecords.emplace((*equalRangeIt).second);
                    
                    ++counter;
                }
            }
        }
        else 
        {
            // Pointer to store map inside VaultStructureHashMap
            Map<T, VaultRecord*>* TtoVaultRecordMap = nullptr;

            // Get map
            VaultMapStructure.GetData(key, TtoVaultRecordMap);
            
            // Iterator to set it in switch
            decltype(TtoVaultRecordMap->End()) startIt, endIt;

            // Lambda function for finding greater value iterator
            auto findGreater = [&]()
                {
                    bool flag = false;
                    startIt = TtoVaultRecordMap->LowerBound(beginKeyValue);

                    // If it is not beginKeyValue in map and it is value bigger than beginKeyValue in map
                    if (startIt != TtoVaultRecordMap->End() && (*startIt).first > beginKeyValue)
                        return;
                        
                    // Iterate to the next value
                    for (auto TtoVaultRecordMapIt = startIt; TtoVaultRecordMapIt != TtoVaultRecordMap->End(); ++TtoVaultRecordMapIt)
                    {
                        if ((*TtoVaultRecordMapIt).first != (*startIt).first) 
                        {
                            startIt = TtoVaultRecordMapIt;
                            flag = true;
                            break;
                        }
                    }
                
                    // Check if it is any record with key value greater than beginKeyValue 
                    if (!flag) startIt = TtoVaultRecordMap->End();
                };

            // Lambda function for finding less value iterator
            auto findLess = [&]()
                {
                    endIt = TtoVaultRecordMap->UpperBound(endKeyValue);

                    // Check if TtoVaultRecordMap size not zero and endIt not last iterator
                    if (endIt != TtoVaultRecordMap->Begin() && TtoVaultRecordMap->Size() > 0)
                    {
                        // Upper bound return next iterator with value greater then endKeyValue
                        --endIt;

                        // Iterate to previous value
                        while(endIt != (*TtoVaultRecordMap).Begin() && (*endIt).first == endKeyValue)
                            --endIt;
                        
                        // Increase iterator to add later last element in vaultRecords
                        if (endIt != TtoVaultRecordMap->Begin())
                            ++endIt;
                    }
                };

            // Switch by request type
            switch (requestType) 
            {
            case VaultRequestType::GreaterOrEqual:
                startIt = TtoVaultRecordMap->LowerBound(beginKeyValue);
                endIt = TtoVaultRecordMap->End();
                break;
            
            case VaultRequestType::Greater:
                findGreater();
                endIt = TtoVaultRecordMap->End();
                break;
            
            case VaultRequestType::Less:
                startIt = TtoVaultRecordMap->Begin();
                findLess();
                break;
            
            case VaultRequestType::LessOrEqual:
                startIt = TtoVaultRecordMap->Begin();
                endIt = TtoVaultRecordMap->UpperBound(beginKeyValue);
                break;
            
            case VaultRequestType::Interval:
                if (beginKeyValue > endKeyValue || (beginKeyValue == endKeyValue && (isIncludeBeginKeyValue == false || isIncludeEndKeyValue == false)))
                {
                    startIt = TtoVaultRecordMap->End();
                    endIt = TtoVaultRecordMap->End();
                    break;
                }

                if(isIncludeBeginKeyValue) startIt = TtoVaultRecordMap->LowerBound(beginKeyValue);
                else findGreater();
                if(isIncludeEndKeyValue) endIt = TtoVaultRecordMap->UpperBound(endKeyValue);
                else findLess();
                break;

            case VaultRequestType::Equal:
            case VaultRequestType::Or:
            case VaultRequestType::And:
                break;
            }

            std::size_t counter = 0;
            for (auto TtoVaultRecordMapIt = startIt; TtoVaultRecordMapIt != endIt; ++TtoVaultRecordMapIt)
            {
                if (counter >= amountOfRecords) break;

                // Check if it is not default predicate
                if (&requestPredicat != &DefaultRequestPredicat)
                {
                    if (requestPredicat(VaultRecordRef((*TtoVaultRecordMapIt).second, const_cast<Vault*>(this))))
                        vaultRecords.emplace((*TtoVaultRecordMapIt).second);
                }
                else
                    vaultRecords.emplace((*TtoVaultRecordMapIt).second);

                ++counter;
            }
        }

        res.ResultCode = VaultOperationResultCode::Success;
        res.SavedType = res.RequestedType;
        res.IsOperationSuccess = true;

        return res;
    }

    template <class T>
    VaultOperationResult Vault::RequestRecords(const VaultRequestType& requestType, const std::string& key, const T& beginKeyValue,
        const T& endKeyValue, VaultRecordSet& vaultRecordSet, const bool& isIncludeBeginKeyValue, 
        const bool& isIncludeEndKeyValue, const std::size_t& amountOfRecords, const std::function<bool(const VaultRecordRef&)>& requestPredicat) const noexcept
    {
        VaultOperationResult res;

        // Lock Vault to read
        ReadLock<RecursiveReadWriteMutex> readLock(RecursiveReadWriteMtx);

        // Save vaultRecordSet
        if (VaultDerivedClass == VaultDerivedClasses::VaultBase)
            RecordSetsSet.emplace(&vaultRecordSet);

        // Remove old data from vaultRecordSet
        if (vaultRecordSet.ParentVault != this) vaultRecordSet.Reset();
        else vaultRecordSet.Clear();

        // Set new parent vault to vaultRecordSet
        vaultRecordSet.ParentVault = const_cast<Vault*>(this);

        // Copy keys from this to vaultRecordSet
        for (auto& keyCopierIt : VaultKeyCopiers)
            keyCopierIt.second(vaultRecordSet);
        
        // Set proper key order
        vaultRecordSet.KeysOrder = KeysOrder;

        // Set unique keys
        vaultRecordSet.UniqueKeys = UniqueKeys;

        res = RequestRecordsSet(requestType, key, beginKeyValue, endKeyValue, vaultRecordSet.RecordsSet, isIncludeBeginKeyValue, isIncludeEndKeyValue, amountOfRecords, requestPredicat);

        for (VaultRecord* record : vaultRecordSet.RecordsSet)
        {
            // Add pointer to record from this to vaultRecordSet structure
            for (auto& adder : vaultRecordSet.VaultRecordAdders)
                adder.second(record);
            
            record->AddToDependentSets(&vaultRecordSet);
        }

        return res;
    }

    template <class T>
    VaultOperationResult Vault::AddKey(const std::string& key, const T& defaultKeyValue, const bool& isUniqueKey, const bool& isUniqueKeyWithoutLambda,
        std::function<T(std::size_t, const VaultRecordRef&)> uniqueKeyFunction) noexcept
    {
        static_assert(!std::is_array<T>::value, "It is not possible to use a c array as a key value. \n\
            If you want to use a string as a key, you must specialize the function with a string. Like this: \n\
            AddKey<std::string>(\"Key\", \"Value\") or AddKey(\"Key\", std::string(\"Value\"))");

        VaultOperationResult res;
        res.Key = key;
        res.RequestedType = typeid(T);

        // Lock Vault to write
        WriteLock<RecursiveReadWriteMutex> writeLock(RecursiveReadWriteMtx);

        // If the key was added earlier
        if (KeysTypes.find(key) != KeysTypes.end())
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::DuplicateKey;
            return res;
        }

        // Create new hash map to store data with template T key
        UnorderedMap<T, VaultRecord*>* TtoVaultRecordHashMap = new UnorderedMap<T, VaultRecord*>(!isUniqueKey);
        VaultHashMapStructure.SetData(key, TtoVaultRecordHashMap, [](const void* ptr)
            {
                delete* (UnorderedMap<T, VaultRecord*>**)ptr;
            }
        );

        // Create new map to store data with template T key
        Map<T, VaultRecord*>* TtoVaultRecordMap = new Map<T, VaultRecord*>(!isUniqueKey);
        VaultMapStructure.SetData(key, TtoVaultRecordMap, [](const void* ptr)
            {
                delete* (Map<T, VaultRecord*>**)ptr;
            }
        );

        // Add new data to record set
        if (isUniqueKey)
        {
            // If no lamda provided and vault not empty
            if (isUniqueKeyWithoutLambda && !RecordsSet.empty())
            {
                res.IsOperationSuccess = false;
                res.ResultCode = VaultOperationResultCode::TryToAddUniqueKeyInNonEmptyVaultWithoutLambda;
                return res;
            }

            std::size_t counter = 0;
            std::vector<T> cachedData;
            bool isCorrectKey = true;

            // Cycle for check uniqueKeyFunction validity
            for (VaultRecord* recordsSetIt : RecordsSet)
            {
                T value = uniqueKeyFunction(counter, VaultRecordRef(recordsSetIt, this));
                ++counter;

                auto emplacePair = TtoVaultRecordHashMap->Emplace(value, recordsSetIt);
                // If failed to emplace unique key then it is duplicate and error in uniqueKeyFunction
                if (!emplacePair.second)
                {
                    isCorrectKey = false;
                    break;
                }

                cachedData.emplace_back(std::move(value));
            }

            // If key correct then add data to all places
            if (isCorrectKey)
            {
                counter = 0;
                for (VaultRecord* recordsSetIt : RecordsSet)
                {
                    TtoVaultRecordMap->Emplace(cachedData[counter], recordsSetIt);
                    if (VaultDerivedClass == VaultDerivedClasses::VaultBase)
                        recordsSetIt->SetData(key, cachedData[counter]);

                    ++counter;
                }
            }
            else
            {
                VaultHashMapStructure.EraseData(key);
                VaultMapStructure.EraseData(key);

                res.IsOperationSuccess = false;
                res.ResultCode = VaultOperationResultCode::UniqueKeyValueAlredyInSet;
                return res;
            }
        }
        else
        {
            for (VaultRecord* recordsSetIt : RecordsSet)
            {
                recordsSetIt->SetData(key, defaultKeyValue);
                TtoVaultRecordHashMap->Emplace(defaultKeyValue, recordsSetIt);
                TtoVaultRecordMap->Emplace(defaultKeyValue, recordsSetIt);
            }
        }

        // Add key to list with key order
        if(VaultDerivedClass == VaultDerivedClasses::VaultBase)
            KeysOrder.emplace_back(key);

        // Add key type to hash map with keys types
        KeysTypes.emplace(key, typeid(T)); 

        // Add data to template
        RecordTemplate.SetData(key, defaultKeyValue);

        // Add unique key
        if (isUniqueKey) UniqueKeys.emplace(key);

        // Add function to VaultRecord creation
        VaultRecordAdders.emplace(key, [=](VaultRecord* newRecord)
            {
                // Make temporary variable to store key value
                T value = defaultKeyValue;
                // Try to get key value from new record. If it is not value inside then defaultKeyValue will be used
                newRecord->GetData(key, value);

                // Try to emplace data into hash table. If it is multi container then it is always emplaced
                // If it is not multi container then data emplaced only if it was not same data alredy in hash table
                auto emplaceRes = TtoVaultRecordHashMap->Emplace(value, newRecord);

                // Return false if data alredy in hash table
                if (!emplaceRes.second) return false;

                TtoVaultRecordMap->Emplace(value, newRecord);
                return true;
            }
        );

        // Add function to TtoVaultRecordHashMap cleareing
        VaultRecordClearers.emplace(key, [=]()
            {
                TtoVaultRecordHashMap->Clear();
                TtoVaultRecordMap->Clear();
            }
        );

        // Add function to erase newRecord from TtoVaultRecordHashMap
        VaultRecordErasers.emplace(key, [=](VaultRecord* newRecord)
            {
                // Get T type data with key key
                T recordTData;
                newRecord->GetData(key, recordTData);

                // Find all elements on multi_map with recordTData value
                auto FirstAndLastIteratorsWithKeyOnHashMap = TtoVaultRecordHashMap->EqualRange(recordTData);

                // Find newRecord and erase it from TtoVaultRecordHashMap
                for (auto pairIt = FirstAndLastIteratorsWithKeyOnHashMap.first; pairIt != FirstAndLastIteratorsWithKeyOnHashMap.second; ++pairIt)
                {
                    if ((*pairIt).second == newRecord)
                    {
                        TtoVaultRecordHashMap->Erase(pairIt);
                        break;
                    }
                }

                // Find all elements on map with recordTData value
                auto FirstAndLastIteratorsWithKeyOnMap = TtoVaultRecordMap->EqualRange(recordTData);
                // Find newRecord and erase it from TtoVaultRecordHashMap
                for (auto pairIt = FirstAndLastIteratorsWithKeyOnMap.first; pairIt != FirstAndLastIteratorsWithKeyOnMap.second; ++pairIt)
                {
                    if ((*pairIt).second == newRecord)
                    {
                        TtoVaultRecordMap->Erase((*pairIt).first);
                        break;
                    }
                }
            }
        );

        VaultRecordSorters.emplace(key, [=](std::function<bool(const VaultRecordRef&)> functionToSortedData, bool isReverse)
        {
            if (!isReverse)
            {
                for (const auto& TtoVaultRecordMapIt : *TtoVaultRecordMap)
                    if(!functionToSortedData(VaultRecordRef(TtoVaultRecordMapIt.second, this)))
                        break;
            }
            else
            {
                for (auto TtoVaultRecordMapIt = TtoVaultRecordMap->Rbegin(); TtoVaultRecordMapIt != TtoVaultRecordMap->Rend(); ++TtoVaultRecordMapIt)
                    if(!functionToSortedData(VaultRecordRef((*TtoVaultRecordMapIt).second, this)))
                        break;
            }
        });
        
        VaultKeyCopiers.emplace(key, [=](VaultRecordSet& vaultRecordSet)
        {
            vaultRecordSet.AddKey(key, defaultKeyValue);
        });

        if (VaultDerivedClass == VaultDerivedClasses::VaultBase)
        {
            for (VaultRecordSet* set : RecordSetsSet)
            {
                if (isUniqueKey)
                    set->AddUniqueKey(key, uniqueKeyFunction);
                else
                    set->AddKey(key, defaultKeyValue);
                set->KeysOrder.emplace_back(key);
            }
        }

        res.IsOperationSuccess = true;
        res.ResultCode = VaultOperationResultCode::Success;
        return res;
    }

    template <class T>
    bool Vault::AddKey(const std::string& key, const T& defaultKeyValue) noexcept
    {
        return AddKey(key, defaultKeyValue, false, false, {[&](std::size_t counter, const VaultRecordRef&) -> T{ return defaultKeyValue; }}).IsOperationSuccess;
    }

    template <class T>
    VaultOperationResult Vault::AddUniqueKey(const std::string& key) noexcept
    {
        return AddKey(key, T(), true, true, {[&](std::size_t counter, const VaultRecordRef&) -> T{ return T(); }});
    }

    template <class T>
    VaultOperationResult Vault::AddUniqueKey(const std::string& key, std::function<T(std::size_t, const VaultRecordRef&)> uniqueKeyFunction) noexcept
    {
        return AddKey(key, T(), true, false, uniqueKeyFunction);
    }

    template <class T>
    VaultOperationResult Vault::UpdateKey(const std::string& key, const T& defaultKeyValue) noexcept
    {
        VaultOperationResult res;
        res.Key = key;
        res.RequestedType = typeid(defaultKeyValue);

        // Lock Vault to write
        WriteLock<RecursiveReadWriteMutex> writeLock(RecursiveReadWriteMtx);

        // If the key was not added earlier, then it can not be updated
        std::unordered_map<std::string, std::type_index>::iterator keyTypeIt = KeysTypes.find(key);
        if (keyTypeIt == KeysTypes.end())
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::WrongKey;
            return res;
        }

        // Check it is trying to set coorect type 
        if (res.RequestedType != keyTypeIt->second)
        {
            res.SavedType = keyTypeIt->second;
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::WrongType;
            return res;
        }

        // Check if it is unique key
        if (UniqueKeys.find(key) != UniqueKeys.end())
        {
            res.SavedType = keyTypeIt->second;
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::TryToUpdateUniqueKey;
            return res;
        }

        // Change data in template
        RecordTemplate.SetData(key, defaultKeyValue);

        return res;
    }

    template <class T>
    VaultOperationResult Vault::GetKeyValue(const std::string& key, T& defaultKeyValue) const noexcept
    {
        VaultOperationResult res;
        res.Key = key;
        res.RequestedType = typeid(T);

        // Lock Vault to read
        ReadLock<RecursiveReadWriteMutex> readLock(RecursiveReadWriteMtx);

        // If key not exist
        if(!GetKeyType(key, res.SavedType))
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::WrongKey;
            return res;
        }

        // Check types
        if (res.SavedType != res.RequestedType)
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::WrongType;
            return res;
        }

        for (auto& it : UniqueKeys)
        {
            std::cout << it << std::endl;
        }

        // Check if it is unique key
        if (UniqueKeys.find(key) != UniqueKeys.end())
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::TryToUpdateUniqueKey;
            return res;
        }

        RecordTemplate.GetData(key, defaultKeyValue);
        
        res.IsOperationSuccess = true;
        res.ResultCode = VaultOperationResultCode::Success;
        res.SavedType = res.RequestedType;

        return res;
    }

    template <class T>
    VaultOperationResult Vault::GetRecord(const std::string& key, const T& keyValue, VaultRecordRef& vaultRecordRef) const noexcept
    {
        static_assert(!std::is_array<T>::value, "It is not possible to use a c array as a key value. \n\
            If you want to use a string as a key, you must specialize the function with a string. Like this: \n\
            AddKey<std::string>(\"Key\", \"Value\") or AddKey(\"Key\", std::string(\"Value\"))");

        // Fill res info known at start
        VaultOperationResult res;
        res.Key = key;
        res.RequestedType = typeid(T);

        // Lock Vault to read
        ReadLock<RecursiveReadWriteMutex> readLock(RecursiveReadWriteMtx);

        // If key not exist
        if(!GetKeyType(key, res.SavedType))
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::WrongKey;
            return res;
        }

        // Check types
        if (res.SavedType != res.RequestedType)
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::WrongType;
            return res;
        }

        // Pointer to store map inside VaultStructureHashMap
        UnorderedMap<T, VaultRecord*>* TtoVaultRecordHashMap = nullptr;

        // Checking whether such a key exists
        VaultHashMapStructure.GetData(key, TtoVaultRecordHashMap);
        
        // Iterator to element with T type and keyValue value
        auto TtoVaultRecordIt = TtoVaultRecordHashMap->Find(keyValue);
        if (TtoVaultRecordIt != TtoVaultRecordHashMap->End())
        {
            vaultRecordRef.SetRecord((*TtoVaultRecordIt).second, const_cast<Vault*>(this));
            res.IsOperationSuccess = true;
            res.ResultCode = VaultOperationResultCode::Success;
        }
        else
        {
            vaultRecordRef.SetRecord(nullptr, const_cast<Vault*>(this));
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::WrongValue;
        }

        return res;
    }

    template <class T>
    VaultOperationResult Vault::GetRecords(const std::string& key, const T& keyValue, std::vector<VaultRecordRef>& recordsRefs, const std::size_t& amountOfRecords) const noexcept
    {
        static_assert(!std::is_array<T>::value, "It is not possible to use a c array as a key value. \n\
            If you want to use a string as a key, you must specialize the function with a string. Like this: \n\
            AddKey<std::string>(\"Key\", \"Value\") or AddKey(\"Key\", std::string(\"Value\"))");

        // Fill res info known at start
        VaultOperationResult res;
        res.Key = key;
        res.RequestedType = typeid(T);

        recordsRefs.clear();

        // Lock Vault to read
        ReadLock<RecursiveReadWriteMutex> readLock(RecursiveReadWriteMtx);

        // If key not exist
        if(!GetKeyType(key, res.SavedType))
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::WrongKey;
            return res;
        }

        // Check types
        if (res.SavedType != res.RequestedType)
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::WrongType;
            return res;
        }

        // Pointer to store map inside VaultStructureHashMap
        UnorderedMap<T, VaultRecord*>* TtoVaultRecordHashMap = nullptr;

        // Checking whether such a key exists
        VaultHashMapStructure.GetData(key, TtoVaultRecordHashMap);
        
        // Pair with begin and end iterator with T type and keyValue value
        auto equalRange = TtoVaultRecordHashMap->EqualRange(keyValue);
        if (equalRange.first != TtoVaultRecordHashMap->end())
        {
            std::size_t counter = 0;
            for (auto equalRangeIt = equalRange.first; equalRangeIt != equalRange.second; ++equalRangeIt)
            {
                ++counter;
                recordsRefs.emplace_back(VaultRecordRef((*equalRangeIt).second, const_cast<Vault*>(this)));
                if (counter >= amountOfRecords) break;
            }
        }
        else
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::WrongValue;
        }

        return res;
    }

    template <class T>
    VaultOperationResult Vault::RequestEqual(const std::string& key, const T& keyValue, VaultRecordSet& vaultRecordSet,
        const std::size_t& amountOfRecords, const std::function<bool(const VaultRecordRef&)>& requestPredicat) const noexcept
    {
        return RequestRecords(VaultRequestType::Equal, key, keyValue, keyValue, vaultRecordSet, false, false, amountOfRecords, requestPredicat);
    }

    template <class T>
    VaultOperationResult Vault::RequestGreater(const std::string& key, const T& keyValue, VaultRecordSet& vaultRecordSet,
        const std::size_t& amountOfRecords, const std::function<bool(const VaultRecordRef&)>& requestPredicat) const noexcept
    {
        return RequestRecords(VaultRequestType::Greater, key, keyValue, keyValue, vaultRecordSet, false, false, amountOfRecords, requestPredicat);
    }

    template <class T>
    VaultOperationResult Vault::RequestGreaterOrEqual(const std::string& key, const T& keyValue, VaultRecordSet& vaultRecordSet,
        const std::size_t& amountOfRecords, const std::function<bool(const VaultRecordRef&)>& requestPredicat) const noexcept
    {
        return RequestRecords(VaultRequestType::GreaterOrEqual, key, keyValue, keyValue, vaultRecordSet, false, false, amountOfRecords, requestPredicat);
    }

    template <class T>
    VaultOperationResult Vault::RequestLess(const std::string& key, const T& keyValue, VaultRecordSet& vaultRecordSet,
        const std::size_t& amountOfRecords, const std::function<bool(const VaultRecordRef&)>& requestPredicat) const noexcept
    {
        return RequestRecords(VaultRequestType::Less, key, keyValue, keyValue, vaultRecordSet, false, false, amountOfRecords, requestPredicat);
    }

    template <class T>
    VaultOperationResult Vault::RequestLessOrEqual(const std::string& key, const T& keyValue, VaultRecordSet& vaultRecordSet,
        const std::size_t& amountOfRecords, const std::function<bool(const VaultRecordRef&)>& requestPredicat) const noexcept
    {
        return RequestRecords(VaultRequestType::LessOrEqual, key, keyValue, keyValue, vaultRecordSet, false, false, amountOfRecords, requestPredicat);
    }

    template <class T>
    VaultOperationResult Vault::RequestInterval(const std::string& key, const T& beginKeyValue,
        const T& endKeyValue, VaultRecordSet& vaultRecordSet, const bool& isIncludeBeginKeyValue, 
        const bool& isIncludeEndKeyValue, const std::size_t& amountOfRecords, const std::function<bool(const VaultRecordRef&)>& requestPredicat) const noexcept
    {
        return RequestRecords(VaultRequestType::Interval, key, beginKeyValue, 
            endKeyValue, vaultRecordSet, isIncludeBeginKeyValue, isIncludeEndKeyValue, 
            amountOfRecords, requestPredicat);
    }

    template <VaultRequestType Type>
    VaultOperationResult Vault::Request(const VaultRequest<Type>&& request, VaultRecordSet& vaultRecordSet) const
    {
        VaultOperationResult res;
        res.IsOperationSuccess = true;
        res.ResultCode = VaultOperationResultCode::Success;

        // Lock Vault to read
        ReadLock<RecursiveReadWriteMutex> readLock(RecursiveReadWriteMtx);

        // Save vaultRecordSet
        if (VaultDerivedClass == VaultDerivedClasses::VaultBase)
            RecordSetsSet.emplace(&vaultRecordSet);

        // Remove old data from vaultRecordSet
        if (vaultRecordSet.ParentVault != this) vaultRecordSet.Reset();
        else vaultRecordSet.Clear();

        // Set new parent vault to vaultRecordSet
        vaultRecordSet.ParentVault = const_cast<Vault*>(this);
        
        // Set key proper key order
        vaultRecordSet.KeysOrder = KeysOrder;

        // Set unique keys
        vaultRecordSet.UniqueKeys = UniqueKeys;

        // Copy keys from this to vaultRecordSet
        for (auto& keyCopierIt : VaultKeyCopiers)
            keyCopierIt.second(vaultRecordSet);
        
        // Try to make complex request 
        try
        {
            request.Request(const_cast<Vault*>(this), vaultRecordSet.RecordsSet);

            for (VaultRecord* record : vaultRecordSet.RecordsSet)
            {
                // Add pointer to record from this to vaultRecordSet structure
                for (auto& adder : vaultRecordSet.VaultRecordAdders)
                    adder.second(record);

                // Lock VaultRecord to thread safety add new dependent VaultRecordSet
                record->AddToDependentSets(&vaultRecordSet);
            }
        }
        catch(VaultOperationResult result) // Catch complex request errors
        {
            vaultRecordSet.Clear();
            res = result;
        }

        return res;
    }

    template <class T>
    VaultOperationResult Vault::EraseRecord(const std::string& key, const T& keyValue) noexcept
    {
        // Fill res info known at start
        VaultOperationResult res;
        res.Key = key;
        res.RequestedType = typeid(T);

        // Lock Vault to write
        WriteLock<RecursiveReadWriteMutex> writeLock(RecursiveReadWriteMtx);

        // If key not exist
        if(!GetKeyType(key, res.SavedType))
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::WrongKey;
            return res;
        }

        // Check types
        if (res.SavedType != res.RequestedType)
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::WrongType;
            return res;
        }

        // Pointer to store map inside VaultStructureHashMap
        UnorderedMap<T, VaultRecord*>* TtoVaultRecordHashMap = nullptr;

        // Checking whether such a key exists
        VaultHashMapStructure.GetData(key, TtoVaultRecordHashMap);
        
        // Iterator to element with T type and keyValue value
        auto TtoVaultRecordIt = TtoVaultRecordHashMap->Find(keyValue);
        if (TtoVaultRecordIt != TtoVaultRecordHashMap->end())
        {
            VaultRecord* tmpRec = (*TtoVaultRecordIt).second;

            for (auto& eraser : VaultRecordErasers)
                eraser.second(tmpRec);

            RecordsSet.erase(tmpRec);
            
            // Check if it is delete. By now it is deletion when EraseRecord called from vault.
            // It is not deleting when RemoveRecord called from VaultRecordSet
            if (VaultDerivedClass == VaultDerivedClasses::VaultBase)
            {
                tmpRec->RemoveFromDependentSets();
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

        return res;
    }

    template <class T>
    VaultOperationResult Vault::EraseRecords(const std::string& key, const T& keyValue, const std::size_t& amountOfRecords) noexcept
    {
        // Fill res info known at start
        VaultOperationResult res;
        res.Key = key;
        res.RequestedType = typeid(T);

        // Lock Vault to write
        WriteLock<RecursiveReadWriteMutex> writeLock(RecursiveReadWriteMtx);

        // If key not exist
        if(!GetKeyType(key, res.SavedType))
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::WrongKey;
            return res;
        }

        // Check types
        if (res.SavedType != res.RequestedType)
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::WrongType;
            return res;
        }

        // Pointer to store map inside VaultStructureHashMap
        UnorderedMap<T, VaultRecord*>* TtoVaultRecordHashMap = nullptr;

        // Checking whether such a key exists
        VaultHashMapStructure.GetData(key, TtoVaultRecordHashMap);
        
        // Pair with begin and end iterator with T type and keyValue value
        auto equalRange = TtoVaultRecordHashMap->EqualRange(keyValue);
        if (equalRange.first != TtoVaultRecordHashMap->end())
        {
            res.IsOperationSuccess = true;
            res.ResultCode = VaultOperationResultCode::Success;

            std::size_t counter = 0;
            for (auto equalRangeIt = equalRange.first; equalRangeIt != equalRange.second;)
            {
                // Save next iterator
                auto nextIt = equalRangeIt;
                ++nextIt;

                ++counter;

                VaultRecord* tmpRec = (*equalRangeIt).second;
                for (auto& eraser : VaultRecordErasers)
                    eraser.second(tmpRec);

                RecordsSet.erase(tmpRec);

                // Check if it is delete. By now it is deletion when EraseRecord called from vault.
                // It is not deleting when RemoveRecord called from VaultRecordSet
                if (VaultDerivedClass == VaultDerivedClasses::VaultBase)
                {
                    tmpRec->RemoveFromDependentSets();
                    tmpRec->Invalidate();
                }
                
                if (counter >= amountOfRecords) break;
                
                equalRangeIt = nextIt;
            }
        }
        else
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::WrongValue;
        }

        return res;
    }

    template<class F>
    void Vault::SortBy(const std::string& key, const F&& func, const bool& isReverse, const std::size_t& amountOfRecords) const noexcept
    {
        std::size_t counter = 0;

        // Lock Vault to read
        ReadLock<RecursiveReadWriteMutex> readLock(RecursiveReadWriteMtx);
        
        auto findResIt = VaultRecordSorters.find(key);
        if (findResIt != VaultRecordSorters.end())
        {
            VaultRecordSorters.find(key)->second([&](const VaultRecordRef& vaultRecordRef)
                {
                    if (counter >= amountOfRecords) return false;
                    
                    if (!func(vaultRecordRef)) return false;

                    ++counter;
                    return true;
                }, isReverse);
        }
    }
}