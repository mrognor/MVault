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

    // Incorrect adding
    res = vlt.AddKey("", -1);
    TEST_ASSERT(res == false, "Error when try to add key with empty name");
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
}