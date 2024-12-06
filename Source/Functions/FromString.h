#pragma once

#include <string>

namespace mvlt
{
    /**
        \defgroup FromStringFunctions From string functions
        \brief All functions for working with reading data from csv

        @{      
    */

    /**
        \brief A template method for providing an interface converting string to a any type

        This function allows you to use the same interface inside the DataStorage for any class, 
        since this function accepts any type. Therefore, if you do not plan to use the functionality of saving to files, 
        then there will be no problems when using a custom type. At the same time, if you plan to work with files, 
        you can specialize this function for each required type.

        \tparam <T> Any type of data

        \param [in] stringToCopyDataFrom the string to be converted to type T
        \param [out] data the variable where the converted string will be written

        \return Returns true if the conversion was successful, otherwise it returns false
    */
    template <class T>
    inline bool FromString(const std::string& stringToCopyDataFrom, T& data) noexcept { return false; }

    /**
        \brief FromString specialization for working with std::string

        \tparam <std::string> string as a type

        \param [in] stringToCopyDataFrom the string to be converted to type std::string
        \param [out] data the variable where the converted string will be written

        \return Returns true if the conversion was successful, otherwise it returns false
    */
    template <>
    inline bool FromString(const std::string& stringToCopyDataFrom, std::string& data) noexcept
    { 
        data = stringToCopyDataFrom;
        return true;
    }

    /**
        \brief FromString specialization for working with bool

        If the string is true, then data will be true and the function will return true.
        If the string is false, then data will be false and the function will return true.
        If the string is not true or false, the function returns false.

        \tparam <bool> bool as a type

        \param [in] stringToCopyDataFrom the string to be converted to type bool
        \param [out] data the variable where the converted string will be written

        \return Returns true if the conversion was successful, otherwise it returns false
    */
    template <>
    inline bool FromString(const std::string& stringToCopyDataFrom, bool& data) noexcept
    {
        if (stringToCopyDataFrom == "true") 
        {
            data = true;
            return true;
        }
        if (stringToCopyDataFrom == "false") 
        {
            data = false;
            return true;
        }
        return false;
    }

    /**
        \brief FromStringDoubleAndFloat template for working with float and double
        
        \tparam <T> floatating type
        \tparam <float> float type
        \tparam <double> double type

        \param [in] stringToCopyDataFrom the string to be converted to type float
        \param [out] data the variable where the converted string will be written

        \return Returns true if the conversion was successful, otherwise it returns false
    */
    template <class T>
    inline bool FromStringDoubleAndFloat(const std::string& stringToCopyDataFrom, T& data) noexcept
    {
        if (stringToCopyDataFrom.length() == 0) return false;

        int integer = 0, fractional = 0;

        std::size_t i = 0;
        bool isNegative = false;

        if (stringToCopyDataFrom[0] == '-' && stringToCopyDataFrom.length() > 1) 
        {
            isNegative = true;
            i = 1;
        }

        for (; i < stringToCopyDataFrom.length(); ++i)
        {
            if (stringToCopyDataFrom[i] == '.') 
            {
                ++i;
                break;
            }

            if (stringToCopyDataFrom[i] < '0' || stringToCopyDataFrom[i] > '9') return false;
            integer *= 10;
            integer += static_cast<int>(stringToCopyDataFrom[i] - '0');
        }

        T coeff = 1;
        for (; i < stringToCopyDataFrom.length(); ++i)
        {
            if (stringToCopyDataFrom[i] < '0' || stringToCopyDataFrom[i] > '9') return false;
            coeff /= 10;
            fractional *= 10;
            fractional += static_cast<int>(stringToCopyDataFrom[i] - '0');
        }

        data = static_cast<T>(integer) + static_cast<T>(fractional) * coeff;

        if (isNegative) data *= -1;

        return true;
    }

    /**
        \brief FromString specialization for working with float
        
        \tparam <float> float as a type

        \param [in] stringToCopyDataFrom the string to be converted to type float
        \param [out] data the variable where the converted string will be written

        \return Returns true if the conversion was successful, otherwise it returns false
    */
    template <>
    inline bool FromString(const std::string& stringToCopyDataFrom, float& data) noexcept
    {
        return FromStringDoubleAndFloat(stringToCopyDataFrom, data);
    }

    /**
        \brief FromString specialization for working with double
        
        \tparam <double> double as a type

        \param [in] stringToCopyDataFrom the string to be converted to type float
        \param [out] data the variable where the converted string will be written

        \return Returns true if the conversion was successful, otherwise it returns false
    */
    template <>
    inline bool FromString(const std::string& stringToCopyDataFrom, double& data) noexcept
    {
        return FromStringDoubleAndFloat(stringToCopyDataFrom, data);
    }

    /**
        \brief FromStringSignedInt template for working with all signed integers
        
        \tparam <T> signed integer as a type
        \tparam <short> short int as a type
        \tparam <int> signed int as a type
        \tparam <long> long int as a type
        \tparam <long long> long long int as a type

        \param [in] stringToCopyDataFrom the string to be converted to type int
        \param [out] data the variable where the converted string will be written

        \return Returns true if the conversion was successful, otherwise it returns false
    */
    template <class T>
    inline bool FromStringSignedInt(const std::string& stringToCopyDataFrom, T& data) noexcept
    {
        if (stringToCopyDataFrom.length() == 0) return false;

        T res = 0;
        
        std::size_t i = 0;
        bool isNegative = false;

        if (stringToCopyDataFrom[0] == '-' && stringToCopyDataFrom.length() > 1) 
        {
            isNegative = true;
            i = 1;
        }

        for (; i < stringToCopyDataFrom.length(); ++i)
        {
            if (stringToCopyDataFrom[i] < '0' || stringToCopyDataFrom[i] > '9') return false;
            res *= 10;
            res += static_cast<int>(stringToCopyDataFrom[i] - '0');
        }

        if (isNegative) data = res * -1;
        else data = res;

        return true;
    }

    /**
        \brief FromString specialization for working with short int
        
        \tparam <short> int as a type

        \param [in] stringToCopyDataFrom the string to be converted to type int
        \param [out] data the variable where the converted string will be written

        \return Returns true if the conversion was successful, otherwise it returns false
    */
    template <>
    inline bool FromString(const std::string& stringToCopyDataFrom, short& data) noexcept
    {
        return FromStringSignedInt(stringToCopyDataFrom, data);
    }

    /**
        \brief FromString specialization for working with int
        
        \tparam <int> int as a type

        \param [in] stringToCopyDataFrom the string to be converted to type int
        \param [out] data the variable where the converted string will be written

        \return Returns true if the conversion was successful, otherwise it returns false
    */
    template <>
    inline bool FromString(const std::string& stringToCopyDataFrom, int& data) noexcept
    {
        return FromStringSignedInt(stringToCopyDataFrom, data);
    }

    /**
        \brief FromString specialization for working with long
        
        \tparam <long> int as a type

        \param [in] stringToCopyDataFrom the string to be converted to type long
        \param [out] data the variable where the converted string will be written

        \return Returns true if the conversion was successful, otherwise it returns false
    */
    template <>
    inline bool FromString(const std::string& stringToCopyDataFrom, long& data) noexcept
    {
        return FromStringSignedInt(stringToCopyDataFrom, data);
    }

    /**
        \brief FromString specialization for working with long long
        
        \tparam <long long> int as a type

        \param [in] stringToCopyDataFrom the string to be converted to type long long
        \param [out] data the variable where the converted string will be written

        \return Returns true if the conversion was successful, otherwise it returns false
    */
    template <>
    inline bool FromString(const std::string& stringToCopyDataFrom, long long& data) noexcept
    {
        return FromStringSignedInt(stringToCopyDataFrom, data);
    }

    /**
        \brief FromStringSignedInt template for working with all unsigned signed integers
        
        \tparam <T> unsigned integer as a type
        \tparam <unsigned short> unsigned short int as a type
        \tparam <unsigned int> unsigned signed int as a type
        \tparam <unsigned long> unsigned long int as a type
        \tparam <unsigned long long> unsigned long long int as a type

        \param [in] stringToCopyDataFrom the string to be converted to type int
        \param [out] data the variable where the converted string will be written

        \return Returns true if the conversion was successful, otherwise it returns false
    */
    template <class T>
    inline bool FromStringUnsignedInt(const std::string& stringToCopyDataFrom, T& data) noexcept
    {
        if (stringToCopyDataFrom.length() == 0) return false;

        T res = 0;

        for (std::size_t i = 0; i < stringToCopyDataFrom.length(); ++i)
        {
            if (stringToCopyDataFrom[i] < '0' || stringToCopyDataFrom[i] > '9') return false;
            res *= 10;
            res += static_cast<int>(stringToCopyDataFrom[i] - '0');
        }

        data = res;
        return true;
    }

    /**
        \brief FromString specialization for working with unsigned short
        
        \tparam <unsigned short> int as a type

        \param [in] stringToCopyDataFrom the string to be converted to type long long
        \param [out] data the variable where the converted string will be written

        \return Returns true if the conversion was successful, otherwise it returns false
    */
    template <>
    inline bool FromString(const std::string& stringToCopyDataFrom, unsigned short& data) noexcept
    {
        return FromStringSignedInt(stringToCopyDataFrom, data);
    }

    /**
        \brief FromString specialization for working with unsigned int
        
        \tparam <unsigned int> int as a type

        \param [in] stringToCopyDataFrom the string to be converted to type unsigned int
        \param [out] data the variable where the converted string will be written

        \return Returns true if the conversion was successful, otherwise it returns false
    */
    template <>
    inline bool FromString(const std::string& stringToCopyDataFrom, unsigned int& data) noexcept
    {
        return FromStringSignedInt(stringToCopyDataFrom, data);
    }

    /**
        \brief FromString specialization for working with unsigned unsigned long int
        
        \tparam <unsigned long int> int as a type

        \param [in] stringToCopyDataFrom the string to be converted to type unsigned long int
        \param [out] data the variable where the converted string will be written

        \return Returns true if the conversion was successful, otherwise it returns false
    */
    template <>
    inline bool FromString(const std::string& stringToCopyDataFrom, unsigned long& data) noexcept
    {
        return FromStringSignedInt(stringToCopyDataFrom, data);
    }

    /**
        \brief FromString specialization for working with unsigned unsigned long long int
        
        \tparam <unsigned long long int> int as a type

        \param [in] stringToCopyDataFrom the string to be converted to type unsigned long long int
        \param [out] data the variable where the converted string will be written

        \return Returns true if the conversion was successful, otherwise it returns false
    */
    template <>
    inline bool FromString(const std::string& stringToCopyDataFrom, unsigned long long& data) noexcept
    {
        return FromStringSignedInt(stringToCopyDataFrom, data);
    }

    /**@} */
}