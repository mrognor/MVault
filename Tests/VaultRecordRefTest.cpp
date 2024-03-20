#include <fstream>

#include "../Source/MVault.h"

void OperatorAssignment(std::ofstream& testResFile)
{
    mvlt::Vault vlt;
    vlt.SetKey("id", -1);

    mvlt::VaultRecordRef vrr1, vrr2, vrr3;
    vrr1 = vlt.CreateRecord();

    testResFile << "## VaultRecordRef::operator=" << std::endl;

    testResFile << "### Synchronous tests" << std::endl;
    
    // Invalid to invalid
    vrr2 = vrr3;
    // Valid to invalid
    vrr2 = vrr1;
    // Invalid to valid
    vrr1 = vrr3;
    // Valid to valid
    vrr1 = vrr2;
    vrr2 = vlt.CreateRecord();
    vrr2 = vrr1;

    testResFile << "Assignments: $\\color{green}{Success!}$" << std::endl;

    testResFile << "### Asynchronous tests" << std::endl;

    testResFile << "VaultRecordRef assignment while SetData in other thread: ";

    auto timeout = std::chrono::steady_clock::now() + std::chrono::milliseconds(10);

    std::thread th1([&]()
    {
        std::this_thread::sleep_until(timeout);

        for (int i = 0; i < 10000; ++i)
            vrr2 = vrr1;
    });

    std::thread th2([&]()
    {
        std::this_thread::sleep_until(timeout);

        for (int i = 0; i < 10000; ++i)
            vrr1.SetData("id", i);
    });

    th1.join();
    th2.join();

    vrr2.PrintRecord();

    testResFile << "$\\color{green}{Success!}$\n\n";


    testResFile << "VaultRecordRef assignment while Vault deleting in other thread: ";

    for (int i = 0; i < 10000; ++i)
    {
        mvlt::Vault* vltp = new mvlt::Vault;
        vltp->SetKey("id", -1);
    
        vrr1 = vltp->CreateRecord();

        timeout = std::chrono::steady_clock::now() + std::chrono::microseconds(100);

        std::thread th3([&]()
        {
            std::this_thread::sleep_until(timeout);

            vrr2 = vrr1;
        });

        std::thread th4([&]()
        {
            std::this_thread::sleep_until(timeout);

            delete vltp;
        });

        th3.join();
        th4.join();
    }

    testResFile << "$\\color{green}{Success!}$\n\n";
}

void OperatorComparison(std::ofstream& testResFile)
{
    testResFile << "## VaultRecordRef::operator==" << std::endl;

    testResFile << "### Synchronous tests" << std::endl;

    mvlt::Vault vlt;
    vlt.SetKey("id", -1);

    mvlt::VaultRecordRef vrr1, vrr2;

    bool isSucces = true;

    // Compare two empty
    if (!(vrr1 == vrr2))
    {
        testResFile << "Two emptyes comparison: $\\color{red}{Failed!}$\n\n";
        isSucces = false;
    }

    // Compare empty and not
    vrr1 = vlt.CreateRecord();

    if (vrr1 == vrr2)
    {
        testResFile << "Empty and non empty comparison: $\\color{red}{Failed!}$\n\n";
        isSucces = false;
    }

    // Compare two equals
    vrr2 = vrr1;

    if (!(vrr1 == vrr2))
    {
        testResFile << "Two equals comparison: $\\color{red}{Failed!}$\n\n";
        isSucces = false;
    }

    vrr2 = vlt.CreateRecord();

    // Compare two not equals
    if (vrr1 == vrr2)
    {
        testResFile << "Two not equals comparison: $\\color{red}{Failed!}$\n\n";
        isSucces = false;
    }

    if (isSucces)
        testResFile << "Comparisons: $\\color{green}{Success!}$\n\n";

    testResFile << "### Asynchronous tests" << std::endl;

    testResFile << "VaultRecordRef comparisons while Vault deleting in other thread: ";

    for (int i = 0; i < 10000; ++i)
    {
        mvlt::Vault* vltp = new mvlt::Vault;
        vltp->SetKey("id", -1);
    
        vrr1 = vltp->CreateRecord();

        bool f;
        auto timeout = std::chrono::steady_clock::now() + std::chrono::microseconds(100);

        std::thread th3([&]()
        {
            std::this_thread::sleep_until(timeout);

            f = (vrr2 == vrr1);
        });

        std::thread th4([&]()
        {
            std::this_thread::sleep_until(timeout);

            delete vltp;
        });

        th3.join();
        th4.join();
    }

    testResFile << "$\\color{green}{Success!}$\n\n";
}

int main()
{
    std::cout << "Testing has started" << std::endl;
    std::ofstream testResFile("TestResult.md");
    mvlt::VaultRecordRef vrr; 
    testResFile << "# VaultRecordRef" << std::endl;

    OperatorAssignment(testResFile);
    OperatorComparison(testResFile);

    testResFile.close();
    std::cout << "Testing is over" << std::endl;
}