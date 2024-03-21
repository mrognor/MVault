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

    testResFile << "### Asynchronous tests" << std::endl;

    testResFile << "VaultRecordRef GetKeys while Drop Vault in other thread: ";

    for (int i = 0; i < 10000; ++i)
    {
        std::vector<std::string> keys;

        vlt.SetKey("a", -1);
        vlt.SetKey<std::string>("b", "");
        vlt.SetKey<std::string>("c", "");
        vlt.SetKey("d", true);

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

void SetData(std::ofstream& testResFile)
{
    testResFile << "## VaultRecordRef::SetData" << std::endl;

    testResFile << "### Synchronous tests" << std::endl;

    mvlt::VaultRecordRef vrr;

    mvlt::VaultOperationResult vor = vrr.SetData("Key", -1);

    if (vor.ResultCode != mvlt::VaultOperationResultCode::DataRecordNotValid)
       testResFile << "Wrong operation result code: $\\color{red}{Failed!}$\n\n";

    mvlt::Vault vlt;

    vlt.SetKey("id", -1);
    
    vrr = vlt.CreateRecord();
    
    bool isSuccess = true;

    vor = vrr.SetData("WrongKey", -1);

    if (vor.ResultCode != mvlt::VaultOperationResultCode::WrongKey)
    {
        testResFile << "SetData with wrong key: $\\color{red}{Failed!}$\n\n";
        isSuccess = false;
    }

    vor = vrr.SetData<bool>("id", true);

    if (vor.ResultCode != mvlt::VaultOperationResultCode::WrongType)
    {
        testResFile << "SetData with wrong key type: $\\color{red}{Failed!}$\n\n";
        isSuccess = false;
    }

    vor = vrr.SetData("id", 1);

    if (vor.ResultCode != mvlt::VaultOperationResultCode::Success)
    {
        testResFile << "CorrectOperation: $\\color{red}{Failed!}$\n\n";
        isSuccess = false;
    }

    if(isSuccess)
        testResFile << "Operations: $\\color{green}{Success!}$\n\n";

    testResFile << "### Asynchronous tests" << std::endl;

    testResFile << "VaultRecordRef SetData 5 threads: ";

    mvlt::VaultRecordRef vrr2 = vrr, vrr3 = vrr;

    for (int i = 0; i < 10000; ++i)
    {
        auto timeout = std::chrono::steady_clock::now() + std::chrono::microseconds(100);

        std::thread th1([&]()
        {
            std::this_thread::sleep_until(timeout);
            vrr.SetData("id", i);
        });

        std::thread th2([&]()
        {
            std::this_thread::sleep_until(timeout);
            vrr.SetData("id", i);
        });

        std::thread th3([&]()
        {
            std::this_thread::sleep_until(timeout);
            vrr.SetData("id", i);
        });

        std::thread th4([&]()
        {
            std::this_thread::sleep_until(timeout);
            vrr2.SetData("id", i);
        });

        std::thread th5([&]()
        {
            std::this_thread::sleep_until(timeout);
            vrr3.SetData("id", i);
        });

        th1.join();
        th2.join();
        th3.join();
        th4.join();
        th5.join();
    }

    vrr3.PrintRecord();
    
    testResFile << "$\\color{green}{Success!}$\n\n";
    
    testResFile << "Set and erase: ";

    for (int i = 0; i < 10000; ++i)
    {
        vrr = vlt.CreateRecord();
        
        auto timeout = std::chrono::steady_clock::now() + std::chrono::microseconds(100);

        std::thread th1([&]()
        {
            std::this_thread::sleep_until(timeout);
            vrr.SetData("id", i);
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

    testResFile << "Delete vlt: ";

    for (int i = 0; i < 10000; ++i)
    {
        mvlt::Vault* vltp = new mvlt::Vault;
        vltp->SetKey("id", -1);
        vrr = vltp->CreateRecord();
        
        auto timeout = std::chrono::steady_clock::now() + std::chrono::microseconds(100);

        std::thread th1([&]()
        {
            std::this_thread::sleep_until(timeout);
            vrr.SetData("id", i);
        });

        std::thread th2([&]()
        {
            std::this_thread::sleep_until(timeout);
            delete vltp;
        });

        th1.join();
        th2.join();
    }

    testResFile << "$\\color{green}{Success!}$\n\n";
}

void GetData(std::ofstream& testResFile)
{
    testResFile << "## VaultRecordRef::GetData" << std::endl;

    testResFile << "### Synchronous tests" << std::endl;

    mvlt::VaultRecordRef vrr;

    int i;
    mvlt::VaultOperationResult vor = vrr.GetData("Key", i);

    if (vor.ResultCode != mvlt::VaultOperationResultCode::DataRecordNotValid)
       testResFile << "Wrong operation result code: $\\color{red}{Failed!}$\n\n";

    mvlt::Vault vlt;

    vlt.SetKey("id", -1);
    
    vrr = vlt.CreateRecord();
    
    bool isSuccess = true;

    vor = vrr.GetData("WrongKey", i);

    if (vor.ResultCode != mvlt::VaultOperationResultCode::WrongKey)
    {
        testResFile << "SetData with wrong key: $\\color{red}{Failed!}$\n\n";
        isSuccess = false;
    }

    bool b;
    vor = vrr.GetData<bool>("id", b);

    if (vor.ResultCode != mvlt::VaultOperationResultCode::WrongType)
    {
        testResFile << "SetData with wrong key type: $\\color{red}{Failed!}$\n\n";
        isSuccess = false;
    }

    vor = vrr.GetData("id", i);

    if (vor.ResultCode != mvlt::VaultOperationResultCode::Success)
    {
        testResFile << "CorrectOperation: $\\color{red}{Failed!}$\n\n";
        isSuccess = false;
    }

    if(isSuccess)
        testResFile << "Operations: $\\color{green}{Success!}$\n\n";

    testResFile << "### Asynchronous tests" << std::endl;

    testResFile << "Set and get" << std::endl;

    mvlt::VaultRecordRef vrr2 = vrr;

    for (int j = 0; j < 10000; ++j)
    {
        auto timeout = std::chrono::steady_clock::now() + std::chrono::microseconds(100);

        std::thread th1([&]()
        {
            std::this_thread::sleep_until(timeout);
            vrr.GetData("id", i);
        });

        std::thread th2([&]()
        {
            std::this_thread::sleep_until(timeout);
            vrr.GetData("id", i);
        });

        std::thread th3([&]()
        {
            std::this_thread::sleep_until(timeout);
            vrr.SetData("id", j);
        });

        std::thread th4([&]()
        {
            std::this_thread::sleep_until(timeout);
            vrr2.GetData("id", j);
        });

        th1.join();
        th2.join();
        th3.join();
        th4.join();
    }

    testResFile << "$\\color{green}{Success!}$\n\n";

    testResFile << "Get and erase: ";

    for (int j = 0; j < 10000; ++j)
    {
        vrr = vlt.CreateRecord();
        
        auto timeout = std::chrono::steady_clock::now() + std::chrono::microseconds(100);

        std::thread th1([&]()
        {
            std::this_thread::sleep_until(timeout);
            vrr.GetData("id", i);
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

    testResFile << "Delete vlt: ";

    for (int j = 0; j < 10000; ++j)
    {
        mvlt::Vault* vltp = new mvlt::Vault;
        vltp->SetKey("id", -1);
        vrr = vltp->CreateRecord();
        
        auto timeout = std::chrono::steady_clock::now() + std::chrono::microseconds(100);

        std::thread th1([&]()
        {
            std::this_thread::sleep_until(timeout);
            vrr.GetData("id", i);
        });

        std::thread th2([&]()
        {
            std::this_thread::sleep_until(timeout);
            delete vltp;
        });

        th1.join();
        th2.join();
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
    SetData(testResFile);
    GetData(testResFile);

    testResFile.close();
    std::cout << "Testing is over" << std::endl;
}