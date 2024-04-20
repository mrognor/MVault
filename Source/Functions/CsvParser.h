#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

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
std::string FormatStringToCsv(const std::string& str);

/**@} */