#include "../../Source/MVault.h"

#include "../TestAssert.h"

using namespace mvlt;
using namespace std;

void VaultRecordSet_CopyConstructor_Test()
{
    Vault vlt;
    VaultRecordSet vrs1;

    vlt.AddKey("A", 1);
    vlt.AddKey<std::string>("B", "");
    vlt.AddKey("C", true);

    for (int i = 1; i < 11; ++i) vlt.CreateRecord({ {"A", i} });

    vlt.Request(Greater("A", 3) && Less("A", 7), vrs1);

    VaultRecordSet vrs2(vrs1);

    TEST_ASSERT(vrs2 == vrs1, "Failed to copy vault record set");
}

void VaultRecordSet_OperatorAssignment_Test()
{
    Vault vlt;
    VaultRecordSet vrs1;

    vlt.AddKey("A", 1);
    vlt.AddKey<std::string>("B", "");
    vlt.AddKey("C", true);

    for (int i = 1; i < 11; ++i) vlt.CreateRecord({ {"A", i} });

    vlt.Request(Greater("A", 3) && Less("A", 7), vrs1);

    VaultRecordSet vrs2 = vrs1;

    TEST_ASSERT(vrs2 == vrs1, "Failed to assign vault record set");
}

void VaultRecordSet_OperatorComparison_Test()
{
    Vault vlt;
    VaultRecordSet vrs1, vrs2;

    vlt.AddKey("A", 1);
    vlt.AddKey<std::string>("B", "");
    vlt.AddKey("C", true);

    for (int i = 1; i < 11; ++i) vlt.CreateRecord({ {"A", i} });

    vlt.Request(Greater("A", 3) && Less("A", 7), vrs1);
    vlt.Request(Greater("A", 3) && Less("A", 7), vrs2);

    TEST_ASSERT(vrs2 == vrs1, "Failed to compare vault record set");
}

void VaultRecordSet_GetIsParentVaultValid_Test()
{
    Vault* vlt = new Vault;
    VaultRecordSet vrs;

    vlt->AddKey("A", -1);
    vlt->CreateRecord();
    vlt->Request(Equal("A", -1), vrs);

    TEST_ASSERT(vrs.GetIsParentVaultValid() == true, "Failed to request");
    delete vlt;
    TEST_ASSERT(vrs.GetIsParentVaultValid() == false, "Failed to invalidate parent vault");
    
    vlt = new Vault;

    vlt->AddKey("A", -1);
    vlt->CreateRecord();
    vlt->Request(Equal("A", -1), vrs);

    TEST_ASSERT(vrs.GetIsParentVaultValid() == true, "Failed to request");
    vlt->DropData();
    TEST_ASSERT(vrs.GetIsParentVaultValid() == true, "Failed to invalidate parent vault");
    vlt->DropVault();
    TEST_ASSERT(vrs.GetIsParentVaultValid() == false, "Failed to invalidate parent vault");

    delete vlt;
}

void VaultRecordSet_GetParentVaultUniqueId_Test()
{
    Vault vlt;
    VaultRecordSet vrs;

    TEST_ASSERT(vrs.GetParentVaultUniqueId() == "null", "Error on VaultRecordSet creation");
    vlt.AddKey("A", 1);
    vlt.AddKey<std::string>("B", "");
    vlt.AddKey("C", true);

    for (int i = 1; i < 11; ++i) vlt.CreateRecord({ {"A", i} });

    vlt.Request(Greater("A", 3) && Less("A", 7), vrs);

    TEST_ASSERT(vrs.GetParentVaultUniqueId() != "null", "Failed to request vault record set");
}

void VaultRecordSet_IsKeyExist_Test()
{
    Vault vlt;

    VaultRecordSet vrs;

    TEST_ASSERT(vrs.IsKeyExist("A") == false, "Exist key check failed");

    vlt.AddKey("A", -1);
    vlt.CreateRecord();

    vlt.Request(Equal("A", -1), vrs);

    // Exist key check
    TEST_ASSERT(vrs.IsKeyExist("A") == true, "Exist key check failed");
    
    // Not exist key check
    TEST_ASSERT(vrs.IsKeyExist("B") == false, "Not exist key check failed");

    // Dynamic key add
    vlt.AddKey("B", -1);

    TEST_ASSERT(vrs.IsKeyExist("B") == true, "Exist key check failed");
}

void VaultRecordSet_GetKeyValue_Test()
{
    Vault vlt;
    int keyValue;
    std::string wrongKeyValue;
    VaultRecordSet vrs;

    // Non assigned set get
    TEST_ASSERT(vrs.GetKeyValue("A", keyValue).ResultCode == VaultOperationResultCode::ParentVaultNotValid, "Failed to get proper key value");

    vlt.AddKey("A", -1);
    vlt.CreateRecord();

    vlt.Request(Equal("A", -1), vrs);

    // Correct get
    TEST_ASSERT(vrs.GetKeyValue("A", keyValue).ResultCode == VaultOperationResultCode::Success, "Failed to get proper key value");

    // Not existed key
    TEST_ASSERT(vrs.GetKeyValue("B", keyValue).ResultCode == VaultOperationResultCode::WrongKey, "Failed to get not existed key value");

    // Vrong value type
    TEST_ASSERT(vrs.GetKeyValue("A", wrongKeyValue).ResultCode == VaultOperationResultCode::WrongType, "Failed to get key value with wrong type");

    // Dynamic key add
    vlt.AddKey("B", -1);

    TEST_ASSERT(vrs.GetKeyValue("B", keyValue).ResultCode == VaultOperationResultCode::Success, "Failed to get not existed key value");
}

void VaultRecordSet_GetKeyType_Test()
{
    Vault vlt;
    std::type_index type = typeid(void);
    VaultRecordSet vrs;

    TEST_ASSERT(vrs.GetKeyType("A", type) == false, "Failed to get not existed key type");

    vlt.AddKey("A", -1);

    vlt.CreateRecord();

    vlt.Request(Equal("A", -1), vrs);

    // Correct get
    vrs.GetKeyType("A", type);
    TEST_ASSERT(type == typeid(int), "Failed to get key type");

    // Get not existed key
    TEST_ASSERT(vrs.GetKeyType("B", type) == false, "Failed to get not existed key type");

    // Dynamic key add
    vlt.AddKey("B", -1);

    TEST_ASSERT(vrs.GetKeyType("B", type) == true, "Failed to get not existed key type");
}

void VaultRecordSet_AddRecord_Test()
{
    Vault vlt;
    VaultRecordSet vrs;
    VaultRecordRef vrf;

    TEST_ASSERT(vrs.AddRecord(vrf).ResultCode == VaultOperationResultCode::ParentVaultNotValid, "Failed to add record");

    vlt.AddKey("A", -1);
    vrf = vlt.CreateRecord();

    TEST_ASSERT(vrs.AddRecord(vrf).ResultCode == VaultOperationResultCode::ParentVaultNotValid, "Failed to add record");

    vlt.Request(Equal("A", -1), vrs);

    vrs.Clear();

    TEST_ASSERT(vrs.AddRecord(vrf).ResultCode == VaultOperationResultCode::Success, "Failed to add record");
    TEST_ASSERT(vrs.AddRecord(vrf).ResultCode == VaultOperationResultCode::RecordAlredyInSet, "Failed to add record");
}

int main()
{
    VaultRecordSet_CopyConstructor_Test();
    VaultRecordSet_OperatorAssignment_Test();
    VaultRecordSet_OperatorComparison_Test();
    VaultRecordSet_GetIsParentVaultValid_Test();
    VaultRecordSet_GetParentVaultUniqueId_Test();
    VaultRecordSet_IsKeyExist_Test();
    VaultRecordSet_GetKeyValue_Test();
    VaultRecordSet_GetKeyType_Test();
    VaultRecordSet_AddRecord_Test();
}