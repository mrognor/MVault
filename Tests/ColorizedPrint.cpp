#include "Tests.h"

#if defined WIN32 || defined _WIN64
HANDLE ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
#endif

void ColorizedPrint(const std::string text, const ConsoleTextColor& color)
{
    #if defined WIN32 || defined _WIN64
    SetConsoleTextAttribute(ConsoleHandle, color);
    std::cout << text << std::endl;    
    SetConsoleTextAttribute(ConsoleHandle, ConsoleTextColor::Default);
    #else
    std::cerr << "\x1B[94m" << msg << "\033[0m" << std::endl;
    #endif
}