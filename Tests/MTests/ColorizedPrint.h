#pragma once

#include <cstdint>
#include <iostream>

#if defined WIN32 || defined _WIN64
#include <Windows.h>

enum class ConsoleTextColor : std::uint8_t
{
    Default = 7,
    Blue = 1,
    Green = 2,
    Cyan = 3,
    Red = 4,
    Pink = 5,
    Yellow = 6,
    White = 7,
    BrightBlue = 9,
    BrightGreen = 10,
    BrightCyan = 11,
    BrightRed = 12,
    BrightPink = 13,
    BrightYellow = 14,
    BrightWhite = 15
};
#else

enum class ConsoleTextColor : std::uint8_t
{
    Default = 33,
    Blue = 34,
    Green = 32,
    Cyan = 36,
    Red = 31,
    Pink = 35,
    Yellow = 33,
    White = 37,
    BrightBlue = 94,
    BrightGreen = 92,
    BrightCyan = 96,
    BrightRed = 91,
    BrightPink = 95,
    BrightYellow = 93,
    BrightWhite = 97
};

#endif

void ColorizedPrint(const std::string text, const ConsoleTextColor& color = ConsoleTextColor::Default, const std::string& end = "\n");
