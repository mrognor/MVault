#include "ColorizedPrint.h"
#include <string>

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
    std::cout << "\x1B[" + std::to_string(color) + "m" + text + "\033[0m" << std::endl;
    #endif
}