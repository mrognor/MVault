#pragma once

#include "VaultRequest.h"

#include "Vault.hpp"

namespace mvlt
{
    template <VaultRequestType Type>
    template <class T>
    VaultRequest<Type>::VaultRequest(const std::string& key, const T& keyValue, std::function<bool(const VaultRecordRef&)> requestPredicat) : Key(key), RequestPredicat(requestPredicat)
    {
        DataPtr = static_cast<void*>(new T(keyValue));

        // Set Request func
        DataRequestFunc = [](const std::string& key, Vault* vlt, std::unordered_set<VaultRecord*>& vaultRecordSet, void* beginValue, void* endValue,
            bool isIncludeBeginKeyValue, bool isIncludeEndKeyValue, std::function<bool(const VaultRecordRef&)> requestPredicat)
        {
            VaultOperationResult res;
            // Simple data request to vlt
            if (endValue == nullptr)
                res = vlt->RequestRecordsSet(Type, key, *static_cast<T*>(beginValue), *static_cast<T*>(beginValue), vaultRecordSet, false, false, -1, requestPredicat);
            else
                res = vlt->RequestRecordsSet(VaultRequestType::Interval, key, *static_cast<T*>(beginValue), *static_cast<T*>(endValue), vaultRecordSet, isIncludeBeginKeyValue, isIncludeEndKeyValue, -1, requestPredicat);

            // Throw error
            if (!res.IsOperationSuccess) throw res;
        };

        DeleteFunc = [](void* ptrToDelete)
        {
            delete static_cast<T*>(ptrToDelete);
        };
    }

    template <VaultRequestType Type>
    template <VaultRequestType RequestType1, VaultRequestType RequestType2>
    VaultRequest<Type>::VaultRequest(const VaultRequest<RequestType1>& request1, const VaultRequest<RequestType2>& request2) noexcept
    {
        // Switch by request type Or or And
        switch (Type)
        {
        case VaultRequestType::Or:
            // Set complex request func
            LogicalRequestFunction = [&](Vault* vlt, std::unordered_set<VaultRecord*>& vaultRecordSet)
            {
                std::unordered_set<VaultRecord*> req1Result, req2Result;

                // Make requests in Or(req1, req2)
                request1.Request(vlt, req1Result);
                request2.Request(vlt, req2Result);

                // Union requests
                for (VaultRecord* record : req1Result)
                    vaultRecordSet.emplace(record);
                for (VaultRecord* record : req2Result)
                    vaultRecordSet.emplace(record);
            };
            break;
        
        case VaultRequestType::And:
            // Set complex request func
            LogicalRequestFunction = [&](Vault* vlt, std::unordered_set<VaultRecord*>& vaultRecordSet)
            {
                // Handle interval situation
                if (request1.Key == request2.Key && 
                    (request1.RequestType == VaultRequestType::Greater || request1.RequestType == VaultRequestType::GreaterOrEqual) &&
                    (request2.RequestType == VaultRequestType::Less || request1.RequestType == VaultRequestType::LessOrEqual))
                {
                    request1.DataRequestFunc(request1.Key, vlt, vaultRecordSet, request1.DataPtr, request2.DataPtr, 
                        (request1.RequestType == VaultRequestType::GreaterOrEqual),
                        (request2.RequestType == VaultRequestType::LessOrEqual), [&](const VaultRecordRef& ref) 
                            {
                                if (request1.RequestPredicat(ref) && request2.RequestPredicat(ref))
                                    return true;
                                else
                                    return false;
                            });
                }
                else // Standart and request
                {
                    std::unordered_set<VaultRecord*> req1Result, req2Result;

                    // Make requests in And(req1, req2)
                    request1.Request(vlt, req1Result);
                    request2.Request(vlt, req2Result);

                    // Set less request to iterate
                    if (req1Result.size() > req2Result.size())
                        std::swap(req1Result, req2Result);

                    // Find intersection
                    for (VaultRecord* record : req1Result)
                        if (req2Result.find(record) != req2Result.end())
                            vaultRecordSet.emplace(record);
                }
            };
            break;
        }
    }

    template <VaultRequestType Type>
    void VaultRequest<Type>::Request(Vault* vlt, std::unordered_set<VaultRecord*>& vaultRecords) const
    {
        switch (Type) 
        {
        case VaultRequestType::Less:
        case VaultRequestType::LessOrEqual:
        case VaultRequestType::Equal:
        case VaultRequestType::GreaterOrEqual:
        case VaultRequestType::Greater:
            DataRequestFunc(Key, vlt, vaultRecords, DataPtr, nullptr, false, false, RequestPredicat);
            break;
        case VaultRequestType::Or:
        case VaultRequestType::And:
            LogicalRequestFunction(vlt, vaultRecords);
            break;
        }
    }

    template <VaultRequestType Type>
    VaultRequest<Type>::~VaultRequest() noexcept
    {
        if (DeleteFunc != nullptr && DataPtr != nullptr)
            DeleteFunc(DataPtr);
    }

    template <VaultRequestType LType, VaultRequestType RType>
    VaultRequest<VaultRequestType::And> operator&& (const VaultRequest<LType>& lhs, const VaultRequest<RType>& rhs) noexcept
    {
        return VaultRequest<VaultRequestType::And>(lhs, rhs);
    }

    template <VaultRequestType LType, VaultRequestType RType>
    VaultRequest<VaultRequestType::Or> operator|| (const VaultRequest<LType>& lhs, const VaultRequest<RType>& rhs) noexcept
    {
        return VaultRequest<VaultRequestType::Or>(lhs, rhs);
    }
}