#include "CompareVaultRecordRef.h"

bool CompareRef(const mvlt::VaultRecordRef& vrr, const std::vector<std::pair<std::string, TypeWrapper>>& record)
{
    std::multiset<std::pair<std::string, std::string>> recordSet, inputRecord;

    for (const std::pair<std::string, std::string>& recordPole : vrr.ToStrings())
    {
        recordSet.emplace(recordPole);
    }

    for (const std::pair<std::string, TypeWrapper>& recordPole : record)
    {
        inputRecord.emplace(std::pair<std::string, std::string>(recordPole.first, recordPole.second.Str));
    }

    return recordSet == inputRecord;
}