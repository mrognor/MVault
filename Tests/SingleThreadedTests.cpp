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
    VaultRecordRef vrr1, vrr2;
    TEST_ASSERT(vrr1.GetKeys() == vector<string>(), "VaultRecordRef is not created with empty keys");

    // Create VaultRecordSets
    VaultRecordSet vrs1, vrs2;
    TEST_ASSERT(vrs1.GetKeys() == vector<string>(), "VaultRecordSet is not created with empty keys");

    // Create new record
    vrr1 = vlt.CreateRecord();

    // Add key
    vlt.AddKey<int>("A", -1);

    // Check VaultRecordRef dynamic key addition
    TEST_ASSERT(vrr1.GetKeys() == vector<string>({"A"}), "The key is not added dynamically to VaultRecordRef");

    // Request VaultRecordSet
    vlt.RequestEqual("A", -1, vrs1);

    // Check VaultRecordSet request
    TEST_ASSERT(vrs1.GetKeys() == vector<string>({"A"}), "The VaultRecordSet request is not working correctly");

    // Add keys
    vlt.AddKey<std::string>("B", "none");
    vlt.AddKey<bool>("C", false);

    // Check dynamic key addition
    TEST_ASSERT(vrr1.GetKeys() == vector<string>({"A", "B", "C"}), "The key is not added dynamically to VaultRecordRef");
    TEST_ASSERT(vrs1.GetKeys() == vector<string>({"A", "B", "C"}), "The key is not added dynamically to VaultRecordSet");
    vrs1.GetRecord<std::string>("B", "none", vrr1);
    TEST_ASSERT(vrr1.IsValid(), "The key is not added dynamically to VaultRecordSet");
}

int main()
{
    KeyAddictionTests();
}