#include "ToJson.h"

namespace mvlt
{
    bool IsQuotedJsonType(const std::type_index& dataType)
    {
        if (dataType == typeid(std::int8_t) ||
            dataType == typeid(std::uint8_t) ||
            dataType == typeid(std::int16_t) ||
            dataType == typeid(std::uint16_t) ||
            dataType == typeid(std::int32_t) ||
            dataType == typeid(std::uint32_t) ||
            dataType == typeid(std::int64_t) ||
            dataType == typeid(std::uint64_t) ||
            dataType == typeid(float) ||
            dataType == typeid(double) ||
            dataType == typeid(bool))
            return false;
        else
            return true;
    }

    std::string ToJson(const DataSaver& dataSaver)
    {
        std::string res;
        std::type_index dataType = dataSaver.GetDataType();

        if (IsQuotedJsonType(dataType))
            res = "\"" + dataSaver.Str() + "\"";
        else
            res = dataSaver.Str();

        return res;
    }
}