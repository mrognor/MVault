#pragma once

#include "GetLine.h"
#include "ColorizedPrint.h"

#define TEST_ASSERT(expression, description) if (!(expression)) \
{ \
    ColorizedPrint("failed!", ConsoleTextColor::Red); \
    ColorizedPrint("\tError! " + std::string(description) + ". File: " + std::string(__FILE__) + " at line: " + std::to_string(__LINE__), ConsoleTextColor::Red); \
    testLog.IsError = true; /* See TestLog.h */ \
}
