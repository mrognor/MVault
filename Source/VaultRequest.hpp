#pragma once

#include "VaultRequest.h"

#include "Vault.hpp"

namespace mvlt
{
    template<class T>
    Equal::Equal(const std::string& key, const T& keyValue)
    {
        RequestFunc = [=](Vault* vlt, VaultRecordSet& vaultRecordSet)
            {
                vlt->RequestEqual(key, keyValue, vaultRecordSet);
            };
    }

    template<class T>
    Greater::Greater(const std::string& key, const T& keyValue)
    {
        RequestFunc = [=](Vault* vlt, VaultRecordSet& vaultRecordSet)
            {
                vlt->RequestGreater(key, keyValue, vaultRecordSet);
            };
    }

    template<class T>
    GreaterOrEqual::GreaterOrEqual(const std::string& key, const T& keyValue)
    {
        RequestFunc = [=](Vault* vlt, VaultRecordSet& vaultRecordSet)
            {
                vlt->RequestGreaterOrEqual(key, keyValue, vaultRecordSet);
            };
    }

    template<class T>
    Less::Less(const std::string& key, const T& keyValue)
    {
        RequestFunc = [=](Vault* vlt, VaultRecordSet& vaultRecordSet)
            {
                vlt->RequestLess(key, keyValue, vaultRecordSet);
            };
    }

    template<class T>
    LessOrEqual::LessOrEqual(const std::string& key, const T& keyValue)
    {
        RequestFunc = [=](Vault* vlt, VaultRecordSet& vaultRecordSet)
            {
                vlt->RequestLessOrEqual(key, keyValue, vaultRecordSet);
            };
    }
}