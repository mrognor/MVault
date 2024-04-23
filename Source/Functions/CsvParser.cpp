#include "CsvParser.h"

namespace mvlt
{
    std::string FormatStringToCsv(const std::string& str) noexcept
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

    
    bool CsvParser::OpenFile(const std::string& fileName) noexcept
    {
        // Open file from end
        std::ifstream csvFile(fileName, std::ios::ate | std::ios::binary);

        // Checking whether the file was opened successfully
        if (!csvFile.is_open()) return false;

        // Get file len
        FileLen = csvFile.tellg();
        csvFile.seekg(0);

        // Read file
        FileData = new char[FileLen];
        csvFile.read(FileData, FileLen);

        // Close file
        csvFile.close();

        ReadingPos = 0;

        return true;
    }

    bool CsvParser::GetNextVector(std::vector<std::string>& vectorWithNext, const char& separator) noexcept
    {
        // Is escaping flag
        bool isEscaping = false;

        // Record pole
        std::string recordPole;

        // Iterate over file
        for (; ReadingPos < FileLen; ++ReadingPos)
        {
            // New line check. rfc 4180 allows only CRLF but this code support also just LF
            // If it is still escaping then this is not new record because it is available to store new line symbol in record poles
            if (!isEscaping && (FileData[ReadingPos] == 13 || FileData[ReadingPos] == 10 )) // 13 - CR or 0d. 10 - LF or 0a.
            {
                // Add last record
                if (!recordPole.empty())
                    vectorWithNext.emplace_back(std::move(recordPole));

                // Check if it is CRLF.
                if (ReadingPos < FileLen - 1 && FileData[ReadingPos + 1] == 10)
                    ++ReadingPos;

                ++ReadingPos;
                return true;
            }
            
            // Handle " symbol
            if (FileData[ReadingPos] == '"')
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
                if (ReadingPos < FileLen - 1 && FileData[ReadingPos + 1] == '"')
                {
                    // Add " to record and skip next symbol
                    recordPole += '"';
                    ++ReadingPos;
                    continue;
                }
                else
                    isEscaping = false; // Turn off escaping
                continue;
            }

            // Handle separator symbol lf not escaping
            if (FileData[ReadingPos] == separator && !isEscaping)
            {
                // Set new record pole to record
                vectorWithNext.emplace_back(std::move(recordPole));
                recordPole.clear();
                continue;
            }
            
            // Add symbol to record pole
            recordPole += FileData[ReadingPos];
        }

        // Add last record
        if (!recordPole.empty())
            vectorWithNext.emplace_back(std::move(recordPole));
        
        if (!vectorWithNext.empty())
            return true;
        
        return false;
    }

    CsvParser::~CsvParser() noexcept
    {
        if (FileData != nullptr) delete[] FileData;
    }
}