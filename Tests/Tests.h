#pragma once

#if defined WIN32 || defined _WIN64
#include <Windows.h>
#endif

#include "../Source/DataStorage.h"

enum ConsoleTextColor
{
    Default = 7,
    Blue = 1,
    Green = 2,
    Cyan = 3,
    Red = 4,
    Pink = 5,
    Yellow = 6,
    White = 7,
    Gray = 8,
    BrightBlue = 9,
    BrightGreen = 10,
    BrightCyan = 11,
    BrightRed = 12,
    BrightPink = 13,
    BrightYellow = 14,
    BrightWhite = 15
};

void ColorizedPrint(const std::string text, const ConsoleTextColor& color = ConsoleTextColor::Default);

void MultithreadingReadTest();

void MultithreadingKeysTest();

void MultithreadingRecordsTest();