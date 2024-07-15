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

int main()
{
    VaultRecordSet_CopyConstructor_Test();
    VaultRecordSet_OperatorAssignment_Test();
    VaultRecordSet_OperatorComparison_Test();
    VaultRecordSet_GetIsParentVaultValid_Test();
    VaultRecordSet_GetParentVaultUniqueId_Test();
}