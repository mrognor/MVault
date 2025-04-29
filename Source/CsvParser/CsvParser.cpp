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

    
    CsvParser::CsvParser() noexcept {}

    bool CsvParser::OpenFile(const std::string& fileName) noexcept
    {
        return FileReader.Open(fileName);
    }

    bool CsvParser::GetNextVector(std::vector<std::string>& vectorWithNext, const char& separator) noexcept
    {
        // Clear old data
        vectorWithNext.clear();

        // Is escaping flag
        bool isEscaping = false;

        // Record pole
        std::string recordPole;

        // Check file size
        if (FileReader.GetFileSize() > 0)
        {
            // Iterate over file
            for (;FileReader.IsEnd(); FileReader.Move())
            {
                // New line check. rfc 4180 allows only CRLF but this code support also just LF
                // If it is still escaping then this is not new record because it is available to store new line symbol in record poles
                if (!isEscaping && (FileReader.GetChar() == 13 || FileReader.GetChar() == 10 )) // 13 - CR or 0d. 10 - LF or 0a.
                {
                    // Add last record
                    if (!recordPole.empty())
                        vectorWithNext.emplace_back(std::move(recordPole));

                    // Check if it is CRLF.
                    if (FileReader.GetCurrentPos() < FileReader.GetFileSize() - 1 && FileReader.GetNextChar() == 10)
                        FileReader.Move();

                    FileReader.Move();
                    return true;
                }
                
                // Handle " symbol
                if (FileReader.GetChar() == '"')
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
                    if (FileReader.GetCurrentPos() < FileReader.GetFileSize() - 1 && FileReader.GetNextChar() == '"')
                    {
                        // Add " to record and skip next symbol
                        recordPole += '"';
                        FileReader.Move();
                        continue;
                    }
                    else
                        isEscaping = false; // Turn off escaping
                    continue;
                }

                // Handle separator symbol lf not escaping
                if (FileReader.GetChar() == separator && !isEscaping)
                {
                    // Set new record pole to record
                    vectorWithNext.emplace_back(std::move(recordPole));
                    recordPole.clear();
                    continue;
                }
                
                // Add symbol to record pole
                recordPole += FileReader.GetChar();
            }
        }

        // Add last record
        if (!recordPole.empty())
            vectorWithNext.emplace_back(std::move(recordPole));

        if (!vectorWithNext.empty())
            return true;
        
        return false;
    }
}