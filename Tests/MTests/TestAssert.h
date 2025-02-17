#pragma once

#include "GetLine.h"
#include "ColorizedPrint.h"

#define TEST_ASSERT(expression, description) if (!(expression)) \
{ \
    if (!testLog.IsError) ColorizedPrint("failed!", ConsoleTextColor::Red); \
    ColorizedPrint("Error! " + std::string(description) + ". File: " + std::string(__FILE__) + " at line: " + std::to_string(__LINE__), ConsoleTextColor::Red); \
    testLog.IsError = true; /* See TestLog.h */ \
}
