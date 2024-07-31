#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

namespace mvlt
{
    /**
        \defgroup CsvParserFunctions Csv parser functions
        \brief All functions for working with DataStorage and csv

        @{      
    */

    /**
        \brief A function for converting a string to a csv field format.

        In csv files, the fields of one record are separated by commas. To store commas inside a field, the field is placed in double quotes.  
        To store quotes inside a field, the fields are placed in quotation marks, and all quotes inside the field are duplicated.  

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
    std::string FormatStringToCsv(const std::string& str) noexcept;

    /// \brief A class for parsing csv files that provides data in a form similar to generators
    class CsvParser
    {
    private:
        
        // Pointer to array with file data
        char* FileData = nullptr;

        // Current position in file
        std::size_t ReadingPos = 0;

        // Full file length
        std::size_t FileLen = 0;
    public:

        /**
            \brief The method for opening the file

            \param [in] fileName The name of the file to open

            \return it will return the true if it was possible to open the file otherwise it will return a false
        */
        bool OpenFile(const std::string& fileName) noexcept;

        /**
            \brief The method for getting the next record in the file. It works by analogy with generators in Python

            \param [out] vectorWithNext vector with all fields in the record
            \param [in] separator the symbol used to separate the records in the file

            \return it will return the true if it was possible to open the file otherwise it will return a false
        */
        bool GetNextVector(std::vector<std::string>& vectorWithNext, const char& separator) noexcept;

        /// \brief Default destructor
        ~CsvParser() noexcept;
    };

    /**@} */
}