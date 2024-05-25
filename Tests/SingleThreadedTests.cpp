#include "../Source/MVault.h"

#include "TestAssert.h"

using namespace mvlt;
using namespace std;

void KeyAddictionTests()
{
    // Create Vault
    Vault vlt;
    TEST_ASSERT(vlt.GetKeys() == vector<string>(), "Vault is not created with empty keys");

    // Create VaultRecordRefs
    VaultRecordRef vrr;
    TEST_ASSERT(vrr.GetKeys() == vector<string>(), "VaultRecordRef is not created with empty keys");

    // Create VaultRecordSets
    VaultRecordSet vrs;
    TEST_ASSERT(vrs.GetKeys() == vector<string>(), "VaultRecordSet is not created with empty keys");

    // Create new record
    vrr = vlt.CreateRecord();

    // Add key
    vlt.AddKey<int>("A", -1);

    // Check VaultRecordRef dynamic key addition
    TEST_ASSERT(vrr.GetKeys() == vector<string>({"A"}), "The key is not added dynamically to VaultRecordRef");

    // Request VaultRecordSet
    vlt.RequestEqual("A", -1, vrs);

    // Check VaultRecordSet request
    TEST_ASSERT(vrs.GetKeys() == vector<string>({"A"}), "The VaultRecordSet request is not working correctly");

    // Add keys
    vlt.AddKey<std::string>("B", "none");
    vlt.AddKey<bool>("C", false);

    // Check dynamic key addition
    TEST_ASSERT(vrr.GetKeys() == vector<string>({"A", "B", "C"}), "The key is not added dynamically to VaultRecordRef");
    TEST_ASSERT(vrs.GetKeys() == vector<string>({"A", "B", "C"}), "The key is not added dynamically to VaultRecordSet");
    vrs.GetRecord<std::string>("B", "none", vrr);
    TEST_ASSERT(vrr.IsValid(), "The key is not added dynamically to VaultRecordSet");
}

void KeyUpdationTests()
{
    // Create Vault
    Vault vlt;
    
    // Create VaultRecordRef
    VaultRecordRef vrr;

    // Add key
    vlt.AddKey<int>("A", -1);

    // Create record
    vrr = vlt.CreateRecord();

    // Check VaultRecordRef getting
    int A;
    vrr.GetData("A", A);
    TEST_ASSERT(A == -1, "The default setting does not work correctly when creating a record");

    // Update default value
    vlt.UpdateKey("A", 0);
    vrr = vlt.CreateRecord();
    vlt.GetRecord("A", 0, vrr);

    // Check key value updating
    vrr.GetData("A", A);
    TEST_ASSERT(A == 0, "Updating the default key value does not work correctly");
}

void KeyCheckTests()
{
    // Create Vault
    Vault vlt;

    // Create VaultRecordRef
    VaultRecordRef vrr;

    // Create VaultRecordSet
    VaultRecordSet vrs;

    // IsKeyExist check on empty
    TEST_ASSERT(!vlt.IsKeyExist("A"), "Failed key request");
    TEST_ASSERT(!vrr.IsKeyExist("A"), "Failed key request");
    TEST_ASSERT(!vrs.IsKeyExist("A"), "Failed key request");

    // Add new key to Vault
    vlt.AddKey<int>("A", -1);

    // Create new record
    vrr = vlt.CreateRecord();

    // Request record
    vlt.RequestEqual("A", -1, vrs);

    // IsKeyExist check on correct data
    TEST_ASSERT(vlt.IsKeyExist("A"), "Failed key request");
    TEST_ASSERT(vrr.IsKeyExist("A"), "Failed key request");
    TEST_ASSERT(vrs.IsKeyExist("A"), "Failed key request");
        
    // IsKeyExist check on incorrect data
    TEST_ASSERT(!vlt.IsKeyExist("B"), "Failed key request");
    TEST_ASSERT(!vrr.IsKeyExist("B"), "Failed key request");
    TEST_ASSERT(!vrs.IsKeyExist("B"), "Failed key request");

    // Remove key
    vlt.RemoveKey("A");

    // IsKeyExist check on incorrect data
    TEST_ASSERT(!vlt.IsKeyExist("A"), "Failed key request");
    TEST_ASSERT(!vrr.IsKeyExist("A"), "Failed key request");
    TEST_ASSERT(!vrs.IsKeyExist("A"), "Failed key request");

    // Add new key
    vlt.AddKey("A", true);

    // IsKeyExist check on correct data
    TEST_ASSERT(vlt.IsKeyExist("A"), "Failed key request");
    TEST_ASSERT(vrr.IsKeyExist("A"), "Failed key request");
    TEST_ASSERT(vrs.IsKeyExist("A"), "Failed key request");
}

void RecordCreationTests()
{
    // Create Vault
    Vault vlt;

    // Create VaultRecordRef
    VaultRecordRef vrr;

    // Create variable to compare results
    VaultOperationResult res;
    res.IsOperationSuccess = true;
    res.RequestedType = typeid(int);
    res.SavedType = typeid(int);
    res.ResultCode = VaultOperationResultCode::Success;

    // Add new keys to Vault
    vlt.AddKey<int>("A", -1);
    vlt.AddKey<int>("B", -1);
    vlt.AddKey<int>("C", -1);
    vlt.AddKey<int>("D", -1);

    // Create new records
    vlt.CreateRecord();
    
    // Check vault size
    TEST_ASSERT(vlt.Size() == 1, "Record was not created!");

    // Check record creation
    res.Key = "A";
    TEST_ASSERT(vlt.GetRecord("A", -1, vrr) == res, "Record was not created correct!");
    res.Key = "B";
    TEST_ASSERT(vlt.GetRecord("B", -1, vrr) == res, "Record was not created correct!");
    res.Key = "C";
    TEST_ASSERT(vlt.GetRecord("C", -1, vrr) == res, "Record was not created correct!");
    res.Key = "D";
    TEST_ASSERT(vlt.GetRecord("D", -1, vrr) == res, "Record was not created correct!");

    // Erase record
    vlt.EraseRecord(vrr);

    // Create 10 new records
    for (int i = 0; i < 10; ++i)
        vlt.CreateRecord({ {"A", i}, {"B", i * 10}, {"C", i * 100}, {"D", i * 1000}});

    // Check vault size
    TEST_ASSERT(vlt.Size() == 10, "Records was not created!");

    // Check all records
    for (int i = 0; i < 10; ++i)
    {
        // Set recordId
        vlt.GetRecord("A", i, vrr);
        std::string recordId = vrr.GetRecordUniqueId();

        // Check i * 10
        vlt.GetRecord("B", i * 10, vrr);
        TEST_ASSERT(recordId == vrr.GetRecordUniqueId(), "Record was not created correct!");

        // Check i * 100
        vlt.GetRecord("C", i * 100, vrr);
        TEST_ASSERT(recordId == vrr.GetRecordUniqueId(), "Record was not created correct!");

        // Check i * 1000
        vlt.GetRecord("D", i * 1000, vrr);
        TEST_ASSERT(recordId == vrr.GetRecordUniqueId(), "Record was not created correct!");
    }
}

void RecordUpdationTests()
{
    // Create Vault
    Vault vlt;

    // Create VaultRecordRef
    VaultRecordRef vrr;

    // Create VaultRecordSet
    VaultRecordSet vrs;

    // Add new keys to Vault
    vlt.AddKey<int>("A", -1);
    vlt.AddKey<int>("B", -1);

    // Create new record
    vrr = vlt.CreateRecord();
    std::string recordId = vrr.GetRecordUniqueId();

    vlt.PrintAsTable();

    // Request record
    vlt.Request(Equal("A", -1), vrs);

    vrs.PrintAsTable();
    // Update data
    vrr.SetData("A", 0);
    
    // Check vault after updation
    vlt.GetRecord("A", 0, vrr);
    TEST_ASSERT(recordId == vrr.GetRecordUniqueId(), "Record updation failed");

    // Check VaultRecordSet after updation
    vrs.GetRecord("A", 0, vrr);
    TEST_ASSERT(recordId == vrr.GetRecordUniqueId(), "Record updation failed");    
    
    vlt.PrintAsTable();
    vrs.PrintAsTable(true);
}

int main()
{
    KeyAddictionTests();
    KeyUpdationTests();
    KeyCheckTests();
    RecordCreationTests();
    RecordUpdationTests();
}