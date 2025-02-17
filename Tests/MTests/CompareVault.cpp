#include "CompareVault.h"

std::string MTestsToJson(const std::vector<std::vector<std::pair<std::string, TypeWrapper>>>& records)
{
    std::string res = "[";

    for (const auto& record : records)
    {
        std::size_t start = res.length();
        for (const auto& field : record)
            res += ",\"" + field.first + "\":" + field.second.Str; 

        res[start] = '{';
        res += "},";
    }

    if (res.length() > 1) res.pop_back();
    res += "]";

    return res;
}