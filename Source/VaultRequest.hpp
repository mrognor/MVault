#pragma once

#include "VaultRequest.h"

#include "Vault.hpp"

namespace mvlt
{
    template <VaultRequestType Type>
    template <class T>
    VaultRequest<Type>::VaultRequest(const std::string& key, const T& keyValue)
    {
        // Set Request func
        RequestFunc = [=](Vault* vlt, std::unordered_set<VaultRecord*>& vaultRecordSet)
        {
            // Simple data request to vlt
            vlt->RequestRecordsSet(Type, key, keyValue, keyValue, vaultRecordSet, false, false, -1);
        };
    }

    template <VaultRequestType Type>
    template <VaultRequestType RequestType1, VaultRequestType RequestType2>
    VaultRequest<Type>::VaultRequest(const VaultRequest<RequestType1>&& request1, const VaultRequest<RequestType2>&& request2)
    {
        // Switch by request type Or or And
        switch (Type)
        {
        case VaultRequestType::Or:
            // Set complex request func
            RequestFunc = [=](Vault* vlt, std::unordered_set<VaultRecord*>& vaultRecordSet)
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
            RequestFunc = [=](Vault* vlt, std::unordered_set<VaultRecord*>& vaultRecordSet)
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
            };
            break;
        }

    }

    template <VaultRequestType Type>
    void VaultRequest<Type>::Request(Vault* vlt, std::unordered_set<VaultRecord*>& vaultRecords) const
    {
        RequestFunc(vlt, vaultRecords);
    }
}