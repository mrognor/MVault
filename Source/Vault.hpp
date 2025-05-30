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
        DBG_LOG_ENTER();

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
            res.SetOpResult(VaultOperationResultCode::WrongType);
            return res;
        }


        // Get std::unordered_multimap with T key and VaultRecord* value
        VaultHashMapStructure.GetData(key, TtoVaultRecordHashMap);

        // Get the current value of the key key inside the VaultRecordRef and save it for further work
        T oldData{};
        dataRecord->GetData(key, oldData);

        // Remove oldData from TtoVaultRecordHashMap from VaultHashMapStructure
        auto FirstAndLastIteratorsWithKeyOnHashMap = TtoVaultRecordHashMap->EqualRange(oldData);

        // Check if it is unique key and try to set duplicate data
        if (!TtoVaultRecordHashMap->IsMultiContainer() && FirstAndLastIteratorsWithKeyOnHashMap.first != TtoVaultRecordHashMap->End())
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::UniqueKeyValueAlredyInSet;

            return res;
        }

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
        const bool& isIncludeEndKeyValue, const std::size_t& amountOfRecords, const std::function<bool(const VaultRecordRef& ref)>& requestPredicat) const noexcept
    {
        DBG_LOG_ENTER();

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
            res.SetOpResult(VaultOperationResultCode::WrongType);
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
            decltype(TtoVaultRecordMap->End()) startIt{}, endIt{};

            // Switch by request type
            switch (requestType) 
            {
            case VaultRequestType::GreaterOrEqual:
                startIt = TtoVaultRecordMap->LowerBound(beginKeyValue);
                endIt = TtoVaultRecordMap->End();
                break;
            
            case VaultRequestType::Greater:
                startIt = TtoVaultRecordMap->UpperBound(beginKeyValue);
                endIt = TtoVaultRecordMap->End();
                break;
            
            case VaultRequestType::Less:
                startIt = TtoVaultRecordMap->Begin();
                endIt = TtoVaultRecordMap->LowerBound(endKeyValue);
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
                else startIt = TtoVaultRecordMap->UpperBound(beginKeyValue);
                if(isIncludeEndKeyValue) endIt = TtoVaultRecordMap->UpperBound(endKeyValue);
                else endIt = TtoVaultRecordMap->LowerBound(endKeyValue);
                // Dont forget to put break here if you adding logic to next cases

            case VaultRequestType::Equal:
            case VaultRequestType::Or:
            case VaultRequestType::And:
            default:
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
        const bool& isIncludeEndKeyValue, const std::size_t& amountOfRecords, const std::function<bool(const VaultRecordRef& ref)>& requestPredicat) const noexcept
    {
        DBG_LOG_ENTER();

        VaultOperationResult res;

        // Lock Vault to read
        ReadLock<RecursiveReadWriteMutex> readLock(RecursiveReadWriteMtx);

        // If it is Vault object
        if (VaultDerivedClass == VaultDerivedClasses::VaultBase)
        {
            // Save vaultRecordSet
            RecordSetsSet.emplace(&vaultRecordSet);

            // Remove old data from vaultRecordSet
            if (vaultRecordSet.ParentVault != this)
            {
                vaultRecordSet.Reset();

                // Set new parent vault to vaultRecordSet
                vaultRecordSet.ParentVault = const_cast<Vault*>(this);

                // Copy keys from this to vaultRecordSet
                for (auto& keyCopierIt : VaultKeyCopiers)
                    keyCopierIt.second(&vaultRecordSet);

                // Set proper key order
                vaultRecordSet.KeysOrder = KeysOrder;

                // Set unique keys
                vaultRecordSet.UniqueKeys = UniqueKeys;
            }
            else vaultRecordSet.Clear();
        }

        res = RequestRecordsSet(requestType, key, beginKeyValue, endKeyValue, vaultRecordSet.RecordsSet, isIncludeBeginKeyValue, isIncludeEndKeyValue, amountOfRecords, requestPredicat);

        for (VaultRecord* record : vaultRecordSet.RecordsSet)
        {
            // Add pointer to record to vaultRecordSet structure
            for (auto& adder : vaultRecordSet.VaultRecordAdders)
                adder.second(record);
            
            record->AddToDependentSets(&vaultRecordSet);
        }

        return res;
    }

    template <class T>
    VaultOperationResult Vault::AddKey(const std::string& key, const T& defaultKeyValue, const bool& isUniqueKey, const bool& isUniqueKeyWithoutLambda,
        const std::function<T(const std::size_t& counter, const VaultRecordRef& ref)>& uniqueKeyFunction) noexcept
    {
        DBG_LOG_ENTER();

        VaultOperationResult res;
        res.Key = key;
        res.RequestedType = typeid(T);

        // Lock Vault to write
        WriteLock<RecursiveReadWriteMutex> writeLock(RecursiveReadWriteMtx);

        // If the key was added earlier
        auto keyType = KeysTypes.find(key);
        if (keyType != KeysTypes.end())
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::DuplicateKey;
            res.SavedType = keyType->second;
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

                cachedData.push_back(std::move(value));
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

        // Add function to TtoVaultRecordHashMap clearing
        VaultRecordClearers.emplace(key, [=]()
            {
                TtoVaultRecordHashMap->Clear();
                TtoVaultRecordMap->Clear();
            }
        );

        // Add function to erase record from TtoVaultRecordHashMap
        VaultRecordErasers.emplace(key, [=](VaultRecord* record)
            {
                // Get T type data with key key
                T recordTData{};
                record->GetData(key, recordTData);

                // Find all elements on multi_map with recordTData value
                auto FirstAndLastIteratorsWithKeyOnHashMap = TtoVaultRecordHashMap->EqualRange(recordTData);

                // Find record and erase it from TtoVaultRecordHashMap
                for (auto pairIt = FirstAndLastIteratorsWithKeyOnHashMap.first; pairIt != FirstAndLastIteratorsWithKeyOnHashMap.second; ++pairIt)
                {
                    if ((*pairIt).second == record)
                    {
                        TtoVaultRecordHashMap->Erase(pairIt);
                        break;
                    }
                }

                // Find all elements on map with recordTData value
                auto FirstAndLastIteratorsWithKeyOnMap = TtoVaultRecordMap->EqualRange(recordTData);
                // Find record and erase it from TtoVaultRecordHashMap
                for (auto pairIt = FirstAndLastIteratorsWithKeyOnMap.first; pairIt != FirstAndLastIteratorsWithKeyOnMap.second; ++pairIt)
                {
                    if ((*pairIt).second == record)
                    {
                        TtoVaultRecordMap->Erase((*pairIt).first);
                        break;
                    }
                }
            }
        );

        VaultRecordSorters.emplace(key, [=](const std::function<bool(const VaultRecordRef& ref)>& functionToSortedData, Vault* vltPtr, const bool& isReverse)
        {
            if (!isReverse)
            {
                for (const auto& TtoVaultRecordMapIt : *TtoVaultRecordMap)
                    if(!functionToSortedData(VaultRecordRef(TtoVaultRecordMapIt.second, vltPtr)))
                        break;
            }
            else
            {
                for (auto TtoVaultRecordMapIt = TtoVaultRecordMap->Rbegin(); TtoVaultRecordMapIt != TtoVaultRecordMap->Rend(); ++TtoVaultRecordMapIt)
                    if(!functionToSortedData(VaultRecordRef((*TtoVaultRecordMapIt).second, vltPtr)))
                        break;
            }
        });
        
        VaultKeyCopiers.emplace(key, [=](Vault* vaultRecordSet)
        {
            vaultRecordSet->AddKey(key, defaultKeyValue);
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
        DBG_LOG_ENTER();

        static_assert(!std::is_array<T>::value, "It is not possible to use a c array as a key value. \n\
            If you want to use a string as a key, you must specialize the function with a string. Like this: \n\
            AddKey<std::string>(\"Key\", \"Value\") or AddKey(\"Key\", std::string(\"Value\"))");

        return AddKey(key, defaultKeyValue, false, false, {[&](std::size_t counter, const VaultRecordRef&) -> T{ return defaultKeyValue; }}).IsOperationSuccess;
    }

    template <class T>
    VaultOperationResult Vault::AddUniqueKey(const std::string& key) noexcept
    {
        DBG_LOG_ENTER();

        static_assert(!std::is_array<T>::value, "It is not possible to use a c array as a key value. \n\
            If you want to use a string as a key, you must specialize the function with a string. Like this: \n\
            AddUniqueKey<std::string>(\"Key\")");

        return AddKey(key, T{}, true, true, {[&](std::size_t counter, const VaultRecordRef&) -> T{ return T{}; }});
    }

    template <class T>
    VaultOperationResult Vault::AddUniqueKey(const std::string& key, const std::function<T(const std::size_t& counter, const VaultRecordRef& ref)>& uniqueKeyFunction) noexcept
    {
        DBG_LOG_ENTER();

        return AddKey(key, T{}, true, false, uniqueKeyFunction);
    }

    template <class T>
    VaultOperationResult Vault::UpdateKey(const std::string& key, const T& defaultKeyValue) noexcept
    {
        DBG_LOG_ENTER();

        static_assert(!std::is_array<T>::value, "It is not possible to use a c array as a key value. \n\
            If you want to use a string as a key, you must specialize the function with a string. Like this: \n\
            UpdateKey<std::string>(\"Key\", \"Value\") or UpdateKey(\"Key\", std::string(\"Value\"))");

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
            res.SetOpResult(VaultOperationResultCode::WrongType);
            return res;
        }

        // Check if it is unique key
        if (UniqueKeys.find(key) != UniqueKeys.end())
        {
            res.SavedType = keyTypeIt->second;
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::UniqueKey;
            return res;
        }

        // Change data in template
        RecordTemplate.SetData(key, defaultKeyValue);

        res.SavedType = res.RequestedType;
        res.ResultCode = VaultOperationResultCode::Success;
        return res;
    }

    template <class T>
    VaultOperationResult Vault::GetKeyValue(const std::string& key, T& defaultKeyValue) const noexcept
    {
        DBG_LOG_ENTER();

        static_assert(!std::is_array<T>::value, "It is not possible to use a c array as a key value.");

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
            res.SetOpResult(VaultOperationResultCode::WrongType);
            return res;
        }

        // Check if it is unique key
        if (UniqueKeys.find(key) != UniqueKeys.end())
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::UniqueKey;
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
        DBG_LOG_ENTER();

        static_assert(!std::is_array<T>::value, "It is not possible to use a c array as a key value. \n\
            If you want to use a string as a key, you must specialize the function with a string. Like this: \n\
            GetRecord<std::string>(\"Key\", \"Value\") or GetRecord(\"Key\", std::string(\"Value\"))");

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
            res.SetOpResult(VaultOperationResultCode::WrongType);
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
        DBG_LOG_ENTER();

        static_assert(!std::is_array<T>::value, "It is not possible to use a c array as a key value. \n\
            If you want to use a string as a key, you must specialize the function with a string. Like this: \n\
            GetRecords<std::string>(\"Key\", \"Value\") or GetRecords(\"Key\", std::string(\"Value\"))");

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
            res.SetOpResult(VaultOperationResultCode::WrongType);
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
    VaultOperationResult Vault::RequestEqual(const std::string& key, const T& keyValue, VaultRecordSet& vaultRecordSet,
        const std::size_t& amountOfRecords, const std::function<bool(const VaultRecordRef& ref)>& requestPredicat) const noexcept
    {
        DBG_LOG_ENTER();

        static_assert(!std::is_array<T>::value, "It is not possible to use a c array as a key value. \n\
            If you want to use a string as a key, you must specialize the function with a string. Like this: \n\
            RequestEqual<std::string>(\"Key\", \"Value\") or RequestEqual(\"Key\", std::string(\"Value\"))");

        return RequestRecords(VaultRequestType::Equal, key, keyValue, keyValue, vaultRecordSet, false, false, amountOfRecords, requestPredicat);
    }

    template <class T>
    VaultOperationResult Vault::RequestGreater(const std::string& key, const T& keyValue, VaultRecordSet& vaultRecordSet,
        const std::size_t& amountOfRecords, const std::function<bool(const VaultRecordRef& ref)>& requestPredicat) const noexcept
    {
        DBG_LOG_ENTER();

        static_assert(!std::is_array<T>::value, "It is not possible to use a c array as a key value. \n\
            If you want to use a string as a key, you must specialize the function with a string. Like this: \n\
            RequestGreater<std::string>(\"Key\", \"Value\") or RequestGreater(\"Key\", std::string(\"Value\"))");

        return RequestRecords(VaultRequestType::Greater, key, keyValue, keyValue, vaultRecordSet, false, false, amountOfRecords, requestPredicat);
    }

    template <class T>
    VaultOperationResult Vault::RequestGreaterOrEqual(const std::string& key, const T& keyValue, VaultRecordSet& vaultRecordSet,
        const std::size_t& amountOfRecords, const std::function<bool(const VaultRecordRef& ref)>& requestPredicat) const noexcept
    {
        DBG_LOG_ENTER();

        static_assert(!std::is_array<T>::value, "It is not possible to use a c array as a key value. \n\
            If you want to use a string as a key, you must specialize the function with a string. Like this: \n\
            RequestGreaterOrEqual<std::string>(\"Key\", \"Value\") or RequestGreaterOrEqual(\"Key\", std::string(\"Value\"))");

        return RequestRecords(VaultRequestType::GreaterOrEqual, key, keyValue, keyValue, vaultRecordSet, false, false, amountOfRecords, requestPredicat);
    }

    template <class T>
    VaultOperationResult Vault::RequestLess(const std::string& key, const T& keyValue, VaultRecordSet& vaultRecordSet,
        const std::size_t& amountOfRecords, const std::function<bool(const VaultRecordRef& ref)>& requestPredicat) const noexcept
    {
        DBG_LOG_ENTER();

        static_assert(!std::is_array<T>::value, "It is not possible to use a c array as a key value. \n\
            If you want to use a string as a key, you must specialize the function with a string. Like this: \n\
            RequestLess<std::string>(\"Key\", \"Value\") or RequestLess(\"Key\", std::string(\"Value\"))");

        return RequestRecords(VaultRequestType::Less, key, keyValue, keyValue, vaultRecordSet, false, false, amountOfRecords, requestPredicat);
    }

    template <class T>
    VaultOperationResult Vault::RequestLessOrEqual(const std::string& key, const T& keyValue, VaultRecordSet& vaultRecordSet,
        const std::size_t& amountOfRecords, const std::function<bool(const VaultRecordRef& ref)>& requestPredicat) const noexcept
    {
        DBG_LOG_ENTER();

        static_assert(!std::is_array<T>::value, "It is not possible to use a c array as a key value. \n\
            If you want to use a string as a key, you must specialize the function with a string. Like this: \n\
            RequestLessOrEqual<std::string>(\"Key\", \"Value\") or RequestLessOrEqual(\"Key\", std::string(\"Value\"))");

        return RequestRecords(VaultRequestType::LessOrEqual, key, keyValue, keyValue, vaultRecordSet, false, false, amountOfRecords, requestPredicat);
    }

    template <class T>
    VaultOperationResult Vault::RequestInterval(const std::string& key, const T& beginKeyValue,
        const T& endKeyValue, VaultRecordSet& vaultRecordSet, const bool& isIncludeBeginKeyValue, 
        const bool& isIncludeEndKeyValue, const std::size_t& amountOfRecords, const std::function<bool(const VaultRecordRef& ref)>& requestPredicat) const noexcept
    {
        DBG_LOG_ENTER();

        static_assert(!std::is_array<T>::value, "It is not possible to use a c array as a key value. \n\
            If you want to use a string as a key, you must specialize the function with a string. Like this: \n\
            RequestInterval<std::string>(\"Key\", \"Value\") or RequestInterval(\"Key\", std::string(\"Value\"))");

        return RequestRecords(VaultRequestType::Interval, key, beginKeyValue, 
            endKeyValue, vaultRecordSet, isIncludeBeginKeyValue, isIncludeEndKeyValue, 
            amountOfRecords, requestPredicat);
    }

    template <VaultRequestType Type>
    VaultOperationResult Vault::Request(const VaultRequest<Type>& request, VaultRecordSet& vaultRecordSet) const
    {
        DBG_LOG_ENTER();

        VaultOperationResult res;
        res.IsOperationSuccess = true;
        res.ResultCode = VaultOperationResultCode::Success;

        // Lock Vault to read
        ReadLock<RecursiveReadWriteMutex> readLock(RecursiveReadWriteMtx);

        // If it is Vault object
        if (VaultDerivedClass == VaultDerivedClasses::VaultBase)
        {
            // Save vaultRecordSet
            RecordSetsSet.emplace(&vaultRecordSet);

            // Remove old data from vaultRecordSet
            if (vaultRecordSet.ParentVault != this)
            {
                vaultRecordSet.Reset();

                // Set new parent vault to vaultRecordSet
                vaultRecordSet.ParentVault = const_cast<Vault*>(this);

                // Copy keys from this to vaultRecordSet
                for (auto& keyCopierIt : VaultKeyCopiers)
                    keyCopierIt.second(&vaultRecordSet);

                // Set proper key order
                vaultRecordSet.KeysOrder = KeysOrder;

                // Set unique keys
                vaultRecordSet.UniqueKeys = UniqueKeys;
            }
            else vaultRecordSet.Clear();
        }

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
        catch(VaultOperationResult& result) // Catch complex request errors
        {
            vaultRecordSet.Clear();
            res = result;
        }

        return res;
    }

    template <class T>
    VaultOperationResult Vault::EraseRecord(const std::string& key, const T& keyValue) noexcept
    {
        DBG_LOG_ENTER();

        static_assert(!std::is_array<T>::value, "It is not possible to use a c array as a key value. \n\
            If you want to use a string as a key, you must specialize the function with a string. Like this: \n\
            EraseRecord<std::string>(\"Key\", \"Value\") or EraseRecord(\"Key\", std::string(\"Value\"))");

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
            res.SetOpResult(VaultOperationResultCode::WrongType);
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
        DBG_LOG_ENTER();

        static_assert(!std::is_array<T>::value, "It is not possible to use a c array as a key value. \n\
            If you want to use a string as a key, you must specialize the function with a string. Like this: \n\
            EraseRecords<std::string>(\"Key\", \"Value\") or EraseRecords(\"Key\", std::string(\"Value\"))");

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
            res.SetOpResult(VaultOperationResultCode::WrongType);
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
}