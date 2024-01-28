#include "CsvParser.h"

std::string FormatStringToCsv(const std::string& str)
{
    bool isEscapingRequired = false;
    std::string res;
    
    for (const char& symbol : str)
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

bool ReadCsvFile(const std::string& fileName, std::vector<std::vector<std::string>>& records)
{
    // Open file from end
    std::ifstream csvFile(fileName, std::ios::ate | std::ios::binary);

    // Checking whether the file was opened successfully
    if (!csvFile.is_open())
    {
        std::cout << "Failed to open file!" << std::endl;
        return false;
    }

    // Clear old records
    records.clear();

    // Get file len
    std::size_t fileLen = csvFile.tellg();
    csvFile.seekg(0);

    // Read file
    char* fileData = new char[fileLen];
    csvFile.read(fileData, fileLen);

    // Is escaping flag
    bool isEscaping = false;

    // Vector with strings to represent record
    std::vector<std::string> record;
    // Record pole
    std::string recordPole;

    // Iterate over file
    for (std::size_t i = 0; i < fileLen; ++i)
    {
        // New line check. rfc 4180 allows only CRLF but this code support also only LF
        // If it is still escaping then this is not new record because it is available to store new line symbol in record poles
        if (!isEscaping && (fileData[i] == 13 || fileData[i] == 10 )) // 13 - CR or 0d. 10 - LF or 0a.
        {
            // Add last pole to record
            record.emplace_back(recordPole);
            // Add record to records
            records.emplace_back(record);

            // Clear record
            record.clear();
            // Clear record pole
            recordPole.clear();

            // Check if it is CRLF.
            if (i < fileLen - 1 && fileData[i + 1] == 10)
                ++i;

            continue;
        }
        
        // Handle " symbol
        if (fileData[i] == '"')
        {
            // Check if it was escaping
            if (!isEscaping) 
            {
                // Start escaping
                isEscaping = true;
                recordPole.clear();
                continue;
            }
            
            // Stop escaping check
            if (i < fileLen - 1 && fileData[i + 1] == '"')
            {
                // Add " to record and skip next symbol
                recordPole += '"';
                ++i;
                continue;
            }
            else
                isEscaping = false; // Turn off escaping
            continue;
        }

        // Handle , symbol lf not escaping
        if (fileData[i] == ',' && !isEscaping)
        {
            // Add new record pole to record
            record.emplace_back(recordPole);
            recordPole.clear();
            continue;
        }
        
        // Add symbol to record pole
        recordPole += fileData[i];
    }

    // Add last record pole to record
    /// \todo Проверить необходимость этих проверок
    if (!recordPole.empty())
        record.emplace_back(recordPole);

    /// \todo Проверить необходимость этих проверок    
    if (!record.empty())
        records.emplace_back(record);

    delete[] fileData;
    csvFile.close();
    return true;
}

bool StringToInt(const std::string& str, int& digit)
{
    if (str.length() == 0) return false;

    int res = 0;
    
    std::size_t i = 0;
    bool isNegative = false;

    if (str[0] == '-' && str.length() > 1) 
    {
        isNegative = true;
        i = 1;
    }

    for (; i < str.length(); ++i)
    {
        if (str[i] < '0' || str[i] > '9') return false;
        res *= 10;
        res += static_cast<int>(str[i] - '0');
    }

    if (isNegative) digit = res * -1;
    else digit = res;

    return true;
}

bool StringToFloat(const std::string& str, float& digit)
{
    if (str.length() == 0) return false;

    int integer = 0, fractional = 0;

    std::size_t i = 0;
    bool isNegative = false;

    if (str[0] == '-' && str.length() > 1) 
    {
        isNegative = true;
        i = 1;
    }

    for (; i < str.length(); ++i)
    {
        if (str[i] == '.') 
        {
            ++i;
            break;
        }

        if (str[i] < '0' || str[i] > '9') return false;
        integer *= 10;
        integer += static_cast<int>(str[i] - '0');
    }

    float coeff = 1;
    for (; i < str.length(); ++i)
    {
        if (str[i] < '0' || str[i] > '9') return false;
        coeff /= 10;
        fractional *= 10;
        fractional += static_cast<int>(str[i] - '0');
    }

    digit = static_cast<float>(integer) + static_cast<float>(fractional) * coeff;

    if (isNegative) digit *= -1;

    return true;
}