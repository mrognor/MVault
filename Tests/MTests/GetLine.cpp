#include "GetLine.h"

std::string GetLine(std::basic_istream<char, std::char_traits<char>> & inputStream) noexcept
{
#if defined WIN32 || defined _WIN64 
    std::string line;
    getline(inputStream, line);
    return line;
#else
    std::string line;
    getline(inputStream, line);
    line.pop_back();
    return line;
#endif
}