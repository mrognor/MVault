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

int main()
{
    KeyAddictionTests();
    KeyUpdationTests();
    KeyCheckTests();
}