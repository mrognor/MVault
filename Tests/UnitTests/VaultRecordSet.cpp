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

    Vault vlt2;

    vrf = vlt2.CreateRecord();

    TEST_ASSERT(vrs.AddRecord(vrf).ResultCode == VaultOperationResultCode::ParentVaultNotMatch, "Failed to add record");
}

void VaultRecordSet_GetRecord_Test()
{
    Vault vlt;
    VaultRecordSet vrs;
    VaultRecordRef vrr;

    TEST_ASSERT(vrs.GetRecord("A", 0, vrr).ResultCode == VaultOperationResultCode::ParentVaultNotValid, "Error on record creation");

    vlt.AddKey("A", -1);
    vlt.AddKey("B", '0');
    vlt.AddKey("C", true);

    // Get set and make it empty
    vrr = vlt.CreateRecord();
    vlt.Request(Equal("A", -1), vrs);
    vlt.EraseRecord(vrr);

    // Correct get record on empty vault
    TEST_ASSERT(!vrr.IsValid(), "Error on record creation") 
    TEST_ASSERT(vrs.GetRecord("A", 0, vrr).ResultCode == VaultOperationResultCode::WrongValue, "Error on record creation");

    // Wrong key
    TEST_ASSERT(vrs.GetRecord("D", 0, vrr).ResultCode == VaultOperationResultCode::WrongKey, "Error on get result");
    // Wrong key type
    TEST_ASSERT(vrs.GetRecord("A", std::string(), vrr).ResultCode == VaultOperationResultCode::WrongType, "Error on get result");

    // Add record to set
    vrs.AddRecord(vlt.CreateRecord());

    // Correct get record
    TEST_ASSERT(vrs.GetRecord("A", -1, vrr).ResultCode == VaultOperationResultCode::Success, "Error on record creation");

    // Wrong value
    TEST_ASSERT(vrs.GetRecord("A", 0, vrr).ResultCode == VaultOperationResultCode::WrongValue, "Error on get result");
    // Wrong key
    TEST_ASSERT(vrs.GetRecord("D", 0, vrr).ResultCode == VaultOperationResultCode::WrongKey, "Error on get result");
    // Wrong key type
    TEST_ASSERT(vrs.GetRecord("A", std::string(), vrr).ResultCode == VaultOperationResultCode::WrongType, "Error on get result");

    // Create records with vector of params
    for (int i = 0; i < 10; ++i) vlt.CreateRecord({ {"A", i}, {"B", static_cast<char>('A' + i)} });
    vlt.Request(Equal("C", true), vrs);

    // Check records
    for (int i = 0; i < 10; ++i)
    {
        VaultRecordRef vrr;
        vrs.GetRecord("A", i, vrr);
        char c;
        vrr.GetData("B", c);
        TEST_ASSERT(c == static_cast<char>('A' + i), "Failed to create record");
    }
}

void VaultRecordSet_GetRecords_Test()
{
    Vault vlt;
    VaultRecordRef vrr;
    VaultRecordSet vrs;
    std::vector<VaultRecordRef> vec;

    TEST_ASSERT(vrs.GetRecords("A", 0, vec).ResultCode == VaultOperationResultCode::ParentVaultNotValid, "Error on record creation");

    vlt.AddKey("A", -1);
    vlt.AddKey("B", '0');
    vlt.AddKey("C", true);

    // Get set and make it empty
    vrr = vlt.CreateRecord();
    vlt.Request(Equal("A", -1), vrs);
    vlt.EraseRecord(vrr);

    // Correct get records on empty vault
    TEST_ASSERT(vec.size() == 0, "Error on record creation") 
    TEST_ASSERT(vrs.GetRecords("A", 0, vec).ResultCode == VaultOperationResultCode::WrongValue, "Error on record creation");

    // Wrong key
    TEST_ASSERT(vrs.GetRecords("D", 0, vec).ResultCode == VaultOperationResultCode::WrongKey, "Error on get result");
    // Wrong key type
    TEST_ASSERT(vrs.GetRecords("A", std::string(), vec).ResultCode == VaultOperationResultCode::WrongType, "Error on get result");

    // Fill vault
    for (int i = 0; i < 10; ++i) vlt.CreateRecord({ {"A", i}, });

    // Get records
    vlt.Request(Equal("C", true), vrs);
    vrs.GetRecords("B", '0', vec);

    for (int i = 0; i < 10; ++i)
    {
        int n;
        TEST_ASSERT(vec[i].GetData("A", n).ResultCode == VaultOperationResultCode::Success, "Error on get records");
    }

    // Get only 2 records
    vrs.GetRecords("B", '0', vec, 2);
    TEST_ASSERT(vec.size() == 2, "Wrong vec size");
}

void VaultRecordSet_Request_Tests()
{
    Vault vlt;
    VaultRecordRef vrr;
    VaultRecordSet vrs1, vrs2;

    TEST_ASSERT(vrs1.Request(Equal("A", 0), vrs2).ResultCode == VaultOperationResultCode::ParentVaultNotValid, "Error on record creation");

    vlt.AddKey("A", -1);
    vlt.AddKey("B", -1);
    vlt.AddKey<std::string>("C", "-1");

    // Correct request on empty vault
    vlt.Request(Equal("A", -1), vrs1);
    TEST_ASSERT(vrs1.Request(Equal("A", -1), vrs2).ResultCode == VaultOperationResultCode::Success, "Error on record creation");

    // Incorrect requests
    // Wrong key
    TEST_ASSERT(vrs1.Request(Equal("D", -1), vrs2).ResultCode == VaultOperationResultCode::WrongKey, "Error on get result");
    // Wrong key type
    TEST_ASSERT(vrs1.Request(Equal("A", std::string()), vrs2).ResultCode == VaultOperationResultCode::WrongType, "Error on get result");

    // Fill vault
    vlt.CreateRecord({ {"A", 0}, {"C", std::to_string(10)}});
    for (int i = 0; i < 10; ++i) vlt.CreateRecord({ {"A", i}, {"C", std::to_string(10 - i)}});
    vlt.CreateRecord({ {"A", 9}, {"C", std::to_string(1)}});

    vlt.Request(Equal("B", -1), vrs1);

    //  Correct requests

    // Equal
    {
        vrs1.Request(Equal("A", 2), vrs2);
        TEST_ASSERT(vrs2.Size() == 1, "Error on record request");

        vrs1.Request(Equal("B", -1), vrs2);
        TEST_ASSERT(vrs2.Size() == 12, "Error on record request");

        vrs1.Request(Equal("C", std::string("a")), vrs2);
        TEST_ASSERT(vrs2.Size() == 0, "Error on record request");
    }

    // Less
    {
        vrs1.Request(Less("A", 1), vrs2);
        TEST_ASSERT(vrs2.Size() == 2, "Error on record request");

        vrs1.Request(Less("A", 0), vrs2);
        TEST_ASSERT(vrs2.Size() == 0, "Error on record request");

        vrs1.Request(Less("A", 15), vrs2);
        TEST_ASSERT(vrs2.Size() == 12, "Error on record request");

        vrs1.Request(Less("A", 10), vrs2);
        TEST_ASSERT(vrs2.Size() == 12, "Error on record request");

        vrs1.Request(Less("A", 9), vrs2);
        TEST_ASSERT(vrs2.Size() == 10, "Error on record request");

        vrs1.Request(Less("B", 1), vrs2);
        TEST_ASSERT(vrs2.Size() == 12, "Error on record request");

        vrs1.Request(Less("B", -2), vrs2);
        TEST_ASSERT(vrs2.Size() == 0, "Error on record request");
    }

    // LessOrEqual
    {
        vrs1.Request(LessOrEqual("A", 1), vrs2);
        TEST_ASSERT(vrs2.Size() == 3, "Error on record request");

        vrs1.Request(LessOrEqual("A", 0), vrs2);
        TEST_ASSERT(vrs2.Size() == 2, "Error on record request");

        vrs1.Request(LessOrEqual("A", 15), vrs2);
        TEST_ASSERT(vrs2.Size() == 12, "Error on record request");

        vrs1.Request(LessOrEqual("A", 9), vrs2);
        TEST_ASSERT(vrs2.Size() == 12, "Error on record request");

        vrs1.Request(LessOrEqual("B", 1), vrs2);
        TEST_ASSERT(vrs2.Size() == 12, "Error on record request");

        vrs1.Request(LessOrEqual("B", -1), vrs2);
        TEST_ASSERT(vrs2.Size() == 12, "Error on record request");

        vrs1.Request(LessOrEqual("B", -2), vrs2);
        TEST_ASSERT(vrs2.Size() == 0, "Error on record request");
    }

    // Greater
    {
        vrs1.Request(Greater("A", 8), vrs2);
        TEST_ASSERT(vrs2.Size() == 2, "Error on record request");

        vrs1.Request(Greater("A", 9), vrs2);
        TEST_ASSERT(vrs2.Size() == 0, "Error on record request");

        vrs1.Request(Greater("A", -1), vrs2);
        TEST_ASSERT(vrs2.Size() == 12, "Error on record request");

        vrs1.Request(Greater("A", 0), vrs2);
        TEST_ASSERT(vrs2.Size() == 10, "Error on record request");

        vrs1.Request(Greater("B", -1), vrs2);
        TEST_ASSERT(vrs2.Size() == 0, "Error on record request");

        vrs1.Request(Greater("B", -2), vrs2);
        TEST_ASSERT(vrs2.Size() == 12, "Error on record request");
    }

    // GreaterOrEqual
    {
        vrs1.Request(GreaterOrEqual("A", 8), vrs2);
        TEST_ASSERT(vrs2.Size() == 3, "Error on record request");

        vrs1.Request(GreaterOrEqual("A", 9), vrs2);
        TEST_ASSERT(vrs2.Size() == 2, "Error on record request");

        vrs1.Request(GreaterOrEqual("A", -1), vrs2);
        TEST_ASSERT(vrs2.Size() == 12, "Error on record request");

        vrs1.Request(GreaterOrEqual("A", 0), vrs2);
        TEST_ASSERT(vrs2.Size() == 12, "Error on record request");

        vrs1.Request(GreaterOrEqual("B", 0), vrs2);
        TEST_ASSERT(vrs2.Size() == 0, "Error on record request");

        vrs1.Request(GreaterOrEqual("B", -1), vrs2);
        TEST_ASSERT(vrs2.Size() == 12, "Error on record request");

        vrs1.Request(GreaterOrEqual("B", -2), vrs2);
        TEST_ASSERT(vrs2.Size() == 12, "Error on record request");
    }

    // Interval
    {
        vrs1.RequestInterval("A", -1, 10, vrs2, true, true);
        TEST_ASSERT(vrs2.Size() == 12, "Error on record request");

        vrs1.RequestInterval("A", -1, 10, vrs2, true, false);
        TEST_ASSERT(vrs2.Size() == 12, "Error on record request");

        vrs1.RequestInterval("A", -1, 10, vrs2, false, true);
        TEST_ASSERT(vrs2.Size() == 12, "Error on record request");

        vrs1.RequestInterval("A", -1, 10, vrs2, false, false);
        TEST_ASSERT(vrs2.Size() == 12, "Error on record request");


        vrs1.RequestInterval("A", 0, 10, vrs2, true, true);
        TEST_ASSERT(vrs2.Size() == 12, "Error on record request");

        vrs1.RequestInterval("A", 0, 10, vrs2, true, false);
        TEST_ASSERT(vrs2.Size() == 12, "Error on record request");

        vrs1.RequestInterval("A", 0, 10, vrs2, false, true);
        TEST_ASSERT(vrs2.Size() == 10, "Error on record request");

        vrs1.RequestInterval("A", 0, 10, vrs2, false, false);
        TEST_ASSERT(vrs2.Size() == 10, "Error on record request");


        vrs1.RequestInterval("A", -1, 9, vrs2, true, true);
        TEST_ASSERT(vrs2.Size() == 12, "Error on record request");

        vrs1.RequestInterval("A", -1, 9, vrs2, true, false);
        TEST_ASSERT(vrs2.Size() == 10, "Error on record request");

        vrs1.RequestInterval("A", -1, 9, vrs2, false, true);
        TEST_ASSERT(vrs2.Size() == 12, "Error on record request");

        vrs1.RequestInterval("A", -1, 9, vrs2, false, false);
        TEST_ASSERT(vrs2.Size() == 10, "Error on record request");


        vrs1.RequestInterval("A", 0, 9, vrs2, true, true);
        TEST_ASSERT(vrs2.Size() == 12, "Error on record request");

        vrs1.RequestInterval("A", 0, 9, vrs2, true, false);
        TEST_ASSERT(vrs2.Size() == 10, "Error on record request");

        vrs1.RequestInterval("A", 0, 9, vrs2, false, true);
        TEST_ASSERT(vrs2.Size() == 10, "Error on record request");

        vrs1.RequestInterval("A", 0, 9, vrs2, false, false);
        TEST_ASSERT(vrs2.Size() == 8, "Error on record request");
    }

    // Request 0 records
    TEST_ASSERT(vrs1.RequestEqual("B", -1, vrs2, 0).ResultCode == VaultOperationResultCode::Success, "Error on record request");
    TEST_ASSERT(vrs2.Size() == 0, "Error on record request");

    // Request 2 records
    TEST_ASSERT(vrs1.RequestEqual("B", -1, vrs2, 2).ResultCode == VaultOperationResultCode::Success, "Error on record request");
    TEST_ASSERT(vrs2.Size() == 2, "Error on record request");

    VaultOperationResult res;
    // Predicat test. B == -1 and A = stoi(C)
    res = vrs1.RequestEqual("B", -1, vrs2, -1, [](const VaultRecordRef& ref)
        {
            int a; std::string c;
            ref.GetData("A", a);
            ref.GetData("C", c);
            if (a == stoi(c)) return true;
            else return false;
        });
    TEST_ASSERT(res.ResultCode == VaultOperationResultCode::Success, "Error on record request");
    TEST_ASSERT(vrs2.Size() == 1, "Error on record request");
    vrs2.GetRecord("B", -1, vrr);
    int n;
    vrr.GetData("A", n);
    TEST_ASSERT(n == 5, "Failed to make request");
}

void VaultRecordSet_Reset_Tests()
{
    Vault vlt;
    VaultRecordSet vrs;

    vlt.AddKey("A", -1);
    vlt.AddKey("B", '0');
    vlt.AddKey("C", true);

    // Fill vault
    for (int i = 0; i < 10; ++i) vlt.CreateRecord({ {"A", i} });

    vrs.Request(Equal("B", '0'), vrs);
    vrs.Reset();

    TEST_ASSERT(vrs.GetIsParentVaultValid() == false, "Failed to drop vault record set");
    TEST_ASSERT(vrs.GetKeys().size() == 0, "Failed to drop vault record set");
    TEST_ASSERT(vrs.Size() == 0, "Failed to drop vault record set");
}

void VaultRecordSet_Clear_Tests()
{
    Vault vlt;
    VaultRecordSet vrs;

    vlt.AddKey("A", -1);
    vlt.AddKey("B", '0');
    vlt.AddKey("C", true);

    // Fill vault
    for (int i = 0; i < 10; ++i) vlt.CreateRecord({ {"A", i} });

    vrs.Request(Equal("B", '0'), vrs);
    vrs.Clear();

    TEST_ASSERT(vrs.GetIsParentVaultValid() == true, "Failed to drop vault record set");
    TEST_ASSERT(vrs.GetKeys().size() == 3, "Failed to drop vault record set");
    TEST_ASSERT(vrs.Size() == 0, "Failed to drop vault record set");
}

void VaultRecordSet_RemoveRecord_Test()
{
    // VaultRecordRef overload
    Vault vlt;
    VaultRecordSet vrs;
    VaultRecordRef vrr;

    // Check no parent set
    TEST_ASSERT(vrs.RemoveRecord("A", -1).ResultCode == VaultOperationResultCode::ParentVaultNotValid, "Failed to erase record");

    // Erase empty record
    TEST_ASSERT(vrs.RemoveRecord(vrr) == false, "Failed to erase record");

    vlt.AddKey("A", -1);
    vlt.AddKey("T", 0);

    // Fill vault
    for (int i = 0; i < 10; ++i) vlt.CreateRecord({ {"A", i} });

    vlt.Request(Equal("T", 0), vrs);
    vlt.GetRecord("A", 0, vrr);

    TEST_ASSERT(vrs.RemoveRecord(vrr), "Failed to erase record");
    TEST_ASSERT(vrs.Size() == 9, "Failed to erase record");
    
    TEST_ASSERT(vrs.RemoveRecord(vrr) == false, "Failed to erase record");

    vrs.GetRecord("A", 1, vrr);
    vrr.Reset();
    TEST_ASSERT(vrs.RemoveRecord(vrr) == false, "Failed to delete record");
    TEST_ASSERT(vrs.Size() == 9, "Failed to erase record");

    // Key and value overload

    // Incorrect erasing
    // Wrong key
    TEST_ASSERT(vrs.RemoveRecord("B", 1).ResultCode == VaultOperationResultCode::WrongKey, "Error on erase");
    // Wrong key type
    TEST_ASSERT(vrs.RemoveRecord("A", 2.9).ResultCode == VaultOperationResultCode::WrongType, "Error on erase");

    // No value erasing
    TEST_ASSERT(vrs.RemoveRecord("A", 0).ResultCode == VaultOperationResultCode::WrongValue, "Error on erase");

    // Correct erasing
    for (int i = 1; i < 10; ++i)
        TEST_ASSERT(vrs.RemoveRecord("A", i).ResultCode == VaultOperationResultCode::Success, "Error on erase");

    vlt.DropData();

    vrs.AddRecord(vlt.CreateRecord());
    vrs.AddRecord(vlt.CreateRecord());

    vlt.EraseRecord("A", -1);
    TEST_ASSERT(vrs.Size() == 1, "Failed to erase record");
    vrs.RemoveRecord("A", -1);
    TEST_ASSERT(vrs.Size() == 0, "Failed to erase record");
}

void VaultRecordSet_RemoveRecords_Test()
{
    Vault vlt;
    VaultRecordSet vrs;
    VaultRecordRef vrr;

    // Check no parent set
    TEST_ASSERT(vrs.RemoveRecords("A", -1).ResultCode == VaultOperationResultCode::ParentVaultNotValid, "Failed to erase record");

    vlt.AddKey("A", -1);
    vlt.Request(Equal("A", -1), vrs);

    // Wrong key
    TEST_ASSERT(vrs.RemoveRecords("B", 1).ResultCode == VaultOperationResultCode::WrongKey, "Error on erase");
    // Wrong key type
    TEST_ASSERT(vrs.RemoveRecords("A", 2.9).ResultCode == VaultOperationResultCode::WrongType, "Error on erase");
    // No value erasing
    TEST_ASSERT(vrs.RemoveRecords("A", 0).ResultCode == VaultOperationResultCode::WrongValue, "Error on erase");

    // Fill vault
    for (int i = 0; i < 10; ++i) vlt.CreateRecord();
    vlt.CreateRecord({ {"A", 1} });

    vlt.Request(Equal("A", -1) || Equal("A", 1), vrs);

    // Erase 2 records
    TEST_ASSERT(vrs.RemoveRecords("A", -1, 2).ResultCode == VaultOperationResultCode::Success, "Failed to erase records");
    TEST_ASSERT(vrs.Size() == 9, "Failed to erase records");

    // Erase all records
    TEST_ASSERT(vrs.RemoveRecords("A", -1).ResultCode == VaultOperationResultCode::Success, "Failed to erase records");
    TEST_ASSERT(vrs.Size() == 1, "Failed to erase records");
}

void VaultRecordSet_Size_Test()
{
    Vault vlt;
    VaultRecordSet vrs;
    VaultRecordRef vrr;

    vlt.AddKey<std::size_t>("A", -1);

    for (std::size_t i = 1; i < 11; ++i)
    {
        vlt.CreateRecord();
        vlt.Request(Equal("A", -1), vrs);
        TEST_ASSERT(vrs.Size() == i, "Failed to create record");
    }

    for (std::size_t i = 10; i > 0; --i)
    {
        vrs.RemoveRecord("A", i);
        TEST_ASSERT(vrs.Size() == i - 1, "Failed to create record");
    }
}

void VaultRecordSet_Destructor_Test()
{
    Vault vlt;

    vlt.AddKey("A", -1);
    vlt.CreateRecord();

    VaultRecordSet* vrs = new VaultRecordSet;

    vlt.Request(Equal("A", -1), *vrs);

    delete vrs;
    vrs = new VaultRecordSet;
    delete vrs;
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
    VaultRecordSet_GetRecord_Test();
    VaultRecordSet_GetRecords_Test();
    VaultRecordSet_Request_Tests();
    VaultRecordSet_Reset_Tests();
    VaultRecordSet_RemoveRecord_Test();
    VaultRecordSet_RemoveRecords_Test();
    VaultRecordSet_Destructor_Test();
}