#pragma once

#include "TypeWrapper.h"

#define COMPARE_VAULT(vlt, ...) TEST_ASSERT(CompareVault(vlt, __VA_ARGS__))

template <class VaultClass>
bool CompareVault(const VaultClass& vlt, const std::vector<std::vector<std::pair<std::string, TypeWrapper>>>& records)
{
    std::multiset<std::multiset<std::pair<std::string, std::string>>> vaultRecords, inputRecords;

    for (const std::vector<std::pair<std::string, std::string>>& record : vlt.ToStrings())
    {
        std::multiset<std::pair<std::string, std::string>> recordSet;

        for (const auto& recPair : record)
            recordSet.emplace(recPair);

        vaultRecords.emplace(recordSet);
    }

    for (const std::vector<std::pair<std::string, TypeWrapper>>& record : records)
    {
        std::multiset<std::pair<std::string, std::string>> recordSet;

        for (const auto& recPair : record)
            recordSet.emplace(std::pair<std::string, std::string>(recPair.first, recPair.second.Str));

        inputRecords.emplace(recordSet);
    }

    return vaultRecords == inputRecords;
}