// VaultRecordRef multi-threading tests

#include "../Source/MVault.h"

#include "MTests/TestAssert.h"
#include "MTests/Starter.h"

using namespace mvlt;
using namespace std;

void VaultRecordRef_SetData_Test()
{
    Vault vlt;

    vlt.AddKey("A", -1);
        
    for (int i = 0; i < 1000; ++i)
    {
        VaultRecordRef vrr1, vrr2;
        vrr1 = vrr2 = vlt.CreateRecord();

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
        vrr1 = vrr2 = vlt.CreateRecord();

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
            vlt.AddKey("B", 0);
        });

        th1.join();
        th2.join();
        std::cout << i << std::endl;
        vlt.RemoveKey("B");
    }
}

int main()
{
    VaultRecordRef_SetData_Test();
    VaultRecordRef_GetData_Test();
}