#include "CsvParser.h"

std::string FormatStringToCsv(const std::string& str)
{
    std::stringstream ss;
    ss << str;
    bool isEscapingRequired = false;
    std::string res;
    
    for (const char& symbol : ss.str())
    {
        if (symbol == '"')
        {
            isEscapingRequired = true;
            res += "\"\"";
            continue;
        }

        if (symbol == ',')
            isEscapingRequired = true;

        if (symbol == '\n')
            isEscapingRequired = true;

        res += symbol;
    }

    if (isEscapingRequired)
        return "\"" + res + "\"";
    else
        return res;
}