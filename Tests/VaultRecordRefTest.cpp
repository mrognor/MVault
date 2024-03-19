#include <fstream>

#include "../Source/MVault.h"

void GetDataTest(std::ofstream& testResFile)
{
    mvlt::Vault vlt;
    vlt.SetKey("id", -1);
    
    mvlt::VaultRecordRef vrr1, vrr2;
    vrr1 = vlt.CreateRecord();

    testResFile << "## VaultRecordRef::operator=" << std::endl;

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

int main()
{
    std::ofstream testResFile("TestResult.md");
    mvlt::VaultRecordRef vrr;
    testResFile << "# VaultRecordRef" << std::endl;

    GetDataTest(testResFile);

    testResFile.close();
}