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

        // A pointer for storing a std::unordered_multimap in which a template data type is used as a key, 
        // and a pointer to the DataHashMap is used as a value
        std::unordered_multimap<T, VaultRecord*>* TtoVaultRecordHashMap = nullptr;

        // A pointer for storing a std::multimap in which a template data type is used as a key, 
        // and a pointer to the DataHashMap is used as a value
        std::multimap<T, VaultRecord*>* TtoVaultRecordMap = nullptr;

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
        auto FirstAndLastIteratorsWithKeyOnHashMap = TtoVaultRecordHashMap->equal_range(oldData);

        // Iterate over all data records with oldData key
        for (auto& pairIt = FirstAndLastIteratorsWithKeyOnHashMap.first; pairIt != FirstAndLastIteratorsWithKeyOnHashMap.second; ++pairIt)
        {
            // Find required data record
            if (pairIt->second == dataRecord)
            {
                TtoVaultRecordHashMap->erase(pairIt);
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
        for (auto& pairIt = FirstAndLastIteratorsWithKeyOnMap.first; pairIt != FirstAndLastIteratorsWithKeyOnMap.second; ++pairIt)
        {
            // Find required data record
            if (pairIt->second == dataRecord)
            {
                TtoVaultRecordMap->erase(pairIt);
                break;
            }
        }

        // Add new data to TtoVaultRecordMap to Vault VaultMapStructure
        TtoVaultRecordMap->emplace(data, dataRecord);


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
            std::unordered_multimap<T, VaultRecord*>* TtoVaultRecordHashMap = nullptr;

            // Get hash map
            VaultHashMapStructure.GetData(key, TtoVaultRecordHashMap);
            
            // Pair with begin and end iterator with T type and beginKeyValue value
            auto equalRange = TtoVaultRecordHashMap->equal_range(beginKeyValue);
            if (equalRange.first != TtoVaultRecordHashMap->end())
            {
                std::size_t counter = 0;
                for (auto equalRangeIt = equalRange.first; equalRangeIt != equalRange.second; ++equalRangeIt)
                {
                    // Check added records count
                    if (counter >= amountOfRecords) break;

                    // Check if it is not default predicate
                    if (&requestPredicat != &DefaultRequestPredicat)
                    {
                        if (requestPredicat(VaultRecordRef(equalRangeIt->second, const_cast<Vault*>(this))))
                            vaultRecords.emplace(equalRangeIt->second);
                    }
                    else
                        vaultRecords.emplace(equalRangeIt->second);
                    
                    ++counter;
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
                    for (auto TtoVaultRecordMapIt = startIt; TtoVaultRecordMapIt != TtoVaultRecordMap->end(); ++TtoVaultRecordMapIt)
                    {
                        if (TtoVaultRecordMapIt->first != startIt->first) 
                        {
                            startIt = TtoVaultRecordMapIt;
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
                        
                        // Increase iterator to add later last element in vaultRecords
                        if (endIt != TtoVaultRecordMap->begin())
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
                    if (requestPredicat(VaultRecordRef(TtoVaultRecordMapIt->second, const_cast<Vault*>(this))))
                        vaultRecords.emplace(TtoVaultRecordMapIt->second);
                }
                else
                    vaultRecords.emplace(TtoVaultRecordMapIt->second);

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
        vaultRecordSet.IsParentVaultValid = true;

        // Copy keys from this to vaultRecordSet
        for (auto& keyCopierIt : VaultKeyCopiers)
            keyCopierIt.second(vaultRecordSet);
        
        // Set proper key order
        vaultRecordSet.KeysOrder = KeysOrder;

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
    bool Vault::AddKey(const std::string& key, const T& defaultKeyValue) noexcept
    {
        static_assert(!std::is_array<T>::value, "It is not possible to use a c array as a key. \n\
            If you want to use a string as a key, you must specialize the function with a string. Like this: \n\
            AddKey<std::string>(\"Key\", \"Value\") or AddKey(\"Key\", std::string(\"Value\"))");

        // Lock Vault to write
        WriteLock<RecursiveReadWriteMutex> writeLock(RecursiveReadWriteMtx);

        // If the key was added earlier, then it must be deleted
        if (KeysTypes.find(key) != KeysTypes.end())
            return false;

        // Add key to list with key order
        if(VaultDerivedClass == VaultDerivedClasses::VaultBase)
            KeysOrder.emplace_back(key);

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
                for (auto pairIt = FirstAndLastIteratorsWithKeyOnHashMap.first; pairIt != FirstAndLastIteratorsWithKeyOnHashMap.second; ++pairIt)
                {
                    if (pairIt->second == newRecord)
                    {
                        TtoVaultRecordHashMap->erase(pairIt);
                        break;
                    }
                }

                // Find all elements on map with recordTData value
                auto FirstAndLastIteratorsWithKeyOnMap = TtoVaultRecordMap->equal_range(recordTData);
                // Find newRecord and erase it from TtoVaultRecordHashMap
                for (auto pairIt = FirstAndLastIteratorsWithKeyOnMap.first; pairIt != FirstAndLastIteratorsWithKeyOnMap.second; ++pairIt)
                {
                    if (pairIt->second == newRecord)
                    {
                        TtoVaultRecordMap->erase(pairIt);
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
                for (auto TtoVaultRecordMapIt = TtoVaultRecordMap->rbegin(); TtoVaultRecordMapIt != TtoVaultRecordMap->rend(); ++TtoVaultRecordMapIt)
                    if(!functionToSortedData(VaultRecordRef(TtoVaultRecordMapIt->second, this)))
                        break;
            }
        });
        
        VaultKeyCopiers.emplace(key, [=](VaultRecordSet& vaultRecordSet)
        {
            vaultRecordSet.AddKey(key, defaultKeyValue);
        });  


        if (VaultDerivedClass == VaultDerivedClasses::VaultBase)
        {
            // Add new data to record set
            for (const auto& recordsSetIt : RecordsSet)
            {
                recordsSetIt->SetData(key, defaultKeyValue);
                TtoVaultRecordHashMap->emplace(defaultKeyValue, recordsSetIt);
                TtoVaultRecordMap->emplace(defaultKeyValue, recordsSetIt);
            }

            for (VaultRecordSet* set : RecordSetsSet)
            {
                set->AddKey(key, defaultKeyValue);
                set->KeysOrder.emplace_back(key);
            }
        }
        else
        {
            // Add new data to record set
            for (const auto& recordsSetIt : RecordsSet)
            {
                TtoVaultRecordHashMap->emplace(defaultKeyValue, recordsSetIt);
                TtoVaultRecordMap->emplace(defaultKeyValue, recordsSetIt);
            }
        }

        return true;
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
            res.RequestedType = keyTypeIt->second;
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::WrongType;
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

        RecordTemplate.GetData(key, defaultKeyValue);
        
        res.IsOperationSuccess = true;
        res.ResultCode = VaultOperationResultCode::Success;
        res.SavedType = res.RequestedType;

        return res;
    }

    template <class T>
    VaultOperationResult Vault::GetRecord(const std::string& key, const T& keyValue, VaultRecordRef& vaultRecordRef) const noexcept
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

        return res;
    }

    template <class T>
    VaultOperationResult Vault::GetRecords(const std::string& key, const T& keyValue, std::vector<VaultRecordRef>& recordsRefs, const std::size_t& amountOfRecords) const noexcept
    {
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
        std::unordered_multimap<T, VaultRecord*>* TtoVaultRecordHashMap = nullptr;

        // Checking whether such a key exists
        VaultHashMapStructure.GetData(key, TtoVaultRecordHashMap);
        
        // Pair with begin and end iterator with T type and keyValue value
        auto equalRange = TtoVaultRecordHashMap->equal_range(keyValue);
        if (equalRange.first != TtoVaultRecordHashMap->end())
        {
            std::size_t counter = 0;
            for (auto equalRangeIt = equalRange.first; equalRangeIt != equalRange.second; ++equalRangeIt)
            {
                ++counter;
                recordsRefs.emplace_back(VaultRecordRef(equalRangeIt->second, const_cast<Vault*>(this)));
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
        vaultRecordSet.IsParentVaultValid = true;
        
        // Set key proper key order
        vaultRecordSet.KeysOrder = KeysOrder;

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
        std::unordered_multimap<T, VaultRecord*>* TtoVaultRecordHashMap = nullptr;

        // Checking whether such a key exists
        VaultHashMapStructure.GetData(key, TtoVaultRecordHashMap);
        
        // Pair with begin and end iterator with T type and keyValue value
        auto equalRange = TtoVaultRecordHashMap->equal_range(keyValue);
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

                VaultRecord* tmpRec = equalRangeIt->second;
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