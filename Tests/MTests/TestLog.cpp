#include "TestLog.h"

std::size_t TestLog::TestCounter = 0;
std::size_t TestLog::ErrorCounter = 0;

TestLog::TestLog(const std::string& className, const std::string& functionName, const std::size_t& lineNumber) 
    : FunctionName(functionName)
{
    if (FunctionName != "main") ColorizedPrint(className + "::" + functionName + " started: ", ConsoleTextColor::Default, "");;
}

TestLog::~TestLog()
{
    if (!IsError) 
        ColorizedPrint("passed!", ConsoleTextColor::Green);
    else
        ++ErrorCounter;

    ++TestCounter;
}