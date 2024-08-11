#include "../Source/MVault.h"

#include "MTests/TestAssert.h"

using namespace mvlt;
using namespace std;

void VaultRecordRef_CopyConstructor_Test()
{
    Vault vlt;
    VaultRecordRef vrr1;
    VaultRecordRef vrr2(vrr1);

    // Invalid refs
    TEST_ASSERT(!vrr1.IsValid(), "Failed to copy vault record ref");
    TEST_ASSERT(!vrr2.IsValid(), "Failed to copy vault record ref");

    vlt.AddKey("A", -1);
    vrr1 = vlt.CreateRecord();

    VaultRecordRef vrr3(vrr1);

    TEST_ASSERT(vrr1 == vrr3, "Failed to copy vault record ref");

    vrr2.Reset();
    VaultRecordRef vrr4(vrr2);
    TEST_ASSERT(!vrr4.IsValid(), "Failed to copy vault record ref");
}

void VaultRecordRef_OperatorAssignment_Test()
{
    Vault vlt;
    VaultRecordRef vrr1;
    VaultRecordRef vrr2 = vrr1;

    // Invalid refs
    TEST_ASSERT(!vrr1.IsValid(), "Failed to assign vault record ref");
    TEST_ASSERT(!vrr2.IsValid(), "Failed to assign vault record ref");

    vlt.AddKey("A", -1);
    vrr1 = vlt.CreateRecord();

    VaultRecordRef vrr3 = vrr1;

    TEST_ASSERT(vrr1 == vrr3, "Failed to assign vault record ref");

    vrr2.Reset();
    VaultRecordRef vrr4 = vrr2;
    TEST_ASSERT(!vrr4.IsValid(), "Failed to assign vault record ref");
}


void VaultRecordRef_OperatorComparison_Test()
{
    Vault vlt;
    VaultRecordRef vrr1, vrr2;

    // Invalid refs
    TEST_ASSERT(vrr1 == vrr2, "Failed to compare vault record ref");

    vlt.AddKey("A", -1);
    vlt.CreateRecord();

    vlt.GetRecord("A", -1, vrr1);
    vlt.GetRecord("A", -1, vrr2);

    TEST_ASSERT(vrr1 == vrr2, "Failed to compare vault record ref");

    vrr1 = vlt.CreateRecord();

    TEST_ASSERT(!(vrr1 == vrr2), "Failed to compare vault record ref");

    vrr2.Reset();

    TEST_ASSERT(!(vrr1 == vrr2), "Failed to compare vault record ref");
    
    vrr1.Reset();
    TEST_ASSERT(vrr1 == vrr2, "Failed to compare vault record ref");
}

void VaultRecordRef_GetRecordUniqueId_Test()
{
    Vault vlt;
    VaultRecordRef vrr1, vrr2;

    // Invalid refs
    TEST_ASSERT(vrr1.GetRecordUniqueId() == "null", "Failed to get unique id vault record ref");
    TEST_ASSERT(vrr2.GetRecordUniqueId() == "null", "Failed to get unique id vault record ref");

    vlt.AddKey("A", -1);
    vlt.CreateRecord();

    vlt.GetRecord("A", -1, vrr1);
    vlt.GetRecord("A", -1, vrr2);

    TEST_ASSERT(vrr1.GetRecordUniqueId() != "null", "Failed to get unique id vault record ref");
    TEST_ASSERT(vrr1.GetRecordUniqueId() == vrr2.GetRecordUniqueId(), "Failed to get unique id vault record ref");
}

void VaultRecordRef_SetData_Test()
{
    Vault vlt;
    VaultRecordRef vrr1, vrr2;
    VaultOperationResult res;

    vlt.AddKey("A", -1);
    vlt.AddKey<std::string>("B", "");

    vrr1 = vlt.CreateRecord();
    vrr2 = vrr1;

    vrr1.SetData("A", 0);
    vrr1.SetData<std::string>("B", "B");

    int a; std::string b;
    vrr2.GetData("A", a);
    vrr2.GetData("B", b);

    TEST_ASSERT(a == 0, "Failed to set data in vault record ref");
    TEST_ASSERT(b == "B", "Failed to set data in vault record ref");

    // Second function version
    for (int i = 1; i < 11; ++i)
    {
        vrr1 = vlt.CreateRecord();
        res = vrr1.SetData({{"A", i}, {"B", std::to_string(i)}});

        vrr1.GetData("A", a);
        vrr1.GetData("B", b);

        TEST_ASSERT(a == i, "Failed to set data in vault record ref");
        TEST_ASSERT(b == std::to_string(i), "Failed to set data in vault record ref");
        TEST_ASSERT(res.IsOperationSuccess, "Failed to set data in vault record ref");
    }

    // Handle error
    res = vrr2.SetData({{"C", 0}});
    TEST_ASSERT(res.ResultCode == VaultOperationResultCode::WrongKey , "Failed to set data in vault record ref");

    res = vrr2.SetData({{"A", 0}, {"B", 0}});
    TEST_ASSERT(res.ResultCode == VaultOperationResultCode::WrongType , "Failed to set data in vault record ref");

    vrr2.GetData("A", a);
    vrr2.GetData("B", b);

    TEST_ASSERT(a == 0, "Failed to set data in vault record ref");
    TEST_ASSERT(b == "B", "Failed to set data in vault record ref");
}

void VaultRecordRef_GetData_Test()
{
    Vault vlt;
    VaultRecordRef vrr;
    VaultOperationResult res;

    // Invalid ref
    TEST_ASSERT(vrr.GetData("A", res).ResultCode == VaultOperationResultCode::DataRecordNotValid, "Failed to get data from vault record ref");

    vlt.AddKey("A", 0);
    vlt.AddKey<std::string>("B","null");

    vrr = vlt.CreateRecord();

    // Valid get
    int i; std::string s;
    vrr.GetData("A", i);
    TEST_ASSERT(i == 0, "Failed to get data from vault record ref");
    vrr.GetData("B", s);
    TEST_ASSERT(s == "null", "Failed to get data from vault record ref");

    // Invalid get
    TEST_ASSERT(vrr.GetData("C", i).ResultCode == VaultOperationResultCode::WrongKey, "Failed to get data from vault record ref");
    TEST_ASSERT(vrr.GetData("B", i).ResultCode == VaultOperationResultCode::WrongType, "Failed to get data from vault record ref");
}

void VaultRecordRef_GetDataAsString_Test()
{
    Vault vlt;
    VaultRecordRef vrr;

    std::string s;

    // Invalid ref
    TEST_ASSERT(vrr.GetDataAsString("A", s).ResultCode == VaultOperationResultCode::DataRecordNotValid, "Failed to get data as string from vault record ref");

    vlt.AddKey("A", 0);
    vlt.AddKey<std::string>("B","null");

    vrr = vlt.CreateRecord();

    // Valid get
    vrr.GetDataAsString("A", s);
    TEST_ASSERT(s == "0", "Failed to get data as string from vault record ref");
    vrr.GetDataAsString("B", s);
    TEST_ASSERT(s == "null", "Failed to get data as string from vault record ref");

    // Invalid get
    TEST_ASSERT(vrr.GetDataAsString("C", s).ResultCode == VaultOperationResultCode::WrongKey, "Failed to get data as string from vault record ref");
}

void VaultRecordRef_IsValid_Test()
{
    Vault vlt;
    VaultRecordRef vrr;

    // Invalid ref
    TEST_ASSERT(vrr.IsValid() == false, "Failed to get is valid from vault record ref");

    vlt.AddKey("A", 0);
    vlt.AddKey<std::string>("B","null");

    vrr = vlt.CreateRecord();

    // Valid ref
    TEST_ASSERT(vrr.IsValid() == true, "Failed to get is valid from vault record ref");
}

void VaultRecordRef_GetKeys_Test()
{
    Vault vlt;
    VaultRecordRef vrr;

    // Invalid ref
    TEST_ASSERT(vrr.GetKeys().size() == 0, "Failed to get keys from vault record ref");

    vlt.AddKey("A", 0);
    vlt.AddKey<std::string>("B","null");

    vrr = vlt.CreateRecord();

    // Valid ref
    std::vector<std::string> v = {"A", "B"};
    TEST_ASSERT(vrr.GetKeys() == v, "Failed to get is valid from vault record ref");
}

void VaultRecordRef_Reset_Test()
{
    Vault vlt;
    VaultRecordRef vrr;

    // Invalid ref
    vrr.Reset();

    vlt.AddKey("A", 0);
    vlt.AddKey<std::string>("B","null");

    vrr = vlt.CreateRecord();

    // Valid ref
    TEST_ASSERT(vrr.IsValid() == true, "Failed to reset vault record ref");
    vrr.Reset();
    TEST_ASSERT(vrr.IsValid() == false, "Failed to reset vault record ref");
    vrr.Reset();
}

int main()
{
    VaultRecordRef_CopyConstructor_Test();
    VaultRecordRef_OperatorAssignment_Test();
    VaultRecordRef_OperatorComparison_Test();   
    VaultRecordRef_GetRecordUniqueId_Test();
    VaultRecordRef_SetData_Test();
    VaultRecordRef_GetData_Test();
    VaultRecordRef_GetDataAsString_Test();
    VaultRecordRef_IsValid_Test();
    VaultRecordRef_GetKeys_Test();
    VaultRecordRef_Reset_Test();
}