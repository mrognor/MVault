#pragma once

#include <string>

namespace mvlt
{
    /**
        \defgroup ToString ToStringFunctions
        \brief Functions for printing custom classes

        @{

        Since the repository supports any type, including custom classes, these functions for converting objects of custom classes must be defined by users. 
        This is implemented using the specialization of a partial function. The library defines a template function **ToString**, 
        which accepts an object of any type and returns an empty string. 
        This is necessary in order for the code to work with any types, even if conversion to a string is not defined for them. 
        However, if you want to add to your type the ability to convert to a string, then you must define the **ToString** function for your type. 

        For example, like this

        \code{.cpp}
        template <>
        std::string mvlt::ToString(const std::vector<int>& data) 
        {
            if (data.size() == 0) return "{}";
            std::string res = "{}";
            for (const int& it : data)
            {
                res += std::to_string(it) + ", ";
            }

            res.pop_back();
            res.pop_back();

            res += "}";
            return res; 
        }
        \endcode

        This code defines the **ToString** function for a vector of integers.

        By calling this function:

        \code{.cpp}
        std::vector<int> vec = {1, 2, 3, 4, 5};
        std::cout << ToString(vec) << std::endl;
        \endcode

        The output will be as follows:  
        ``` {1, 2, 3, 4, 5} ```

        By default, the function is defined for the following types:  
        \* int  
        \* bool  
        \* float  
        \* std::string  
    */

    /**       
        \brief A template method for providing an interface converting any type to a string

        This function allows you to use the same interface inside the Vault for any class, 
        since this function accepts any type. Therefore, if you do not plan to use the functionality of saving to files or printing data, 
        then there will be no problems when using a custom type. At the same time, if you plan to work with files or printing data, 
        you can specialize this function for each required type.

        \tparam <T> Any type of data
        \param [in] data the variable to be converted to a string

        \return This function returns an empty string. 
        The specialization of this function for types will return strings with data from data.
    */
    template <class T>
    std::string ToString(const T& data) noexcept { return ""; }

    /**       
        \brief Specialization of the ToString method for the int type

        \tparam <int> integer variable
        \param [in] data the variable to be converted to a string

        \return result of std::to_string function
    */
    template <>
    inline std::string ToString(const int& data) noexcept { return std::to_string(data); }

    /**       
        \brief Specialization of the ToString method for the bool type

        \tparam <bool> boolean variable
        \param [in] data the variable to be converted to a string

        \return string with true if data true, otherwise retutn string with false
    */
    template <>
    inline std::string ToString(const bool& data) noexcept { if(data) return "true"; else return "false"; }

    /**       
        \brief Specialization of the ToString method for the std::string type

        \tparam <std::string> string variable
        \param [in] data the variable to be converted to a string

        \return data
    */
    template <>
    inline std::string ToString(const std::string& data) noexcept { return data; }

    /**       
        \brief Specialization of the ToString method for the float type

        \tparam <float> float variable
        \param [in] data the variable to be converted to a string

        \return result of std::to_string function
    */
    template <>
    inline std::string ToString(const float& data) noexcept { return std::to_string(data); }

    /**       
        \brief Specialization of the ToString method for the float type

        \tparam <char> char variable
        \param [in] data the variable to be converted to a string

        \return string with one char 
    */
    template <>
    inline std::string ToString(const char& data) noexcept { return std::string(1, data); }

    /**@} */
}