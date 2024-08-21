// VaultRecordSet multi-threading tests

#include "../Source/MVault.h"

#include "MTests/TestAssert.h"
#include "MTests/Starter.h"

using namespace mvlt;
using namespace std;

void VaultRecordSet_Copy_Test()
{
    Vault vlt;

    for (int i = 0; i < 1000; ++i)
    {
        vlt.AddKey("A", -1);
        for (int j = 0; j < 1000; ++j)
            vlt.CreateRecord({{"A", j}});

        VaultRecordSet vrs1, vrs2;
        vlt.RequestLess("A", i, vrs1);

        Starter starter;

        std::thread th1([&]()
        {
            starter.Wait();
            vrs2 = vrs1;
        });

        std::thread th2([&]()
        {
            starter.Wait();
            VaultRecordSet vrs(vrs1);
        });

        std::thread th3([&]()
        {
            starter.Wait();
            vlt.DropVault();
        });

        th1.join();
        th2.join();
        th3.join();
    }
}

int main()
{
    std::cout << "Start!" << std::endl;

    VaultRecordSet_Copy_Test();

    std::cout << "Success!" << std::endl;
}