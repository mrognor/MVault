#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

/*! 
    \defgroup CsvParserFunctions Csv parser functions
    \brief All functions for working with DataStorage and csv

    @{      
*/


/**
    \brief A function for converting a string to a csv field format.

    In csv files, the fields of one record are separated by commas. To store commas inside a field, the field is placed in double quotes.  
    To store quotes inside a field, the fields are pla  ced in quotation marks, and all quotes inside the field are duplicated.  

    For example: string ",,"", will be turned into """,,""""".  
    Detailed description of each symbol:
    - "  - a quotation mark to start escaping
    - "" - two quotation marks to indicate the quotation mark character inside the escape
    - ,, - the comma characters
    - "" - two quotation marks to indicate the quotation mark character inside the escape
    - "" - two quotation marks to indicate the quotation mark character inside the escape
    - "  - a quotation mark to end escaping

    \param [in] str the string to be converted

    \return returns a string in csv format
*/
std::string FormatStringToCsv(const std::string& str);

/**
    \brief A method for reading a csv file and loading data from it into memory

    \param [in] fileName the name of the file to read the data from
    \param [in] records a reference to the vector of vectors. 
    The outer vector is a vector of records, the inner vector is a vector of fields of one record

    \return returns true if it was possible to read the file, otherwise it returns false
 */
bool ReadCsvFile(const std::string& fileName, std::vector<std::vector<std::string>>& records);

/**
    \brief A template method for providing an interface converting any type to a string

    This function allows you to use the same interface inside the DataStorage for any class, 
    since this function accepts any type. Therefore, if you do not plan to use the functionality of saving to files, 
    then there will be no problems when using a custom type. At the same time, if you plan to work with files, 
    you can specialize this function for each required type.

    \tparam <T> - Any type of data
    \param [in] data the variable to be converted to a string

    \return This function returns an empty string. 
    The specialization of this function for types will return strings with data from data.
*/
template <class T>
std::string ToString(const T& data) { return ""; }

/**
    \brief ToString specialization for working with strings

    \tparam <std::string> - string as a type
    \param [in] data the variable to be converted to a string

    \return Returns data
*/
template <class T = std::string>
std::string ToString(const std::string& data) { return data; }

/**
    \brief ToString specialization for working with int

    \tparam <int> - int as a type
    \param [in] data the variable to be converted to a string

    \return Returns data converted to string
*/
template <class T = int,long>
std::string ToString(const int& data) { return std::to_string(data); }

/**
    \brief ToString specialization for working with bool

    \tparam <bool> - bool as a type
    \param [in] data the variable to be converted to a string

    \return If data is true, the function returns the string true. Otherwise, it returns the string false
*/
template <class T = bool>
std::string ToString(const bool& data) 
{ 
    if (data) return "true";
    else return "false"; 
}

/// \todo Перенести в пример
bool StringToInt(const std::string& str, int& digit);

/// \todo перенести в пример
bool StringToFloat(const std::string& str, float& digit);

/**
    \brief A template method for providing an interface converting string to a any type

    This function allows you to use the same interface inside the DataStorage for any class, 
    since this function accepts any type. Therefore, if you do not plan to use the functionality of saving to files, 
    then there will be no problems when using a custom type. At the same time, if you plan to work with files, 
    you can specialize this function for each required type.

    \tparam <T> - Any type of data
    \param [in] stringToCopyDataFrom the string to be converted to type T
    \param [out] data the variable where the converted string will be written

    \return Returns true if the conversion was successful, otherwise it returns false
*/
template <class T>
bool FromString(const std::string& stringToCopyDataFrom, T& data) { return false; }

/**
    \brief FromString specialization for working with std::string

    \tparam <std::string> - string as a type
    \param [in] stringToCopyDataFrom the string to be converted to type std::string
    \param [out] data the variable where the converted string will be written

    \return Returns true if the conversion was successful, otherwise it returns false
*/
template <class T = std::string>
bool FromString(const std::string& stringToCopyDataFrom, std::string& data) 
{ 
    data = stringToCopyDataFrom;
    return true;
}

/**
    \brief FromString specialization for working with bool

    If the string is true, then data will be true and the function will return true.
    If the string is false, then data will be false and the function will return true.
    If the string is not true or false, the function returns false.

    \tparam <bool> - bool as a type
    \param [in] stringToCopyDataFrom the string to be converted to type bool
    \param [out] data the variable where the converted string will be written

    \return Returns true if the conversion was successful, otherwise it returns false
*/
template <class T = bool>
bool FromString(const std::string& stringToCopyDataFrom, bool& data) 
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
    \brief FromString specialization for working with int
    
    \tparam <int> - int as a type
    \param [in] stringToCopyDataFrom the string to be converted to type int
    \param [out] data the variable where the converted string will be written

    \return Returns true if the conversion was successful, otherwise it returns false
*/
template <class T = int>
bool FromString(const std::string& stringToCopyDataFrom, int& data) 
{
    return StringToInt(stringToCopyDataFrom, data);
}

/**
    \brief FromString specialization for working with float
    
    \tparam <float> - float as a type
    \param [in] stringToCopyDataFrom the string to be converted to type float
    \param [out] data the variable where the converted string will be written

    \return Returns true if the conversion was successful, otherwise it returns false
*/
template <class T = float>
bool FromString(const std::string& stringToCopyDataFrom, float& data) 
{
    return StringToFloat(stringToCopyDataFrom, data);
}

/*! @} */