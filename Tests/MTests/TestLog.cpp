#include "TestLog.h"

std::size_t TestLog::TestCounter = 0;
std::size_t TestLog::ErrorCounter = 0;

TestLog::TestLog(const std::string& className, const std::string& functionName, const std::size_t& lineNumber, const std::string& testModuleName) 
    : FunctionName(functionName)
{
    if (FunctionName != "main") ColorizedPrint(className + "::" + functionName + " started: ", ConsoleTextColor::Default, "");;

    std::size_t slashPos = testModuleName.rfind("/");
    TestModuleName = testModuleName.substr(slashPos + 1, testModuleName.rfind(".") - slashPos - 1);
}

TestLog::~TestLog()
{
    if (FunctionName != "main")
    {
        if (!IsError) 
            ColorizedPrint("passed!", ConsoleTextColor::Green);
        else
            ++ErrorCounter;

        ++TestCounter;
    }
    else
    {
        ColorizedPrint("============================================================");
        ColorizedPrint("Total tests: " + std::to_string(TestCounter) + ". ", ConsoleTextColor::Default, "");
        ColorizedPrint("Passed: " + std::to_string(TestCounter - ErrorCounter) + ". ", ConsoleTextColor::Green, "");
        ColorizedPrint("Failed: " + std::to_string(ErrorCounter) + ". ", ConsoleTextColor::Red);
        ColorizedPrint("============================================================");

        std::ofstream reportFile("report.md", std::ios::app);
        reportFile << TestModuleName << "\n  Tests: <b>" << std::to_string(TestCounter) << "</b>\n" << "    Passed: <b>" << std::to_string(TestCounter - ErrorCounter) 
            << "</b>\n    Failed: <b>" << std::to_string(ErrorCounter) << "</b>\n    \n";
    }
}