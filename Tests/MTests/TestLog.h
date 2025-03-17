#pragma once

#include <vector>
#include <chrono>
#include <fstream>

#include "GetLine.h"
#include "ColorizedPrint.h"

extern std::size_t TestCounter;
extern std::size_t ErrorCounter;
extern std::vector<std::string> FailedTests;

class TestLog
{
private:
    std::string ClassName;
    std::size_t LineNumber = 0;
    decltype(std::chrono::high_resolution_clock::now()) TestStartTime;
public:
    bool IsError = false;

    std::string FunctionName;
    std::string FullName;

    TestLog(const std::string& className, const std::string& functionName);

    ~TestLog();
};

#define TEST_LOG(className) TestLog testLog(className, __FUNCTION__);