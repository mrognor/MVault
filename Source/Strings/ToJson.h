#pragma once

#include "../DataSaver.h"

namespace mvlt
{
    /**
        \brief A function for determining whether to add quotation marks to a string representation of a type to be stored inside a json string

        \param [in] dataType type to check

        \return true if needed, otherwise return false
    */
    bool IsQuotedJsonType(const std::type_index& dataType);

    /**
        \brief Method for convert DataSaver to json string

        \param [in] dataSaver

        \return string to save in json
    */
    std::string ToJson(const DataSaver& dataSaver);

    /**
        \brief A template method for converting data to json strings

        \tparam <T> Any type of data

        \param [in] data the variable to be converted to a json strings

        \return json string with data
    */
    template<class T>
    std::string ToJson(const T& data)
    {
        std::string res;

        if (IsQuotedJsonType(typeid(T)))
            res = "\"" + ToString(data) + "\"";
        else
            res = ToString(data);

        return res;
    }
}