#pragma once

#include "VaultRequest.h"

#include "Vault.hpp"

namespace mvlt
{
    template <VaultRequestType Type>
    template <class T>
    VaultRequest<Type>::VaultRequest(const std::string& key, const T& keyValue)
    {
        switch (Type) 
        {
        case VaultRequestType::Equal:
            RequestFunc = [=](Vault* vlt, VaultRecordSet& vaultRecordSet)
            {
                vlt->RequestEqual(key, keyValue, vaultRecordSet);
            };
            break;

        case VaultRequestType::Greater:
            RequestFunc = [=](Vault* vlt, VaultRecordSet& vaultRecordSet)
            {
                vlt->RequestGreater(key, keyValue, vaultRecordSet);
            };
            break;

        case VaultRequestType::GreaterOrEqual:
            RequestFunc = [=](Vault* vlt, VaultRecordSet& vaultRecordSet)
            {
                vlt->RequestGreaterOrEqual(key, keyValue, vaultRecordSet);
            };
            break;

        case VaultRequestType::Less:
            RequestFunc = [=](Vault* vlt, VaultRecordSet& vaultRecordSet)
            {
                vlt->RequestLess(key, keyValue, vaultRecordSet);
            };
            break;

        case VaultRequestType::LessOrEqual:
            RequestFunc = [=](Vault* vlt, VaultRecordSet& vaultRecordSet)
            {
                vlt->RequestLessOrEqual(key, keyValue, vaultRecordSet);
            };
            break;
        }
    }

    template <VaultRequestType Type>
    void VaultRequest<Type>::Request(Vault* vlt, VaultRecordSet& vaultRecordSet) const
    {
        RequestFunc(vlt, vaultRecordSet);
    }
}