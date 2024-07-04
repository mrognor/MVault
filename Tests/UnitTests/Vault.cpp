#include "../../Source/MVault.h"

#include "../TestAssert.h"

using namespace mvlt;
using namespace std;

void Vault_AddKey_Test()
{
    Vault vlt;
    bool res;

    // Correct add new key
    res = vlt.AddKey("A", -1);
    TEST_ASSERT(res == true, "Failed to add new key");

    // Correct try to add exist key
    res = vlt.AddKey("A", -1);
    TEST_ASSERT(res == false, "Error when try to add key with existing name");
}

void Vault_UpdateKey_Test()
{
    Vault vlt;
    VaultOperationResult res;

    vlt.AddKey("A", -1);
    vlt.AddKey<std::string>("B", "none");

    // Correct key update
    res = vlt.UpdateKey("A", 0);
    TEST_ASSERT(res.IsOperationSuccess == true, "Failed to update key");

    // Wrong key update
    res = vlt.UpdateKey("C", -1);
    TEST_ASSERT(res.ResultCode == VaultOperationResultCode::WrongKey, "Cannot update not existing key");

    // Wrong key type
    res = vlt.UpdateKey("B", -1);
    TEST_ASSERT(res.ResultCode == VaultOperationResultCode::WrongType, "Cannot set incorrect type to new key value");
}

void Vault_IsKeyExist_Test()
{
    Vault vlt;
    bool res;

    vlt.AddKey("A", -1);

    // Exist key check
    res = vlt.IsKeyExist("A");
    TEST_ASSERT(res == true, "Exist key check failed");
    
    // Not exist key check
    res = vlt.IsKeyExist("B");
    TEST_ASSERT(res == false, "Not exist key check failed");
}

void Vault_GetKeyValue_Test()
{
    Vault vlt;
    VaultOperationResult res;
    int keyValue;
    std::string wrongKeyValue;

    vlt.AddKey("A", -1);

    // Correct get
    res = vlt.GetKeyValue("A", keyValue);
    TEST_ASSERT(res.ResultCode == VaultOperationResultCode::Success, "Failed to get proper key value");

    // Not existed key
    res = vlt.GetKeyValue("B", keyValue);
    TEST_ASSERT(res.ResultCode == VaultOperationResultCode::WrongKey, "Failed to get not existed key value");

    // Vrong value type
    res = vlt.GetKeyValue("A", wrongKeyValue);
    TEST_ASSERT(res.ResultCode == VaultOperationResultCode::WrongType, "Failed to get key value with wrong type");
}

void Vault_GetKeyType_Test()
{
    Vault vlt;
    bool res;
    std::type_index type = typeid(void);

    vlt.AddKey("A", -1);

    // Correct get
    vlt.GetKeyType("A", type);
    TEST_ASSERT(type == typeid(int), "Failed to get key type");

    // Get not existed key
    res = vlt.GetKeyType("B", type);
    TEST_ASSERT(res == false, "Failed to get not existed key type");
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
    VaultOperationResult res;

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
    res = vlt.CreateRecord({ {"D", 1} });
    TEST_ASSERT(res.ResultCode == VaultOperationResultCode::WrongKey, "Failed to create record");
    res = vlt.CreateRecord({ {"A", 'a'} });
    TEST_ASSERT(res.ResultCode == VaultOperationResultCode::WrongType, "Failed to create record");

    res = vlt.CreateRecord({ {"A", 1}, {"D", 1} });
    TEST_ASSERT(res.ResultCode == VaultOperationResultCode::WrongKey, "Failed to create record");
    res = vlt.CreateRecord({ {"A", 1}, {"A", 'a'} });
    TEST_ASSERT(res.ResultCode == VaultOperationResultCode::WrongType, "Failed to create record");

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
    VaultOperationResult res;

    vlt.AddKey("A", -1);
    vlt.AddKey("B", '0');
    vlt.AddKey("C", true);

    // Correct get record on empty vault
    res = vlt.GetRecord("A", 0, vrr);
    TEST_ASSERT(!vrr.IsValid(), "Error on record creation") 
    TEST_ASSERT(res.ResultCode == VaultOperationResultCode::WrongValue, "Error on record creation");

    res = vlt.GetRecord("D", 0, vrr); // Wrong key
    TEST_ASSERT(res.ResultCode == VaultOperationResultCode::WrongKey, "Error on get result");
    res = vlt.GetRecord("A", std::string(), vrr); // Wrong key type
    TEST_ASSERT(res.ResultCode == VaultOperationResultCode::WrongType, "Error on get result");

    vlt.CreateRecord();
    
    // Correct get record
    res = vlt.GetRecord("A", -1, vrr);
    TEST_ASSERT(res.ResultCode == VaultOperationResultCode::Success, "Error on record creation");

    res = vlt.GetRecord("A", 0, vrr); // Wrong value
    TEST_ASSERT(res.ResultCode == VaultOperationResultCode::WrongValue, "Error on get result");
    res = vlt.GetRecord("D", 0, vrr); // Wrong key
    TEST_ASSERT(res.ResultCode == VaultOperationResultCode::WrongKey, "Error on get result");
    res = vlt.GetRecord("A", std::string(), vrr); // Wrong key type
    TEST_ASSERT(res.ResultCode == VaultOperationResultCode::WrongType, "Error on get result");

    // Create records with vector of params
    for (int i = 0; i < 10; ++i) vlt.CreateRecord({ {"A", i}, {"B", static_cast<char>('A' + i)} });

    // Check records
    for (int i = 0; i < 10; ++i)
    {
        VaultRecordRef vrr;
        vlt.GetRecord("A", i, vrr);
        char c;
        vrr.GetData("B", c);
        TEST_ASSERT(c == static_cast<char>('A' + i), "Failed to create record");
    }
}

void Vault_GetRecords_Test()
{
    Vault vlt;
    VaultRecordRef vrr;
    VaultOperationResult res;
    std::vector<VaultRecordRef> vec;

    vlt.AddKey("A", -1);
    vlt.AddKey("B", '0');
    vlt.AddKey("C", true);

    // Correct get records on empty vault
    res = vlt.GetRecords("A", 0, vec);
    TEST_ASSERT(vec.size() == 0, "Error on record creation") 
    TEST_ASSERT(res.ResultCode == VaultOperationResultCode::WrongValue, "Error on record creation");

    res = vlt.GetRecords("D", 0, vec); // Wrong key
    TEST_ASSERT(res.ResultCode == VaultOperationResultCode::WrongKey, "Error on get result");
    res = vlt.GetRecords("A", std::string(), vec); // Wrong key type
    TEST_ASSERT(res.ResultCode == VaultOperationResultCode::WrongType, "Error on get result");

    // Fill vault
    for (int i = 0; i < 10; ++i) vlt.CreateRecord({ {"A", i}, });

    // Get reqords
    vlt.GetRecords("B", '0', vec);

    for (int i = 0; i < 10; ++i)
    {
        int n;
        res = vec[i].GetData("A", n);
        TEST_ASSERT(res.ResultCode == VaultOperationResultCode::Success, "Error on get records");
    }

    // Get only 2 records
    vlt.GetRecords("B", '0', vec, 2);
    TEST_ASSERT(vec.size() == 2, "Wrong vec size");

    res = vlt.GetRecords("D", 0, vec); // Wrong key
    TEST_ASSERT(res.ResultCode == VaultOperationResultCode::WrongKey, "Error on get result");
    res = vlt.GetRecords("A", std::string(), vec); // Wrong key type
    TEST_ASSERT(res.ResultCode == VaultOperationResultCode::WrongType, "Error on get result");
}

void Vault_RequestEqual_Tests()
{
    Vault vlt;
    VaultRecordRef vrr;
    VaultOperationResult res;
    VaultRecordSet vrs;

    vlt.AddKey("A", -1);
    vlt.AddKey("B", -1);
    vlt.AddKey<std::string>("C", "-1");

    // Correct get records on empty vault
    res = vlt.RequestEqual("A", -1, vrs);
    TEST_ASSERT(vrs.Size() == 0, "Error on record creation") 
    TEST_ASSERT(res.ResultCode == VaultOperationResultCode::Success, "Error on record creation");

    res = vlt.RequestEqual("D", 0, vrs); // Wrong key
    TEST_ASSERT(res.ResultCode == VaultOperationResultCode::WrongKey, "Error on get result");
    res = vlt.RequestEqual("A", std::string(), vrs); // Wrong key type
    TEST_ASSERT(res.ResultCode == VaultOperationResultCode::WrongType, "Error on get result");

    // Fill vault
    for (int i = 0; i < 10; ++i) vlt.CreateRecord({ {"A", i}, {"C", std::to_string(10 - i)}});

    // Simple requests
    res = vlt.RequestEqual<std::string>("C", "2", vrs);
    TEST_ASSERT(res.ResultCode == VaultOperationResultCode::Success, "Error on record request");
    TEST_ASSERT(vrs.Size() == 1, "Error on record request");

    res = vlt.RequestEqual("B", -1, vrs);
    TEST_ASSERT(res.ResultCode == VaultOperationResultCode::Success, "Error on record request");
    TEST_ASSERT(vrs.Size() == 10, "Error on record request");

    // Request 0 records
    res = vlt.RequestEqual("B", -1, vrs, 0);
    TEST_ASSERT(res.ResultCode == VaultOperationResultCode::Success, "Error on record request");
    TEST_ASSERT(vrs.Size() == 0, "Error on record request");

    // Request 2 records
    res = vlt.RequestEqual("B", -1, vrs, 2);
    TEST_ASSERT(res.ResultCode == VaultOperationResultCode::Success, "Error on record request");
    TEST_ASSERT(vrs.Size() == 2, "Error on record request");

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

void Vault_RequestGreater_Tests()
{
    Vault vlt;
    VaultRecordRef vrr;
    VaultOperationResult res;
    VaultRecordSet vrs;

    vlt.AddKey("A", -1);
    vlt.AddKey("B", -1);
    vlt.AddKey<std::string>("C", "-1");

    // Correct get records on empty vault
    res = vlt.RequestGreater("A", -1, vrs);
    TEST_ASSERT(vrs.Size() == 0, "Error on record creation") 
    TEST_ASSERT(res.ResultCode == VaultOperationResultCode::Success, "Error on record creation");

    res = vlt.RequestGreater("D", 0, vrs); // Wrong key
    TEST_ASSERT(res.ResultCode == VaultOperationResultCode::WrongKey, "Error on get result");
    res = vlt.RequestGreater("A", std::string(), vrs); // Wrong key type
    TEST_ASSERT(res.ResultCode == VaultOperationResultCode::WrongType, "Error on get result");

    // Fill vault
    for (int i = 0; i < 10; ++i) vlt.CreateRecord({ {"A", i}, {"C", std::to_string(10 - i)}});

    // Simple requests
    res = vlt.RequestGreater<std::string>("C", "8", vrs);
    TEST_ASSERT(res.ResultCode == VaultOperationResultCode::Success, "Error on record request");
    TEST_ASSERT(vrs.Size() == 1, "Error on record request");

    res = vlt.RequestGreater("B", -2, vrs);
    TEST_ASSERT(res.ResultCode == VaultOperationResultCode::Success, "Error on record request");
    TEST_ASSERT(vrs.Size() == 10, "Error on record request");

    // Request 0 records
    res = vlt.RequestGreater("B", -2, vrs, 0);
    TEST_ASSERT(res.ResultCode == VaultOperationResultCode::Success, "Error on record request");
    TEST_ASSERT(vrs.Size() == 0, "Error on record request");

    // Request 2 records
    res = vlt.RequestGreater("B", -2, vrs, 2);
    TEST_ASSERT(res.ResultCode == VaultOperationResultCode::Success, "Error on record request");
    TEST_ASSERT(vrs.Size() == 2, "Error on record request");

    // Predicat test. A > 4 and A = stoi(C)
    res = vlt.RequestGreater("A", 4, vrs, -1, [](const VaultRecordRef& ref)
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

void Vault_RequestGreaterOrEqual_Tests()
{
    Vault vlt;
    VaultRecordRef vrr;
    VaultOperationResult res;
    VaultRecordSet vrs;

    vlt.AddKey("A", -1);
    vlt.AddKey("B", -1);
    vlt.AddKey<std::string>("C", "-1");

    // Correct get records on empty vault
    res = vlt.RequestGreaterOrEqual("A", -1, vrs);
    TEST_ASSERT(vrs.Size() == 0, "Error on record creation") 
    TEST_ASSERT(res.ResultCode == VaultOperationResultCode::Success, "Error on record creation");

    res = vlt.RequestGreaterOrEqual("D", 0, vrs); // Wrong key
    TEST_ASSERT(res.ResultCode == VaultOperationResultCode::WrongKey, "Error on get result");
    res = vlt.RequestGreaterOrEqual("A", std::string(), vrs); // Wrong key type
    TEST_ASSERT(res.ResultCode == VaultOperationResultCode::WrongType, "Error on get result");

    // Fill vault
    for (int i = 0; i < 10; ++i) vlt.CreateRecord({ {"A", i}, {"C", std::to_string(10 - i)}});

    // Simple requests
    res = vlt.RequestGreaterOrEqual<std::string>("C", "9", vrs);
    TEST_ASSERT(res.ResultCode == VaultOperationResultCode::Success, "Error on record request");
    TEST_ASSERT(vrs.Size() == 1, "Error on record request");

    res = vlt.RequestGreaterOrEqual("B", -1, vrs);
    TEST_ASSERT(res.ResultCode == VaultOperationResultCode::Success, "Error on record request");
    TEST_ASSERT(vrs.Size() == 10, "Error on record request");

    // Request 0 records
    res = vlt.RequestGreaterOrEqual("B", -1, vrs, 0);
    TEST_ASSERT(res.ResultCode == VaultOperationResultCode::Success, "Error on record request");
    TEST_ASSERT(vrs.Size() == 0, "Error on record request");

    // Request 2 records
    res = vlt.RequestGreaterOrEqual("B", -1, vrs, 2);
    TEST_ASSERT(res.ResultCode == VaultOperationResultCode::Success, "Error on record request");
    TEST_ASSERT(vrs.Size() == 2, "Error on record request");

    // Predicat test. A >= 5 and A = stoi(C)
    res = vlt.RequestGreaterOrEqual("A", 5, vrs, -1, [](const VaultRecordRef& ref)
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

void Vault_RequestLess_Tests()
{
    Vault vlt;
    VaultRecordRef vrr;
    VaultOperationResult res;
    VaultRecordSet vrs;

    vlt.AddKey("A", -1);
    vlt.AddKey("B", -1);
    vlt.AddKey<std::string>("C", "-1");

    // Correct get records on empty vault
    res = vlt.RequestLess("A", -1, vrs);
    TEST_ASSERT(vrs.Size() == 0, "Error on record creation") 
    TEST_ASSERT(res.ResultCode == VaultOperationResultCode::Success, "Error on record creation");

    res = vlt.RequestLess("D", 0, vrs); // Wrong key
    TEST_ASSERT(res.ResultCode == VaultOperationResultCode::WrongKey, "Error on get result");
    res = vlt.RequestLess("A", std::string(), vrs); // Wrong key type
    TEST_ASSERT(res.ResultCode == VaultOperationResultCode::WrongType, "Error on get result");

    // Fill vault
    for (int i = 0; i < 10; ++i) vlt.CreateRecord({ {"A", i}, {"C", std::to_string(10 - i)}});

    // Simple requests
    res = vlt.RequestLess<std::string>("C", "1", vrs);
    TEST_ASSERT(res.ResultCode == VaultOperationResultCode::Success, "Error on record request");
    TEST_ASSERT(vrs.Size() == 1, "Error on record request");

    res = vlt.RequestLess("B", 0, vrs);
    TEST_ASSERT(res.ResultCode == VaultOperationResultCode::Success, "Error on record request");
    TEST_ASSERT(vrs.Size() == 10, "Error on record request");

    // Request 0 records
    res = vlt.RequestLess("B", 0, vrs, 0);
    TEST_ASSERT(res.ResultCode == VaultOperationResultCode::Success, "Error on record request");
    TEST_ASSERT(vrs.Size() == 0, "Error on record request");

    // Request 2 records
    res = vlt.RequestLess("B", 0, vrs, 2);
    TEST_ASSERT(res.ResultCode == VaultOperationResultCode::Success, "Error on record request");
    TEST_ASSERT(vrs.Size() == 2, "Error on record request");

    // Predicat test. A < 6 and A = stoi(C)
    res = vlt.RequestLess("A", 6, vrs, -1, [](const VaultRecordRef& ref)
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
    Vault_RequestEqual_Tests();
    Vault_RequestGreater_Tests();
    Vault_RequestGreaterOrEqual_Tests();
    Vault_RequestLess_Tests();
}