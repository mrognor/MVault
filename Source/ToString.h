#pragma once

#include <string>

namespace mvlt
{
    /**
        \todo Добавить описание этой функции в доки
        
        \brief A template method for providing an interface converting any type to a string

        This function allows you to use the same interface inside the Vault for any class, 
        since this function accepts any type. Therefore, if you do not plan to use the functionality of saving to files, 
        then there will be no problems when using a custom type. At the same time, if you plan to work with files, 
        you can specialize this function for each required type.

        \tparam <T> Any type of data
        \param [in] data the variable to be converted to a string

        \return This function returns an empty string. 
        The specialization of this function for types will return strings with data from data.
    */
    template <class T>
    std::string ToString(const T& data) { return ""; }

    /// \todo Добавить описание этой функции в доки
    template <>
    inline std::string ToString(const int& data) { return std::to_string(data); }

    /// \todo Добавить описание этой функции в доки
    template <>
    inline std::string ToString(const bool& data) { if(data) return "true"; else return "false"; }

    /// \todo Добавить описание этой функции в доки
    template <>
    inline std::string ToString(const std::string& data) { return data; }

    /// \todo Добавить описание этой функции в доки
    template <>
    inline std::string ToString(const float& data) { return std::to_string(data); }
}