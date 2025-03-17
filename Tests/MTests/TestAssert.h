#pragma once

#include "GetLine.h"
#include "ColorizedPrint.h"

#define TEST_ASSERT(expression, ...) if (!(expression)) \
{ \
    std::string commentString{__VA_ARGS__}; \
    if (commentString.empty()) ColorizedPrint("Error! File: " + std::string(__FILE__) + " at line: " + std::to_string(__LINE__), ConsoleTextColor::Red);\
    else ColorizedPrint(std::string("Error! ") + commentString + ". File: " + std::string(__FILE__) + " at line: " + std::to_string(__LINE__), ConsoleTextColor::Red); \
    testLog.IsError = true; /* See TestLog.h */ \
    if (FailedTests.empty()) FailedTests.emplace_back(testLog.FullName); \
    else if (FailedTests.back() != testLog.FullName) FailedTests.emplace_back(testLog.FullName); \
}
