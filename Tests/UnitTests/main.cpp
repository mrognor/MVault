#include "VaultUnitTests.h"
#include "VaultRecordSetUnitTests.h"

const std::uint64_t VaultUnitTestsKey = ~0b0001;
const std::uint64_t VaultRecordSetUnitTestsKey = ~0b0010;

std::vector<std::string> ReadParams(int argc, char** argv)
{
    std::vector<std::string> res;

    for (int i = 1; i < argc; ++i)
    {
        std::string argument;
        char* argPtr = argv[i];
        
        // Read char from argument and save it to argument var
        while (*argPtr != 0)
        {
            argument.push_back(*argPtr);
            ++argPtr;
        }
        
        res.emplace_back(std::move(argument));
    }

    return res;
}

int main(int argc, char** argv)
{
    std::uint64_t requiredTests = 0;
    std::vector<std::string> params = ReadParams(argc, argv);

    if (!params.empty())
    {
        requiredTests = ~0;
        for (const std::string& param : params)
        {
            if (param == "Vault") requiredTests &= VaultUnitTestsKey;
            else if (param == "VaultRecordSet") requiredTests &= VaultRecordSetUnitTestsKey;
        }
    }

    if ((requiredTests & ~VaultUnitTestsKey) == 0) VaultUnitTests();
    if ((requiredTests & ~VaultRecordSetUnitTestsKey) == 0) VaultRecordSetUnitTests();


    ColorizedPrint("============================================================");
    ColorizedPrint("Total tests: " + std::to_string(TestLog::TestCounter) + ". ", ConsoleTextColor::Default, "");
    ColorizedPrint("Passed: " + std::to_string(TestLog::TestCounter - TestLog::ErrorCounter) + ". ", ConsoleTextColor::Green, "");
    ColorizedPrint("Failed: " + std::to_string(TestLog::ErrorCounter) + ". ", ConsoleTextColor::Red);
    ColorizedPrint("============================================================");

    std::ofstream reportFile("report.md");
    reportFile << "Tests: " << std::to_string(TestLog::TestCounter) << "\nPassed: " << std::to_string(TestLog::TestCounter - TestLog::ErrorCounter) 
        << "\nFailed: " << std::to_string(TestLog::ErrorCounter);
}