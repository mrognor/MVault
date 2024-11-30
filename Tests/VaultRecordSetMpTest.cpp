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

void VaultRecordSet_OperatorComparison_Test()
{
    Vault vlt;

    vlt.AddKey("A", -1);
    std::atomic_bool b;

    for (int i = 0; i < 1000; ++i)
    {
        for (int j = 0; j < 1000; ++j)
            vlt.CreateRecord({{"A", j}});

        VaultRecordSet vrs1, vrs2, vrs3, vrs4;
        vlt.RequestLess("A", i, vrs1);
        vlt.RequestGreater("A", i, vrs2);
        vrs3 = vrs1;

        Starter starter;

        std::thread th1([&]()
        {
            starter.Wait();
            b.store(vrs2 == vrs1, std::memory_order_relaxed);
        });

        std::thread th2([&]()
        {
            starter.Wait();
            b.store(vrs3 == vrs4, std::memory_order_relaxed);
        });

        std::thread th3([&]()
        {
            starter.Wait();
            vlt.DropData();
        });

        th1.join();
        th2.join();
        th3.join();
    }
}

void VaultRecordSet_GetIsParentVaultValid_Test()
{
    Vault vlt;

    vlt.AddKey("A", -1);

    for (int i = 0; i < 1000; ++i)
    {
        for (int j = 0; j < 1000; ++j)
            vlt.CreateRecord({{"A", j}});

        VaultRecordSet vrs;
        vlt.RequestLess("A", i, vrs);

        Starter starter;

        std::thread th1([&]()
        {
            starter.Wait();
            vrs.GetIsParentVaultValid();
        });

        std::thread th2([&]()
        {
            starter.Wait();
            vlt.DropData();
        });

        th1.join();
        th2.join();
    }
}

void VaultRecordSet_IsKeyExist_Test()
{
    Vault vlt;

    vlt.AddKey("A", -1);
    for (int j = 0; j < 1000; ++j)
        vlt.CreateRecord({{"A", j}});

    for (int i = 0; i < 1000; ++i)
    {
        VaultRecordSet vrs;
        vlt.RequestLess("A", i, vrs);

        Starter starter;

        std::thread th1([&]()
        {
            starter.Wait();
            vrs.IsKeyExist("A");
        });

        std::thread th2([&]()
        {
            starter.Wait();
            vlt.RemoveKey("A");
        });

        th1.join();
        th2.join();

        vlt.AddKey("A", i);
    }
}

void VaultRecordSet_GetKeyValue_Test()
{
    Vault vlt;

    vlt.AddKey("A", -1);
    for (int j = 0; j < 1000; ++j)
        vlt.CreateRecord({{"A", j}});

    int iv;

    for (int i = 0; i < 1000; ++i)
    {
        VaultRecordSet vrs;
        vlt.RequestLess("A", i, vrs);

        Starter starter;

        std::thread th1([&]()
        {
            starter.Wait();
            vrs.GetKeyValue("A", iv);
        });

        std::thread th2([&]()
        {
            starter.Wait();
            vlt.RemoveKey("A");
        });

        th1.join();
        th2.join();

        vlt.AddKey("A", i);
    }
}

void VaultRecordSet_GetKeyType_Test()
{
    Vault vlt;

    vlt.AddKey("A", -1);
    for (int j = 0; j < 1000; ++j)
        vlt.CreateRecord({{"A", j}});

    std::type_index ty = typeid(void);;

    for (int i = 0; i < 1000; ++i)
    {
        VaultRecordSet vrs;
        vlt.RequestLess("A", i, vrs);

        Starter starter;

        std::thread th1([&]()
        {
            starter.Wait();
            vrs.GetKeyType("A", ty);
        });

        std::thread th2([&]()
        {
            starter.Wait();
            vlt.RemoveKey("A");
        });

        th1.join();
        th2.join();

        vlt.AddKey("A", i);
    }
}

void VaultRecordSet_AddRecord_Test()
{
    Vault vlt;

    vlt.AddKey("A", -1);


    for (int i = 0; i < 1000; ++i)
    {
        for (int j = 0; j < 1000; ++j)
            vlt.CreateRecord({{"A", j}});

        VaultRecordSet vrs;
        vlt.RequestLess("A", i, vrs);

        VaultRecordRef vrr;
        vlt.CreateRecord(vrr, {});

        Starter starter;

        std::thread th1([&]()
        {
            starter.Wait();
            vrs.AddRecord(vrr);
        });

        std::thread th2([&]()
        {
            starter.Wait();
            vlt.DropData();
        });

        th1.join();
        th2.join();
    }
}

void VaultRecordSet_GetRecord_Test()
{
    Vault vlt;
    VaultRecordRef vrr;

    vlt.AddKey("A", -1);

    for (int i = 0; i < 1000; ++i)
    {
        for (int j = 0; j < 1000; ++j)
            vlt.CreateRecord({{"A", j}});

        VaultRecordSet vrs;
        vlt.RequestLess("A", i, vrs);

        Starter starter;

        std::thread th1([&]()
        {
            starter.Wait();
            vrs.GetRecord("A", i, vrr);
        });

        std::thread th2([&]()
        {
            starter.Wait();
            vlt.DropData();
        });

        th1.join();
        th2.join();
    }
}

void VaultRecordSet_GetRecords_Test()
{
    Vault vlt;
    VaultRecordRef vrr;

    vlt.AddKey("A", -1);

    for (int i = 0; i < 1000; ++i)
    {
        for (int j = 0; j < 1000; ++j)
            vlt.CreateRecord({{"A", i}});

        VaultRecordSet vrs;
        vlt.RequestLess("A", i, vrs);
        std::vector<VaultRecordRef> resVec;

        Starter starter;

        std::thread th1([&]()
        {
            starter.Wait();
            vrs.GetRecords("A", i, resVec);
        });

        std::thread th2([&]()
        {
            starter.Wait();
            vlt.DropData();
        });

        th1.join();
        th2.join();
    }
}

void VaultRecordSet_Request_Test()
{
    Vault vlt;
    VaultRecordRef vrr;

    vlt.AddKey("A", -1);

    for (int i = 0; i < 1000; ++i)
    {
        for (int j = 0; j < 1000; ++j)
            vlt.CreateRecord({{"A", j}});

        VaultRecordSet vrs1, vrs2;
        vlt.RequestLess("A", i, vrs1);

        Starter starter;

        std::thread th1([&]()
        {
            starter.Wait();
            vrs1.Request(Greater("A", 200) && Less("A", 600), vrs2);
        });

        std::thread th2([&]()
        {
            starter.Wait();
            vlt.DropData();
        });

        th1.join();
        th2.join();
    }
}

void VaultRecordSet_Reset_Test()
{
    Vault vlt;
    VaultRecordRef vrr;

    vlt.AddKey("A", -1);

    for (int i = 0; i < 1000; ++i)
    {
        for (int j = 0; j < 1000; ++j)
            vlt.CreateRecord({{"A", j}});

        VaultRecordSet vrs;
        vlt.RequestLess("A", i, vrs);

        Starter starter;

        std::thread th1([&]()
        {
            starter.Wait();
            vrs.Reset();
        });

        std::thread th2([&]()
        {
            starter.Wait();
            vlt.DropData();
        });

        th1.join();
        th2.join();
    }
}

void VaultRecordSet_Clear_Test()
{
    Vault vlt;
    VaultRecordRef vrr;

    vlt.AddKey("A", -1);

    for (int i = 0; i < 1000; ++i)
    {
        for (int j = 0; j < 1000; ++j)
            vlt.CreateRecord({{"A", j}});

        VaultRecordSet vrs;
        vlt.RequestLess("A", i, vrs);

        Starter starter;

        std::thread th1([&]()
        {
            starter.Wait();
            vrs.Reset();
        });

        std::thread th2([&]()
        {
            starter.Wait();
            vlt.DropData();
        });

        th1.join();
        th2.join();
    }
}

void VaultRecordSet_RemoveRecord_Test()
{
    Vault vlt;

    vlt.AddKey("A", -1);

    for (int i = 0; i < 1000; ++i)
    {
        for (int j = 0; j < 1000; ++j)
            vlt.CreateRecord({{"A", j}});

        VaultRecordSet vrs1, vrs2;
        VaultRecordRef vrr;

        vlt.RequestLess("A", i, vrs1);
        vlt.RequestLess("A", i, vrs2);
        vrs1.GetRecord("A", 0, vrr);

        Starter starter;

        std::thread th1([&]()
        {
            starter.Wait();
            vrs1.RemoveRecord(vrr);
        });

        std::thread th2([&]()
        {
            starter.Wait();
            vrs2.RemoveRecord("A", 0);
        });

        std::thread th3([&]()
        {
            starter.Wait();
            vlt.DropData();
        });

        th1.join();
        th2.join();
        th3.join();
    }
}

void VaultRecordSet_RemoveRecords_Test()
{
    Vault vlt;

    vlt.AddKey("A", -1);

    for (int i = 0; i < 1000; ++i)
    {
        for (int j = 0; j < 1000; ++j)
            vlt.CreateRecord({{"A", i}});

        VaultRecordSet vrs;

        vlt.RequestLess("A", i, vrs);

        Starter starter;

        std::thread th1([&]()
        {
            starter.Wait();
            vrs.RemoveRecords("A", i);
        });

        std::thread th2([&]()
        {
            starter.Wait();
            vlt.DropData();
        });

        th1.join();
        th2.join();
    }
}

void VaultRecordSet_Size_Test()
{
    Vault vlt;

    vlt.AddKey("A", -1);

    for (int i = 0; i < 1000; ++i)
    {
        for (int j = 0; j < 1000; ++j)
            vlt.CreateRecord({{"A", j}});

        VaultRecordSet vrs;

        vlt.RequestLess("A", i, vrs);

        Starter starter;

        std::thread th1([&]()
        {
            starter.Wait();
            vrs.Size();
        });

        std::thread th2([&]()
        {
            starter.Wait();
            vlt.DropData();
        });

        th1.join();
        th2.join();
    }
}

void VaultRecordSet_GetKeys_Test()
{
    Vault vlt;

    vlt.AddKey("A", -1);

    for (int i = 0; i < 1000; ++i)
    {
        for (int j = 0; j < 1000; ++j)
            vlt.CreateRecord({{"A", j}});

        VaultRecordSet vrs;

        vlt.RequestLess("A", i, vrs);

        Starter starter;

        std::thread th1([&]()
        {
            starter.Wait();
            vrs.GetKeys();
        });

        std::thread th2([&]()
        {
            starter.Wait();
            vlt.DropData();
        });

        th1.join();
        th2.join();
    }
}

int main()
{
    VaultRecordSet_Copy_Test();
    VaultRecordSet_OperatorComparison_Test();
    VaultRecordSet_GetIsParentVaultValid_Test();
    VaultRecordSet_IsKeyExist_Test();
    VaultRecordSet_GetKeyValue_Test();
    VaultRecordSet_GetKeyType_Test();
    VaultRecordSet_AddRecord_Test();
    VaultRecordSet_GetRecord_Test();
    VaultRecordSet_GetRecords_Test();
    VaultRecordSet_Request_Test();
    VaultRecordSet_Reset_Test();
    VaultRecordSet_Clear_Test();
    VaultRecordSet_RemoveRecord_Test();
    VaultRecordSet_RemoveRecords_Test();
    VaultRecordSet_Size_Test();
    VaultRecordSet_GetKeys_Test();
}