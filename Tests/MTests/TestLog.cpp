#include "TestLog.h"

std::size_t TestCounter = 0;
std::size_t ErrorCounter = 0;
std::vector<std::string> FailedTests;

TestLog::TestLog(const std::string& className, const std::string& functionName) 
    : ClassName(className), FunctionName(functionName)
{
    FullName = className + "::" + functionName;
    if (FunctionName != "main") 
    {
        ColorizedPrint("[ RUN      ]", ConsoleTextColor::Green, "");
        ColorizedPrint(" " + FullName);
        TestStartTime = std::chrono::high_resolution_clock::now();
    }
}

TestLog::~TestLog()
{
    if (!IsError) 
    {
        auto testEndTime = std::chrono::high_resolution_clock::now();
        ColorizedPrint("[       OK ]", ConsoleTextColor::Green, "");
        ColorizedPrint(" " + FullName + " (" + 
            std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(testEndTime - TestStartTime).count()) + " ms)");
    }
    else
    {
        auto testEndTime = std::chrono::high_resolution_clock::now();
        ColorizedPrint("[  FAILED  ]", ConsoleTextColor::Red, "");
        ColorizedPrint(" " + FullName + " (" + 
            std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(testEndTime - TestStartTime).count()) + " ms)");
        ++ErrorCounter;
    }

    ++TestCounter;
}