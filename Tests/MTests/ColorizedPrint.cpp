#include "ColorizedPrint.h"
#include <string>

void ColorizedPrint(const std::string text, const ConsoleTextColor& color)
{
    if (color != ConsoleTextColor::Default)
    {
        #if defined WIN32 || defined _WIN64

        // Get console descriptor
        HANDLE ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

        // Object to store previous console colour
        CONSOLE_SCREEN_BUFFER_INFO info;
        // Tty to store console colour
        if (GetConsoleScreenBufferInfo(ConsoleHandle, &info))
        {
            SetConsoleTextAttribute(ConsoleHandle, static_cast<std::uint8_t>(color));
            std::cout << text << std::endl;
            SetConsoleTextAttribute(ConsoleHandle, static_cast<std::uint8_t>(info.wAttributes));
        }
        else std::cout << text << std::endl;
        #else
        std::cout << "\x1B[" + std::to_string(static_cast<std::uint8_t>(color)) + "m" + text + "\033[0m" << std::endl;
        #endif
    }
    else
        std::cout << text << std::endl;
}