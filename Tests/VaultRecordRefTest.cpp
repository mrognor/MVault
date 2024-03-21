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

void GetDataAsString(std::ofstream& testResFile)
{
    testResFile << "## VaultRecordRef::GetDataAsString" << std::endl;

    testResFile << "### Synchronous tests" << std::endl;

    mvlt::Vault vlt;
    vlt.SetKey("id", -1);

    mvlt::VaultRecordRef vrr;

    bool isSucces = true;
    std::string dataString;

    if (vrr.GetDataAsString("key", dataString))
    {
        testResFile << "Wrong key: $\\color{red}{Failed!}$\n\n";
        isSucces = false;
    }

    if (vrr.GetDataAsString("id", dataString))
    {
        testResFile << "Empty ref returns true: $\\color{red}{Failed!}$\n\n";
        isSucces = false;
    }

    vrr = vlt.CreateRecord();

    if (vrr.GetDataAsString("key", dataString))
    {
        testResFile << "Wrong key: $\\color{red}{Failed!}$\n\n";
        isSucces = false;
    }

    if (!vrr.GetDataAsString("id", dataString))
    {
        testResFile << "Correct key returns false: $\\color{red}{Failed!}$\n\n";
        isSucces = false;
    }

    if (dataString != "-1")
    {
        testResFile << "Wrong value in ref: $\\color{red}{Failed!}$\n\n";
        isSucces = false;
    }

    if (isSucces)
        testResFile << "Checks: $\\color{green}{Success!}$\n\n";

    testResFile << "### Asynchronous tests" << std::endl;

    testResFile << "VaultRecordRef GetDataAsString while VaultRecordRef deleting in other thread: ";


    for (int i = 0; i < 10000; ++i)
    {
        vrr = vlt.CreateRecord();

        auto timeout = std::chrono::steady_clock::now() + std::chrono::microseconds(10);

        std::thread th1([&]()
        {
            std::this_thread::sleep_until(timeout);
            
            vrr.GetDataAsString("id", dataString);
        });

        std::thread th2([&]()
        {
            std::this_thread::sleep_until(timeout);

            vlt.EraseRecord(vrr);
        });

        th1.join();
        th2.join();
    }

    testResFile << "$\\color{green}{Success!}$\n\n";


    testResFile << "VaultRecordRef GetDataAsString while Vault deleting in other thread: ";

    for (int i = 0; i < 10000; ++i)
    {
        mvlt::Vault* vltp = new mvlt::Vault;
        vltp->SetKey("id", -1);
    
        vrr = vltp->CreateRecord();

        auto timeout = std::chrono::steady_clock::now() + std::chrono::microseconds(100);

        std::thread th3([&]()
        {
            std::this_thread::sleep_until(timeout);
            vrr.GetDataAsString("id", dataString);
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

void GetKeys(std::ofstream& testResFile)
{
    testResFile << "## VaultRecordRef::GetKeys" << std::endl;

    testResFile << "### Synchronous tests" << std::endl;

    mvlt::Vault vlt;
    vlt.SetKey("a", -1);
    vlt.SetKey<std::string>("b", "");
    vlt.SetKey<std::string>("c", "");
    vlt.SetKey("d", true);

    if (vlt.GetKeys() == std::vector<std::string>({"a", "b", "c", "d"}))
        testResFile << "Checks: $\\color{green}{Success!}$\n\n";
    else
        testResFile << "Checks: $\\color{red}{Failed!}$\n\n";

    testResFile << "VaultRecordRef GetKeys while Drop Vault in other thread: ";

    for (int i = 0; i < 10000; ++i)
    {
        std::vector<std::string> keys;

        auto timeout = std::chrono::steady_clock::now() + std::chrono::microseconds(100);

        std::thread th1([&]()
        {
            std::this_thread::sleep_until(timeout);
            keys = vlt.GetKeys();
        });

        std::thread th2([&]()
        {
            std::this_thread::sleep_until(timeout);
            vlt.DropVault();
        });

        th1.join();
        th2.join();
    }

    testResFile << "$\\color{green}{Success!}$\n\n";

    testResFile << "VaultRecordRef GetKeys while Vault deleting in other thread: ";

    for (int i = 0; i < 10000; ++i)
    {
        mvlt::Vault* vltp = new mvlt::Vault;
        vltp->SetKey("id", -1);

        std::vector<std::string> keys;

        auto timeout = std::chrono::steady_clock::now() + std::chrono::microseconds(100);

        std::thread th3([&]()
        {
            std::this_thread::sleep_until(timeout);
            keys = vlt.GetKeys();
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
    GetDataAsString(testResFile);
    GetKeys(testResFile);

    testResFile.close();
    std::cout << "Testing is over" << std::endl;
}