#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
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

/*! @} */