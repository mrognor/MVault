#include "../../Source/MVault.h"

#include "../TestAssert.h"

using namespace mvlt;
using namespace std;

void Vault_AddKey_Test()
{
    Vault vlt;

    // Correct add new key
    TEST_ASSERT(vlt.AddKey("A", -1) == true, "Failed to add new key");

    // Correct try to add exist key
    TEST_ASSERT(vlt.AddKey("A", -1) == false, "Error when try to add key with existing name");
}

void Vault_UpdateKey_Test()
{
    Vault vlt;

    vlt.AddKey("A", -1);
    vlt.AddKey<std::string>("B", "none");

    // Correct key update
    TEST_ASSERT(vlt.UpdateKey("A", 0).IsOperationSuccess == true, "Failed to update key");

    // Wrong key update
    TEST_ASSERT(vlt.UpdateKey("C", -1).ResultCode == VaultOperationResultCode::WrongKey, "Cannot update not existing key");

    // Wrong key type
    TEST_ASSERT(vlt.UpdateKey("B", -1).ResultCode == VaultOperationResultCode::WrongType, "Cannot set incorrect type to new key value");
}

void Vault_IsKeyExist_Test()
{
    Vault vlt;

    vlt.AddKey("A", -1);

    // Exist key check
    TEST_ASSERT(vlt.IsKeyExist("A") == true, "Exist key check failed");
    
    // Not exist key check
    TEST_ASSERT(vlt.IsKeyExist("B") == false, "Not exist key check failed");
}

void Vault_GetKeyValue_Test()
{
    Vault vlt;
    int keyValue;
    std::string wrongKeyValue;

    vlt.AddKey("A", -1);

    // Correct get
    TEST_ASSERT(vlt.GetKeyValue("A", keyValue).ResultCode == VaultOperationResultCode::Success, "Failed to get proper key value");

    // Not existed key
    TEST_ASSERT(vlt.GetKeyValue("B", keyValue).ResultCode == VaultOperationResultCode::WrongKey, "Failed to get not existed key value");

    // Vrong value type
    TEST_ASSERT(vlt.GetKeyValue("A", wrongKeyValue).ResultCode == VaultOperationResultCode::WrongType, "Failed to get key value with wrong type");
}

void Vault_GetKeyType_Test()
{
    Vault vlt;
    std::type_index type = typeid(void);

    vlt.AddKey("A", -1);

    // Correct get
    vlt.GetKeyType("A", type);
    TEST_ASSERT(type == typeid(int), "Failed to get key type");

    // Get not existed key
    TEST_ASSERT(vlt.GetKeyType("B", type) == false, "Failed to get not existed key type");
}

void Vault_GetKeys_Test()
{
    Vault vlt;
    std::vector<std::string> keys;

    // Empty vault
    keys = vlt.GetKeys();
    TEST_ASSERT(keys.empty(), "Failed to get empty vault keys");

    vlt.AddKey("A", -1);

    // One key
    keys = vlt.GetKeys();
    TEST_ASSERT(keys == vector<string>({"A"}), "Failed to get vault key");

    vlt.AddKey("B", -1);
    vlt.AddKey("C", -1);
    vlt.AddKey("D", -1);
    vlt.AddKey("E", -1);
    vlt.AddKey("F", -1);
    vlt.AddKey("G", -1);

    // Many keys
    keys = vlt.GetKeys();
    TEST_ASSERT(keys == vector<string>({"A", "B", "C", "D", "E", "F", "G"}), "Failed to get vault keys");
}

void Vault_RemoveKey_Test()
{
    Vault vlt;
    std::vector<std::string> keys;

    vlt.AddKey("A", -1);
    vlt.AddKey("B", -1);
    vlt.AddKey("C", -1);

    vlt.CreateRecord();

    // Check keys
    keys = vlt.GetKeys();
    TEST_ASSERT(keys == vector<string>({"A", "B", "C"}), "Failed to set vault keys");

    vlt.RemoveKey("B");

    // Check keys
    keys = vlt.GetKeys();
    TEST_ASSERT(keys == vector<string>({"A", "C"}), "Failed to set vault keys");
}

void Vault_CreateRecord_Test()
{
    Vault vlt;

    vlt.AddKey("A", -1);
    vlt.AddKey("B", '0');
    vlt.AddKey("C", true);

    // Empty create record overload
    for (int i = 0; i < 10; ++i) vlt.CreateRecord();
    
    // Check vault size
    TEST_ASSERT(vlt.Size() == 10, "Failed to create records");

    vlt.DropData();

    // Create record with vector of params
    for (int i = 0; i < 10; ++i) vlt.CreateRecord({ {"A", i}, {"B", static_cast<char>('A' + i)} });

    for (int i = 0; i < 10; ++i)
    {
        VaultRecordRef vrr;
        vlt.GetRecord("A", i, vrr);
        char c;
        vrr.GetData("B", c);
        TEST_ASSERT(c == static_cast<char>('A' + i), "Failed to create record");
    }

    vlt.DropData();

    // Create record with record ref and vector of params
    for (int i = 0; i < 10; ++i)
    {
        VaultRecordRef vrr;
        vlt.CreateRecord(vrr, { {"A", i}, {"B", static_cast<char>('A' + i)} });
        char c;
        vrr.GetData("B", c);
        TEST_ASSERT(c == static_cast<char>('A' + i), "Failed to create record");
    }

    // Incorrect creations
    TEST_ASSERT(vlt.CreateRecord({ {"D", 1} }).ResultCode == VaultOperationResultCode::WrongKey, "Failed to create record");
    TEST_ASSERT(vlt.CreateRecord({ {"A", 'a'} }).ResultCode == VaultOperationResultCode::WrongType, "Failed to create record");

    TEST_ASSERT(vlt.CreateRecord({ {"A", 1}, {"D", 1} }).ResultCode == VaultOperationResultCode::WrongKey, "Failed to create record");
    TEST_ASSERT(vlt.CreateRecord({ {"A", 1}, {"A", 'a'} }).ResultCode == VaultOperationResultCode::WrongType, "Failed to create record");

    // Third method overload
    VaultRecordRef vrr1, vrr2;
    vlt.CreateRecord(vrr1, {{"A", 100}});
    vlt.GetRecord("A", 100, vrr2);
    TEST_ASSERT(vrr1.GetRecordUniqueId() == vrr2.GetRecordUniqueId(), "Failed to create record");
}

void Vault_GetRecord_Test()
{
    Vault vlt;
    VaultRecordRef vrr;

    vlt.AddKey("A", -1);
    vlt.AddKey("B", '0');
    vlt.AddKey("C", true);

    // Correct get record on empty vault
    TEST_ASSERT(!vrr.IsValid(), "Error on record creation") 
    TEST_ASSERT(vlt.GetRecord("A", 0, vrr).ResultCode == VaultOperationResultCode::WrongValue, "Error on record creation");

    // Wrong key
    TEST_ASSERT(vlt.GetRecord("D", 0, vrr).ResultCode == VaultOperationResultCode::WrongKey, "Error on get result");
    // Wrong key type
    TEST_ASSERT(vlt.GetRecord("A", std::string(), vrr).ResultCode == VaultOperationResultCode::WrongType, "Error on get result");

    vlt.CreateRecord();
    
    // Correct get record
    TEST_ASSERT(vlt.GetRecord("A", -1, vrr).ResultCode == VaultOperationResultCode::Success, "Error on record creation");

    // Wrong value
    TEST_ASSERT(vlt.GetRecord("A", 0, vrr).ResultCode == VaultOperationResultCode::WrongValue, "Error on get result");
    // Wrong key
    TEST_ASSERT(vlt.GetRecord("D", 0, vrr).ResultCode == VaultOperationResultCode::WrongKey, "Error on get result");
    // Wrong key type
    TEST_ASSERT(vlt.GetRecord("A", std::string(), vrr).ResultCode == VaultOperationResultCode::WrongType, "Error on get result");

    // Create records with vector of params
    for (int i = 0; i < 10; ++i) vlt.CreateRecord({ {"A", i}, {"B", static_cast<char>('A' + i)} });

    // Check records
    for (int i = 0; i < 10; ++i)
    {
        VaultRecordRef vrrl;
        vlt.GetRecord("A", i, vrrl);
        char c;
        vrrl.GetData("B", c);
        TEST_ASSERT(c == static_cast<char>('A' + i), "Failed to create record");
    }
}

void Vault_GetRecords_Test()
{
    Vault vlt;
    VaultRecordRef vrr;
    std::vector<VaultRecordRef> vec;

    vlt.AddKey("A", -1);
    vlt.AddKey("B", '0');
    vlt.AddKey("C", true);

    // Correct get records on empty vault
    TEST_ASSERT(vec.size() == 0, "Error on record creation") 
    TEST_ASSERT(vlt.GetRecords("A", 0, vec).ResultCode == VaultOperationResultCode::WrongValue, "Error on record creation");

    // Wrong key
    TEST_ASSERT(vlt.GetRecords("D", 0, vec).ResultCode == VaultOperationResultCode::WrongKey, "Error on get result");
    // Wrong key type
    TEST_ASSERT(vlt.GetRecords("A", std::string(), vec).ResultCode == VaultOperationResultCode::WrongType, "Error on get result");

    // Fill vault
    for (int i = 0; i < 10; ++i) vlt.CreateRecord({ {"A", i}, });

    // Get records
    vlt.GetRecords("B", '0', vec);

    for (int i = 0; i < 10; ++i)
    {
        int n;
        TEST_ASSERT(vec[i].GetData("A", n).ResultCode == VaultOperationResultCode::Success, "Error on get records");
    }

    // Get only 2 records
    vlt.GetRecords("B", '0', vec, 2);
    TEST_ASSERT(vec.size() == 2, "Wrong vec size");
}

void Vault_Request_Tests()
{
    Vault vlt;
    VaultRecordRef vrr;
    VaultRecordSet vrs;

    vlt.AddKey("A", -1);
    vlt.AddKey("B", -1);
    vlt.AddKey<std::string>("C", "-1");

    // Correct request on empty vault
    TEST_ASSERT(vlt.Request(Equal("A", -1), vrs).ResultCode == VaultOperationResultCode::Success, "Error on record creation");

    // Incorrect requests
    // Wrong key
    TEST_ASSERT(vlt.Request(Equal("D", -1), vrs).ResultCode == VaultOperationResultCode::WrongKey, "Error on get result");
    // Wrong key type
    TEST_ASSERT(vlt.Request(Equal("A", std::string()), vrs).ResultCode == VaultOperationResultCode::WrongType, "Error on get result");

    // Fill vault
    vlt.CreateRecord({ {"A", 0}, {"C", std::to_string(10)}});
    for (int i = 0; i < 10; ++i) vlt.CreateRecord({ {"A", i}, {"C", std::to_string(10 - i)}});
    vlt.CreateRecord({ {"A", 9}, {"C", std::to_string(1)}});

    //  Correct requests

    // Equal
    {
        vlt.Request(Equal("A", 2), vrs);
        TEST_ASSERT(vrs.Size() == 1, "Error on record request");

        vlt.Request(Equal("B", -1), vrs);
        TEST_ASSERT(vrs.Size() == 12, "Error on record request");

        vlt.Request(Equal("C", std::string("a")), vrs);
        TEST_ASSERT(vrs.Size() == 0, "Error on record request");
    }

    // Less
    {
        vlt.Request(Less("A", 1), vrs);
        TEST_ASSERT(vrs.Size() == 2, "Error on record request");

        vlt.Request(Less("A", 0), vrs);
        TEST_ASSERT(vrs.Size() == 0, "Error on record request");

        vlt.Request(Less("A", 15), vrs);
        TEST_ASSERT(vrs.Size() == 12, "Error on record request");

        vlt.Request(Less("A", 10), vrs);
        TEST_ASSERT(vrs.Size() == 12, "Error on record request");

        vlt.Request(Less("A", 9), vrs);
        TEST_ASSERT(vrs.Size() == 10, "Error on record request");

        vlt.Request(Less("B", 1), vrs);
        TEST_ASSERT(vrs.Size() == 12, "Error on record request");

        vlt.Request(Less("B", -2), vrs);
        TEST_ASSERT(vrs.Size() == 0, "Error on record request");
    }

    // LessOrEqual
    {
        vlt.Request(LessOrEqual("A", 1), vrs);
        TEST_ASSERT(vrs.Size() == 3, "Error on record request");

        vlt.Request(LessOrEqual("A", 0), vrs);
        TEST_ASSERT(vrs.Size() == 2, "Error on record request");

        vlt.Request(LessOrEqual("A", 15), vrs);
        TEST_ASSERT(vrs.Size() == 12, "Error on record request");

        vlt.Request(LessOrEqual("A", 9), vrs);
        TEST_ASSERT(vrs.Size() == 12, "Error on record request");

        vlt.Request(LessOrEqual("B", 1), vrs);
        TEST_ASSERT(vrs.Size() == 12, "Error on record request");

        vlt.Request(LessOrEqual("B", -1), vrs);
        TEST_ASSERT(vrs.Size() == 12, "Error on record request");

        vlt.Request(LessOrEqual("B", -2), vrs);
        TEST_ASSERT(vrs.Size() == 0, "Error on record request");
    }

    // Greater
    {
        vlt.Request(Greater("A", 8), vrs);
        TEST_ASSERT(vrs.Size() == 2, "Error on record request");

        vlt.Request(Greater("A", 9), vrs);
        TEST_ASSERT(vrs.Size() == 0, "Error on record request");

        vlt.Request(Greater("A", -1), vrs);
        TEST_ASSERT(vrs.Size() == 12, "Error on record request");

        vlt.Request(Greater("A", 0), vrs);
        TEST_ASSERT(vrs.Size() == 10, "Error on record request");

        vlt.Request(Greater("B", -1), vrs);
        TEST_ASSERT(vrs.Size() == 0, "Error on record request");

        vlt.Request(Greater("B", -2), vrs);
        TEST_ASSERT(vrs.Size() == 12, "Error on record request");
    }

    // GreaterOrEqual
    {
        vlt.Request(GreaterOrEqual("A", 8), vrs);
        TEST_ASSERT(vrs.Size() == 3, "Error on record request");

        vlt.Request(GreaterOrEqual("A", 9), vrs);
        TEST_ASSERT(vrs.Size() == 2, "Error on record request");

        vlt.Request(GreaterOrEqual("A", -1), vrs);
        TEST_ASSERT(vrs.Size() == 12, "Error on record request");

        vlt.Request(GreaterOrEqual("A", 0), vrs);
        TEST_ASSERT(vrs.Size() == 12, "Error on record request");

        vlt.Request(GreaterOrEqual("B", 0), vrs);
        TEST_ASSERT(vrs.Size() == 0, "Error on record request");

        vlt.Request(GreaterOrEqual("B", -1), vrs);
        TEST_ASSERT(vrs.Size() == 12, "Error on record request");

        vlt.Request(GreaterOrEqual("B", -2), vrs);
        TEST_ASSERT(vrs.Size() == 12, "Error on record request");
    }

    // Interval
    {
        vlt.RequestInterval("A", -1, 10, vrs, true, true);
        TEST_ASSERT(vrs.Size() == 12, "Error on record request");

        vlt.RequestInterval("A", -1, 10, vrs, true, false);
        TEST_ASSERT(vrs.Size() == 12, "Error on record request");

        vlt.RequestInterval("A", -1, 10, vrs, false, true);
        TEST_ASSERT(vrs.Size() == 12, "Error on record request");

        vlt.RequestInterval("A", -1, 10, vrs, false, false);
        TEST_ASSERT(vrs.Size() == 12, "Error on record request");


        vlt.RequestInterval("A", 0, 10, vrs, true, true);
        TEST_ASSERT(vrs.Size() == 12, "Error on record request");

        vlt.RequestInterval("A", 0, 10, vrs, true, false);
        TEST_ASSERT(vrs.Size() == 12, "Error on record request");

        vlt.RequestInterval("A", 0, 10, vrs, false, true);
        TEST_ASSERT(vrs.Size() == 10, "Error on record request");

        vlt.RequestInterval("A", 0, 10, vrs, false, false);
        TEST_ASSERT(vrs.Size() == 10, "Error on record request");


        vlt.RequestInterval("A", -1, 9, vrs, true, true);
        TEST_ASSERT(vrs.Size() == 12, "Error on record request");

        vlt.RequestInterval("A", -1, 9, vrs, true, false);
        TEST_ASSERT(vrs.Size() == 10, "Error on record request");

        vlt.RequestInterval("A", -1, 9, vrs, false, true);
        TEST_ASSERT(vrs.Size() == 12, "Error on record request");

        vlt.RequestInterval("A", -1, 9, vrs, false, false);
        TEST_ASSERT(vrs.Size() == 10, "Error on record request");


        vlt.RequestInterval("A", 0, 9, vrs, true, true);
        TEST_ASSERT(vrs.Size() == 12, "Error on record request");

        vlt.RequestInterval("A", 0, 9, vrs, true, false);
        TEST_ASSERT(vrs.Size() == 10, "Error on record request");

        vlt.RequestInterval("A", 0, 9, vrs, false, true);
        TEST_ASSERT(vrs.Size() == 10, "Error on record request");

        vlt.RequestInterval("A", 0, 9, vrs, false, false);
        TEST_ASSERT(vrs.Size() == 8, "Error on record request");
    }

    // Request 0 records
    TEST_ASSERT(vlt.RequestEqual("B", -1, vrs, 0).ResultCode == VaultOperationResultCode::Success, "Error on record request");
    TEST_ASSERT(vrs.Size() == 0, "Error on record request");

    // Request 2 records
    TEST_ASSERT(vlt.RequestEqual("B", -1, vrs, 2).ResultCode == VaultOperationResultCode::Success, "Error on record request");
    TEST_ASSERT(vrs.Size() == 2, "Error on record request");

    VaultOperationResult res;
    // Predicat test. B == -1 and A = stoi(C)
    res = vlt.RequestEqual("B", -1, vrs, -1, [](const VaultRecordRef& ref)
        {
            int a; std::string c;
            ref.GetData("A", a);
            ref.GetData("C", c);
            if (a == stoi(c)) return true;
            else return false;
        });
    TEST_ASSERT(res.ResultCode == VaultOperationResultCode::Success, "Error on record request");
    TEST_ASSERT(vrs.Size() == 1, "Error on record request");
    vrs.GetRecord("B", -1, vrr);
    int n;
    vrr.GetData("A", n);
    TEST_ASSERT(n == 5, "Failed to make request");
}

void Vault_DropVault_Tests()
{
    Vault vlt;

    vlt.AddKey("A", -1);
    vlt.AddKey("B", '0');
    vlt.AddKey("C", true);

    // Fill vault
    for (int i = 0; i < 10; ++i) vlt.CreateRecord({ {"A", i} });

    vlt.DropVault();

    TEST_ASSERT(vlt.GetKeys().size() == 0, "Failed to drop vault");
    TEST_ASSERT(vlt.Size() == 0, "Failed to drop vault");
}

void Vault_DropData_Tests()
{
    Vault vlt;

    vlt.AddKey("A", -1);
    vlt.AddKey("B", '0');
    vlt.AddKey("C", true);

    // Fill vault
    for (int i = 0; i < 10; ++i) vlt.CreateRecord({ {"A", i} });

    vlt.DropData();

    TEST_ASSERT(vlt.GetKeys().size() == 3, "Failed to drop vault");
    TEST_ASSERT(vlt.Size() == 0, "Failed to drop vault");
}

void Vault_EraseRecord_Test()
{
    // VaultRecordRef overload
    Vault vlt;
    VaultRecordRef vrr;

    // Erase empty record
    TEST_ASSERT(vlt.EraseRecord(vrr) == false, "Failed to erase record");

    vlt.AddKey("A", -1);

    // Fill vault
    for (int i = 0; i < 10; ++i) vlt.CreateRecord({ {"A", i} });

    vlt.GetRecord("A", 0, vrr);
    TEST_ASSERT(vlt.EraseRecord(vrr), "Failed to erase record");
    TEST_ASSERT(vlt.Size() == 9, "Failed to erase record");
    
    TEST_ASSERT(vlt.EraseRecord(vrr) == false, "Failed to erase record");

    vlt.GetRecord("A", 1, vrr);
    vrr.Reset();
    TEST_ASSERT(vlt.EraseRecord(vrr) == false, "Failed to delete record");
    TEST_ASSERT(vlt.Size() == 9, "Failed to erase record");

    // Key and value overload

    // Incorrect erasing
    // Wrong key
    TEST_ASSERT(vlt.EraseRecord("B", 1).ResultCode == VaultOperationResultCode::WrongKey, "Error on erase");
    // Wrong key type
    TEST_ASSERT(vlt.EraseRecord("A", 2.9).ResultCode == VaultOperationResultCode::WrongType, "Error on erase");

    // No value erasing
    TEST_ASSERT(vlt.EraseRecord("A", 0).ResultCode == VaultOperationResultCode::WrongValue, "Error on erase");

    // Correct erasing
    for (int i = 1; i < 10; ++i)
        TEST_ASSERT(vlt.EraseRecord("A", i).ResultCode == VaultOperationResultCode::Success, "Error on erase");

    vlt.CreateRecord();
    vlt.CreateRecord();

    vlt.EraseRecord("A", -1);
    TEST_ASSERT(vlt.Size() == 1, "Failed to erase record");
}

void Vault_EraseRecords_Test()
{
    Vault vlt;
    VaultRecordRef vrr;

    vlt.AddKey("A", -1);

    // Wrong key
    TEST_ASSERT(vlt.EraseRecords("B", 1).ResultCode == VaultOperationResultCode::WrongKey, "Error on erase");
    // Wrong key type
    TEST_ASSERT(vlt.EraseRecords("A", 2.9).ResultCode == VaultOperationResultCode::WrongType, "Error on erase");
    // No value erasing
    TEST_ASSERT(vlt.EraseRecords("A", 0).ResultCode == VaultOperationResultCode::WrongValue, "Error on erase");

    // Fill vault
    for (int i = 0; i < 10; ++i) vlt.CreateRecord();
    vlt.CreateRecord({ {"A", 1} });

    // Erase 2 records
    TEST_ASSERT(vlt.EraseRecords("A", -1, 2).ResultCode == VaultOperationResultCode::Success, "Failed to erase records");
    TEST_ASSERT(vlt.Size() == 9, "Failed to erase records");

    // Erase all records
    TEST_ASSERT(vlt.EraseRecords("A", -1).ResultCode == VaultOperationResultCode::Success, "Failed to erase records");
    TEST_ASSERT(vlt.Size() == 1, "Failed to erase records");
}

void Vault_Size_Test()
{
    Vault vlt;
    VaultRecordRef vrr;

    vlt.AddKey<std::size_t>("A", -1);

    for (std::size_t i = 1; i < 11; ++i)
    {
        vlt.CreateRecord({ {"A", i} });
        TEST_ASSERT(vlt.Size() == i, "Failed to create record");
    }

    for (std::size_t i = 10; i > 0; --i)
    {
        vlt.EraseRecord("A", i);
        TEST_ASSERT(vlt.Size() == i - 1, "Failed to create record");
    }
}

void Vault_Destructor_Test()
{
    Vault* vlt = new Vault;

    vlt->AddKey("A", -1);
    vlt->CreateRecord();

    delete vlt;
    vlt = new Vault;
    delete vlt;
}

int main()
{
    Vault_AddKey_Test();
    Vault_UpdateKey_Test();
    Vault_IsKeyExist_Test();
    Vault_GetKeyValue_Test();
    Vault_GetKeyType_Test();
    Vault_GetKeys_Test();
    Vault_RemoveKey_Test();
    Vault_CreateRecord_Test();
    Vault_GetRecord_Test();
    Vault_GetRecords_Test();
    Vault_Request_Tests();
    Vault_DropVault_Tests();
    Vault_DropData_Tests();
    Vault_EraseRecord_Test();
    Vault_EraseRecords_Test();
    Vault_Size_Test();
    Vault_Destructor_Test();
}