#include "VaultUnitTests.h"
#include "VaultRecordSetUnitTests.h"
#include "VaultRecordRefUnitTests.h"

const std::uint64_t VaultUnitTestsKey = ~0b0001;
const std::uint64_t VaultRecordSetUnitTestsKey = ~0b0010;
const std::uint64_t VaultRecordRefUnitTestsKey = ~0b0100;

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
            else if (param == "VaultRecordRef") requiredTests &= VaultRecordRefUnitTestsKey;
        }
    }

    auto beg = std::chrono::high_resolution_clock::now();

    if ((requiredTests & ~VaultUnitTestsKey) == 0) 
    {
        ColorizedPrint("[----------]", ConsoleTextColor::Green, "");
        ColorizedPrint(" Tests for Vault");
        VaultUnitTests();
        std::cout << std::endl;
    }
    if ((requiredTests & ~VaultRecordSetUnitTestsKey) == 0) 
    {
        ColorizedPrint("[----------]", ConsoleTextColor::Green, "");
        ColorizedPrint(" Tests for VaultRecordSet");
        VaultRecordSetUnitTests();
        std::cout << std::endl;
    }
    if ((requiredTests & ~VaultRecordRefUnitTestsKey) == 0) 
    {
        ColorizedPrint("[----------]", ConsoleTextColor::Green, "");
        ColorizedPrint(" Tests for VaultRecordSet");
        VaultRecordRefUnitTests();
        std::cout << std::endl;
    }

    auto end = std::chrono::high_resolution_clock::now();

    ColorizedPrint("[==========]", ConsoleTextColor::Green, "");
    /// \todo 2 hardcoded
    ColorizedPrint(" " + std::to_string(TestCounter) + " tests from 2 files ran. (" +
        std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(end - beg).count()) +
        " ms total)");
    ColorizedPrint("[  PASSED  ]" , ConsoleTextColor::Green, "");
    ColorizedPrint(" " + std::to_string(TestCounter - ErrorCounter) + " tests.", ConsoleTextColor::Default);

    if (!FailedTests.empty())
    {
        ColorizedPrint("[  FAILED  ]" , ConsoleTextColor::Red, "");

        if (FailedTests.size() == 1)
            ColorizedPrint(" " + std::to_string(ErrorCounter) + " test, listed below:", ConsoleTextColor::Default);
        else
            ColorizedPrint(" " + std::to_string(ErrorCounter) + " tests, listed below:", ConsoleTextColor::Default);

        for (const std::string& testName : FailedTests)
        {
            ColorizedPrint("[  FAILED  ]" , ConsoleTextColor::Red, "");
            ColorizedPrint(" " + testName, ConsoleTextColor::Default);
        }
    }

    std::ofstream reportFile("report.md");
    reportFile << "Tests: " << TestCounter << "\nPassed: " << (TestCounter - ErrorCounter) 
        << "\nFailed: " << ErrorCounter << "\n" 
        << std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(end - beg).count()) << " ms" << std::endl;
}