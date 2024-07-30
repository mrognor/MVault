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

int main()
{
    VaultRecordRef_SetData_Test();
}