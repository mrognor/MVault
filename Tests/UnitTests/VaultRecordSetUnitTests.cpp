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
    TEST_ASSERT(vrs.GetUniqueKeys().size() == 0);
    TEST_ASSERT(vrs.GetIsParentVaultValid() == false);
    TEST_ASSERT(vrs.GetParentVaultUniqueId() == "null");
    
    COMPARE_VAULT(vrs, {});
)

TEST_BODY(CopyConstructor, CopyWithoutParent,
    VaultRecordSet vrs1;
    VaultRecordSet vrs2(vrs1);

    TEST_ASSERT(vrs2.Size() == 0);
    TEST_ASSERT(vrs2.GetKeys().size() == 0);
    TEST_ASSERT(vrs2.GetUniqueKeys().size() == 0);
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
    TEST_ASSERT(vrs2.GetUniqueKeys().size() == 0);
    TEST_ASSERT(vrs2.GetIsParentVaultValid() == true);

    COMPARE_VAULT(vrs2, {});
)

TEST_BODY(CopyConstructor, CopyFilledSet,
    Vault vlt;
    VaultRecordSet vrs1;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({ {"A", 0}, {"B", std::string("a")} });
    vlt.CreateRecord({ {"A", 0}, {"B", std::string("b")} });
    vlt.CreateRecord({ {"A", 0}, {"B", std::string("c")} });

    vlt.RequestEqual("A", 0, vrs1);

    VaultRecordSet vrs2(vrs1);

    TEST_ASSERT(vrs2.Size() == 3);
    TEST_ASSERT(vrs2.GetKeys().size() == 2);
    TEST_ASSERT(vrs2.GetUniqueKeys().size() == 1);
    TEST_ASSERT(vrs2.GetIsParentVaultValid() == true);

    COMPARE_VAULT(vrs2, {
        {{"A", 0}, {"B",  std::string("a")}},
        {{"A", 0}, {"B",  std::string("b")}},
        {{"A", 0}, {"B",  std::string("c")}},
    });
)

TEST_BODY(AssignmentOperator, AssignWithoutParents,
    VaultRecordSet vrs1, vrs2;
    vrs2 = vrs1;

    TEST_ASSERT(vrs2.Size() == 0);
    TEST_ASSERT(vrs2.GetKeys().size() == 0);
    TEST_ASSERT(vrs2.GetUniqueKeys().size() == 0);
    TEST_ASSERT(vrs2.GetIsParentVaultValid() == false);
    TEST_ASSERT(vrs2.GetParentVaultUniqueId() == "null");

    COMPARE_VAULT(vrs2, {});
)

TEST_BODY(AssignmentOperator, AssignWithParentsToWithoutParent,
    Vault vlt;
    VaultRecordSet vrs1, vrs2;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");
    
    vlt.CreateRecord({ {"A", 0}, {"B", std::string("a")} });
    vlt.CreateRecord({ {"A", 0}, {"B", std::string("b")} });
    vlt.CreateRecord({ {"A", 0}, {"B", std::string("c")} });

    vlt.RequestEqual("A", 0, vrs1);

    vrs1 = vrs2;

    TEST_ASSERT(vrs1.Size() == 0);
    TEST_ASSERT(vrs1.GetKeys().size() == 0);
    TEST_ASSERT(vrs1.GetUniqueKeys().size() == 0);
    TEST_ASSERT(vrs1.GetIsParentVaultValid() == false);
    TEST_ASSERT(vrs1.GetParentVaultUniqueId() == "null");

    COMPARE_VAULT(vrs1, {});
)

TEST_BODY(AssignmentOperator, AssignEmpty,
    Vault vlt;
    GENERATE_SET(vrs1);
    VaultRecordSet vrs2;

    vrs2 = vrs1;

    TEST_ASSERT(vrs2.Size() == 0);
    TEST_ASSERT(vrs2.GetKeys().size() == 0);
    TEST_ASSERT(vrs2.GetUniqueKeys().size() == 0);
    TEST_ASSERT(vrs2.GetIsParentVaultValid() == true);

    COMPARE_VAULT(vrs2, {});
)

TEST_BODY(AssignmentOperator, AssignFilledSet,
    Vault vlt;
    VaultRecordSet vrs1, vrs2;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");
    
    vlt.CreateRecord({ {"A", 0}, {"B", std::string("a")} });
    vlt.CreateRecord({ {"A", 0}, {"B", std::string("b")} });
    vlt.CreateRecord({ {"A", 0}, {"B", std::string("c")} });

    vlt.RequestEqual("A", 0, vrs1);

    vrs2 = vrs1;

    TEST_ASSERT(vrs2.Size() == 3);
    TEST_ASSERT(vrs2.GetKeys().size() == 2);
    TEST_ASSERT(vrs2.GetUniqueKeys().size() == 1);
    TEST_ASSERT(vrs2.GetIsParentVaultValid() == true);

    COMPARE_VAULT(vrs2, {
        {{"A", 0}, {"B",  std::string("a")}},
        {{"A", 0}, {"B",  std::string("b")}},
        {{"A", 0}, {"B",  std::string("c")}},
    });
)

TEST_BODY(MoveConstructor, MoveWithoutParents,
    VaultRecordSet vrs1;
    VaultRecordSet vrs2(std::move(vrs1));

    TEST_ASSERT(vrs2.Size() == 0);
    TEST_ASSERT(vrs2.GetKeys().size() == 0);
    TEST_ASSERT(vrs2.GetUniqueKeys().size() == 0);
    TEST_ASSERT(vrs2.GetIsParentVaultValid() == false);
    TEST_ASSERT(vrs2.GetParentVaultUniqueId() == "null");

    COMPARE_VAULT(vrs2, {});
)

TEST_BODY(MoveConstructor, MoveEmpty,
    Vault vlt;
    GENERATE_SET(vrs1);
    VaultRecordSet vrs2(std::move(vrs1));

    TEST_ASSERT(vrs2.Size() == 0);
    TEST_ASSERT(vrs2.GetKeys().size() == 0);
    TEST_ASSERT(vrs2.GetUniqueKeys().size() == 0);
    TEST_ASSERT(vrs2.GetIsParentVaultValid() == true);
    COMPARE_VAULT(vrs2, {});

    TEST_ASSERT(vrs1.Size() == 0);
    TEST_ASSERT(vrs1.GetKeys().size() == 0);
    TEST_ASSERT(vrs1.GetUniqueKeys().size() == 0);
    TEST_ASSERT(vrs1.GetIsParentVaultValid() == false);
    TEST_ASSERT(vrs1.GetParentVaultUniqueId() == "null");
    COMPARE_VAULT(vrs1, {});
)

TEST_BODY(MoveConstructor, MoveFilled,
    Vault vlt;
    VaultRecordSet vrs1;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");
    
    vlt.CreateRecord({ {"A", 0}, {"B", std::string("a")} });
    vlt.CreateRecord({ {"A", 0}, {"B", std::string("b")} });
    vlt.CreateRecord({ {"A", 0}, {"B", std::string("c")} });

    vlt.RequestEqual("A", 0, vrs1);

    VaultRecordSet vrs2(std::move(vrs1));

    TEST_ASSERT(vrs2.Size() == 3);
    TEST_ASSERT(vrs2.GetKeys().size() == 2);
    TEST_ASSERT(vrs2.GetUniqueKeys().size() == 1);
    TEST_ASSERT(vrs2.GetIsParentVaultValid() == true);

    COMPARE_VAULT(vrs2, {
        {{"A", 0}, {"B",  std::string("a")}},
        {{"A", 0}, {"B",  std::string("b")}},
        {{"A", 0}, {"B",  std::string("c")}},
    });

    TEST_ASSERT(vrs1.Size() == 0);
    TEST_ASSERT(vrs1.GetKeys().size() == 0);
    TEST_ASSERT(vrs1.GetUniqueKeys().size() == 0);
    TEST_ASSERT(vrs1.GetIsParentVaultValid() == false);
    TEST_ASSERT(vrs1.GetParentVaultUniqueId() == "null");
    COMPARE_VAULT(vrs1, {});
)

TEST_BODY(MoveAssignmentOperator, AssignWithoutParents,
    VaultRecordSet vrs1, vrs2;
    vrs2 = std::move(vrs1);

    TEST_ASSERT(vrs2.Size() == 0);
    TEST_ASSERT(vrs2.GetKeys().size() == 0);
    TEST_ASSERT(vrs2.GetUniqueKeys().size() == 0);
    TEST_ASSERT(vrs2.GetIsParentVaultValid() == false);
    TEST_ASSERT(vrs1.GetParentVaultUniqueId() == "null");

    COMPARE_VAULT(vrs2, {});
)

TEST_BODY(MoveAssignmentOperator, AssignWithParentsToWithoutParent,
    Vault vlt;
    VaultRecordSet vrs1, vrs2;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");
    
    vlt.CreateRecord({ {"A", 0}, {"B", std::string("a")} });
    vlt.CreateRecord({ {"A", 0}, {"B", std::string("b")} });
    vlt.CreateRecord({ {"A", 0}, {"B", std::string("c")} });

    vlt.RequestEqual("A", 0, vrs1);

    vrs1 = std::move(vrs2);

    TEST_ASSERT(vrs1.Size() == 0);
    TEST_ASSERT(vrs1.GetKeys().size() == 0);
    TEST_ASSERT(vrs1.GetUniqueKeys().size() == 0);
    TEST_ASSERT(vrs1.GetIsParentVaultValid() == false);
    TEST_ASSERT(vrs1.GetParentVaultUniqueId() == "null");

    COMPARE_VAULT(vrs1, {});
)

TEST_BODY(MoveAssignmentOperator, AssignEmpty,
    Vault vlt;
    GENERATE_SET(vrs1);
    VaultRecordSet vrs2;

    vrs2 = std::move(vrs1);

    TEST_ASSERT(vrs2.Size() == 0);
    TEST_ASSERT(vrs2.GetKeys().size() == 0);
    TEST_ASSERT(vrs2.GetUniqueKeys().size() == 0);
    TEST_ASSERT(vrs2.GetIsParentVaultValid() == true);

    COMPARE_VAULT(vrs2, {});
)

TEST_BODY(MoveAssignmentOperator, AssignFilledSet,
    Vault vlt;
    VaultRecordSet vrs1, vrs2;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");
    
    vlt.CreateRecord({ {"A", 0}, {"B", std::string("a")} });
    vlt.CreateRecord({ {"A", 0}, {"B", std::string("b")} });
    vlt.CreateRecord({ {"A", 0}, {"B", std::string("c")} });

    vlt.RequestEqual("A", 0, vrs1);

    vrs2 = std::move(vrs1);

    TEST_ASSERT(vrs2.Size() == 3);
    TEST_ASSERT(vrs2.GetKeys().size() == 2);
    TEST_ASSERT(vrs2.GetUniqueKeys().size() == 1);
    TEST_ASSERT(vrs2.GetIsParentVaultValid() == true);

    COMPARE_VAULT(vrs2, {
        {{"A", 0}, {"B",  std::string("a")}},
        {{"A", 0}, {"B",  std::string("b")}},
        {{"A", 0}, {"B",  std::string("c")}},
    });

    TEST_ASSERT(vrs1.Size() == 0);
    TEST_ASSERT(vrs1.GetKeys().size() == 0);
    TEST_ASSERT(vrs1.GetUniqueKeys().size() == 0);
    TEST_ASSERT(vrs1.GetIsParentVaultValid() == false);
    TEST_ASSERT(vrs1.GetParentVaultUniqueId() == "null");

    COMPARE_VAULT(vrs1, {});
)

void VaultRecordSetUnitTests()
{
    DefaultConstructor::Default();

    CopyConstructor::CopyWithoutParent();
    CopyConstructor::CopyEmptySet();
    CopyConstructor::CopyFilledSet();

    AssignmentOperator::AssignWithoutParents();
    AssignmentOperator::AssignWithParentsToWithoutParent();
    AssignmentOperator::AssignEmpty();
    AssignmentOperator::AssignFilledSet();

    MoveConstructor::MoveWithoutParents();
    MoveConstructor::MoveEmpty();
    MoveConstructor::MoveFilled();

    MoveAssignmentOperator::AssignWithoutParents();
    MoveAssignmentOperator::AssignWithParentsToWithoutParent();
    MoveAssignmentOperator::AssignEmpty();
    MoveAssignmentOperator::AssignFilledSet();
}