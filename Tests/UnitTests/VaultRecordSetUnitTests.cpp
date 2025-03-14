#include "VaultRecordSetUnitTests.h"

#define GENERATE_SET(setName) \
VaultRecordSet setName; \
vlt.AddKey("A", 0); \
vlt.CreateRecord({{}}); \
vlt.RequestEqual("A", 0, setName); \
vlt.EraseRecord("A", 0); \
vlt.RemoveKey("A");

using namespace VaultRecordSetTestNs;

TEST_BODY(DefaultConstructor, Default,
    VaultRecordSet vrs;

    TEST_ASSERT(vrs.Size() == 0);
    TEST_ASSERT(vrs.GetKeys().size() == 0);
    TEST_ASSERT(vrs.GetIsParentVaultValid() == false);
    TEST_ASSERT(vrs.GetParentVaultUniqueId() == "null");
    
    COMPARE_VAULT(vrs, {});
)

TEST_BODY(CopyConstructor, CopyWithoutParent,
    VaultRecordSet vrs1;
    VaultRecordSet vrs2(vrs1);

    TEST_ASSERT(vrs2.Size() == 0);
    TEST_ASSERT(vrs2.GetKeys().size() == 0);
    TEST_ASSERT(vrs2.GetIsParentVaultValid() == false);
    TEST_ASSERT(vrs2.GetParentVaultUniqueId() == "null");

    COMPARE_VAULT(vrs2, {});
)

TEST_BODY(CopyConstructor, CopyEmptySet,
    Vault vlt;
    GENERATE_SET(vrs1);
    VaultRecordSet vrs2(vrs1);

    TEST_ASSERT(vrs2.Size() == 0);
    TEST_ASSERT(vrs2.GetKeys().size() == 0);
    TEST_ASSERT(vrs2.GetIsParentVaultValid() == true);

    COMPARE_VAULT(vrs2, {});
)

TEST_BODY(CopyConstructor, CopyFilledSet,
    Vault vlt;
    VaultRecordSet vrs;

    vlt.AddKey("A", 0);
    vlt.AddKey<std::string>("B", "");
    
    vlt.CreateRecord({ {"A", 0}, {"B", std::string("a")} });
    vlt.CreateRecord({ {"A", 0}, {"B", std::string("b")} });
    vlt.CreateRecord({ {"A", 0}, {"B", std::string("c")} });

    vlt.RequestEqual("A", 0, vrs);


    TEST_ASSERT(vrs.Size() == 3);
    TEST_ASSERT(vrs.GetKeys().size() == 2);

    COMPARE_VAULT(vrs, {
        {{"A", 0}, {"B",  std::string("a")}},
        {{"A", 0}, {"B",  std::string("b")}},
        {{"A", 0}, {"B",  std::string("c")}},
    });
)

void VaultRecordSetUnitTests()
{
    DefaultConstructor::Default();

    CopyConstructor::CopyWithoutParent();
    CopyConstructor::CopyEmptySet();
    CopyConstructor::CopyFilledSet();
}