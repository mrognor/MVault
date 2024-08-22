// VaultRecordSet multi-threading tests

#include "../Source/MVault.h"

#include "MTests/TestAssert.h"
#include "MTests/Starter.h"

using namespace mvlt;
using namespace std;

void VaultRecordSet_Copy_Test()
{
    Vault vlt;

    vlt.AddKey("A", -1);

    for (int i = 0; i < 1000; ++i)
    {
        for (int j = 0; j < 1000; ++j)
            vlt.CreateRecord({{"A", j}});

        VaultRecordSet vrs1, vrs2, vrs3;
        vlt.RequestLess("A", i, vrs1);
        vlt.RequestGreater("A", i, vrs2);

        Starter starter;

        std::thread th1([&]()
        {
            starter.Wait();
            vrs2 = vrs1;
        });

        std::thread th2([&]()
        {
            starter.Wait();
            vrs3 = vrs1;
        });

        std::thread th3([&]()
        {
            starter.Wait();
            VaultRecordSet vrs(vrs1);
        });

        std::thread th4([&]()
        {
            starter.Wait();
            vlt.DropData();
        });

        th1.join();
        th2.join();
        th3.join();
        th4.join();
    }
}

int main()
{
    std::cout << "Start!" << std::endl;

    VaultRecordSet_Copy_Test();

    std::cout << "Success!" << std::endl;
}