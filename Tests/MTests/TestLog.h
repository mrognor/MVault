#pragma once

#include <fstream>

#include "GetLine.h"
#include "ColorizedPrint.h"

class TestLog
{
public:
    bool IsError = false;

    static std::size_t TestCounter;
    static std::size_t ErrorCounter;

    std::string FunctionName;
    std::string TestModuleName;

    TestLog(const std::string& className, const std::string& functionName, const std::size_t& lineNumber, const std::string& testModuleName);

    ~TestLog();
};

#define TEST_LOG(className) TestLog testLog(className, __FUNCTION__, __LINE__, __FILE__);