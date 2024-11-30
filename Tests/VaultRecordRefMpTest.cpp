// VaultRecordRef multi-threading tests

#include "../Source/MVault.h"

#include "MTests/TestAssert.h"
#include "MTests/Starter.h"

using namespace mvlt;
using namespace std;

void VaultRecordRef_Copy_Test()
{
    Vault vlt;

    vlt.AddKey("A", -1);
        
    for (int i = 0; i < 1000; ++i)
    {
        VaultRecordRef vrr1, vrr2, vrr3, vrr4;
        vlt.CreateRecord(vrr1, {});
        vrr2 = vrr3 = vrr1;

        Starter starter;

        std::thread th1([&]()
        {
            starter.Wait();
            vrr3 = vrr1;
        });

        std::thread th2([&]()
        {
            starter.Wait();
            vrr4 = vrr1;
        });
    
        std::thread th3([&]()
        {
            starter.Wait();
            VaultRecordRef vrr(vrr1);
        });

        std::thread th4([&]()
        {
            starter.Wait();
            vlt.EraseRecord(vrr2);
        });

        th1.join();
        th2.join();
        th3.join();
        th4.join();
    }
}

void VaultRecordRef_OperatorComparison_Test()
{
    Vault vlt;

    vlt.AddKey("A", -1);
        
    for (int i = 0; i < 1000; ++i)
    {
        bool res1, res2;
        VaultRecordRef vrr1, vrr2, vrr3;
        vlt.CreateRecord(vrr1, {});
        vrr2 = vrr1;

        Starter starter;

        std::thread th1([&]()
        {
            starter.Wait();
            res1 = (vrr1 == vrr2);
        });

        std::thread th2([&]()
        {
            starter.Wait();
            res2 = (vrr2 == vrr3);
        });

        std::thread th3([&]()
        {
            starter.Wait();
            vlt.EraseRecord(vrr2);
        });

        th1.join();
        th2.join();
        th3.join();
    }
}

void VaultRecordRef_GetRecordUniqueId_Test()
{
    Vault vlt;

    vlt.AddKey("A", -1);
        
    for (int i = 0; i < 1000; ++i)
    {
        VaultRecordRef vrr1, vrr2;
        vlt.CreateRecord(vrr1, {});
        vrr2 = vrr1;

        Starter starter;

        std::thread th1([&]()
        {
            starter.Wait();
            std::string res = vrr1.GetRecordUniqueId();
        });

        std::thread th2([&]()
        {
            starter.Wait();
            vlt.EraseRecord(vrr2);
        });

        th1.join();
        th2.join();
    }
}

void VaultRecordRef_SetData_Test()
{
    Vault vlt;

    vlt.AddKey("A", -1);
    
    for (int i = 0; i < 1000; ++i)
    {
        VaultRecordRef vrr1, vrr2;
        vlt.CreateRecord(vrr1, {});
        vrr1 = vrr2;

        Starter starter;

        std::thread th1([&]()
        {
            starter.Wait();
            vrr1.SetData("A", 0);
        });

        std::thread th2([&]()
        {
            starter.Wait();
            vlt.EraseRecord(vrr2);
        });

        th1.join();
        th2.join();
    }

    // Second overload
    vlt.AddKey<std::string>("B", "null");
    for (int i = 0; i < 1000; ++i)
    {
        VaultRecordRef vrr1, vrr2;
        vlt.CreateRecord(vrr1, {});
        vrr2 = vrr1;

        Starter starter;

        std::thread th1([&]()
        {
            starter.Wait();
            vrr1.SetData({ {"A", 0}, {"B", std::string("str")} });
        });

        std::thread th2([&]()
        {
            starter.Wait();
            vlt.EraseRecord(vrr2);
        });

        th1.join();
        th2.join();
    }
}

void VaultRecordRef_GetData_Test()
{
    Vault vlt;

    vlt.AddKey("A", -1);
    
    // Add key and get data
    for (int i = 0; i < 1000; ++i)
    {
        TEST_ASSERT(vlt.GetKeys() == std::vector<std::string>{"A"}, "Failed to get vault keys!");
        VaultRecordRef vrr1, vrr2;
        vlt.CreateRecord(vrr1, {});
        vrr2 = vrr1;

        Starter starter;

        std::thread th1([&]()
        {
            int j;
            starter.Wait();
            vrr1.GetData("A", j);
        });

        std::thread th2([&]()
        {
            starter.Wait();
            vlt.EraseRecord(vrr2);
        });

        th1.join();
        th2.join();
    }
}

void VaultRecordRef_GetDataAsString_Test()
{
    Vault vlt;

    vlt.AddKey("A", -1);
    
    // Add key and get data
    for (int i = 0; i < 1000; ++i)
    {
        TEST_ASSERT(vlt.GetKeys() == std::vector<std::string>{"A"}, "Failed to get vault keys!");
        VaultRecordRef vrr1, vrr2;
        vlt.CreateRecord(vrr1, {});
        vrr2 = vrr1;

        Starter starter;

        std::thread th1([&]()
        {
            std::string str;
            starter.Wait();
            vrr1.GetDataAsString("A", str);
        });

        std::thread th2([&]()
        {
            starter.Wait();
            vlt.EraseRecord(vrr2);
        });

        th1.join();
        th2.join();
    }
}

void VaultRecordRef_IsValid_Test()
{
    Vault vlt;

    vlt.AddKey("A", -1);
    
    // Add key and get data
    for (int i = 0; i < 1000; ++i)
    {
        TEST_ASSERT(vlt.GetKeys() == std::vector<std::string>{"A"}, "Failed to get vault keys!");
        VaultRecordRef vrr1, vrr2;
        vlt.CreateRecord(vrr1, {});
        vrr2 = vrr1;

        Starter starter;

        std::thread th1([&]()
        {
            std::string str;
            starter.Wait();
            vrr1.IsValid();
        });

        std::thread th2([&]()
        {
            starter.Wait();
            vlt.EraseRecord(vrr2);
        });

        th1.join();
        th2.join();
    }
}

void VaultRecordRef_IsKeyExist_Test()
{
    Vault vlt;

    vlt.AddKey("A", -1);
    
    // Add key and get data
    for (int i = 0; i < 1000; ++i)
    {
        TEST_ASSERT(vlt.GetKeys() == std::vector<std::string>{"A"}, "Failed to get vault keys!");
        VaultRecordRef vrr1, vrr2;
        vlt.CreateRecord(vrr1, {});
        vrr2 = vrr1;

        Starter starter;

        std::thread th1([&]()
        {
            std::string str;
            starter.Wait();
            vrr1.IsKeyExist("A");
        });

        std::thread th2([&]()
        {
            starter.Wait();
            vlt.EraseRecord(vrr2);
        });

        th1.join();
        th2.join();
    }
}

void VaultRecordRef_GetKeys_Test()
{
    Vault vlt;

    vlt.AddKey("A", -1);
    
    // Add key and get data
    for (int i = 0; i < 1000; ++i)
    {
        TEST_ASSERT(vlt.GetKeys() == std::vector<std::string>{"A"}, "Failed to get vault keys!");
        VaultRecordRef vrr1, vrr2;
        vlt.CreateRecord(vrr1, {});
        vrr2 = vrr1;

        Starter starter;

        std::thread th1([&]()
        {
            std::string str;
            starter.Wait();
            vrr1.GetKeys();
        });

        std::thread th2([&]()
        {
            starter.Wait();
            vlt.EraseRecord(vrr2);
        });

        th1.join();
        th2.join();
    }
}

void VaultRecordRef_Reset_Test()
{
    Vault vlt;

    vlt.AddKey("A", -1);
    
    // Add key and get data
    for (int i = 0; i < 1000; ++i)
    {
        TEST_ASSERT(vlt.GetKeys() == std::vector<std::string>{"A"}, "Failed to get vault keys!");
        VaultRecordRef vrr1, vrr2;
        vlt.CreateRecord(vrr1, {});
        vrr2 = vrr1;

        Starter starter;

        std::thread th1([&]()
        {
            std::string str;
            starter.Wait();
            vrr1.Reset();
        });

        std::thread th2([&]()
        {
            starter.Wait();
            vlt.EraseRecord(vrr2);
        });

        th1.join();
        th2.join();
    }
}

void VaultRecordRef_Destructor_Test()
{
    Vault vlt;

    vlt.AddKey("A", -1);
    
    // Add key and get data
    for (int i = 0; i < 1000; ++i)
    {
        TEST_ASSERT(vlt.GetKeys() == std::vector<std::string>{"A"}, "Failed to get vault keys!");
        VaultRecordRef vrr;
        vlt.CreateRecord(vrr, {});
        
        VaultRecordRef* vrrp = new VaultRecordRef(vrr);

        Starter starter;

        std::thread th1([&]()
        {
            std::string str;
            starter.Wait();
            delete vrrp;
        });

        std::thread th2([&]()
        {
            starter.Wait();
            vlt.EraseRecord(vrr);
        });

        th1.join();
        th2.join();
    }
}

int main()
{
    VaultRecordRef_Copy_Test();
    VaultRecordRef_OperatorComparison_Test();
    VaultRecordRef_GetRecordUniqueId_Test();
    VaultRecordRef_SetData_Test();
    VaultRecordRef_GetData_Test();
    VaultRecordRef_GetDataAsString_Test();
    VaultRecordRef_IsValid_Test();
    VaultRecordRef_IsKeyExist_Test();
    VaultRecordRef_GetKeys_Test();
    VaultRecordRef_Reset_Test();
    VaultRecordRef_Destructor_Test();
}