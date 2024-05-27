#pragma once

#include "ColorizedPrint.h"

#define TEST_ASSERT(expression, description) if (!(expression)) \
{ \
    ColorizedPrint( "Error! " + std::string(description) + ". File: " + std::string(__FILE__) + " at line: " + std::to_string(__LINE__), ConsoleTextColor::Red); \
}
