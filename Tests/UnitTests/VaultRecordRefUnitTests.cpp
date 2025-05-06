#include "VaultRecordRefUnitTests.h"

namespace VaultRecordRefTestNs
{
std::vector<std::pair<std::string, std::function<void()>>> Tests;

TEST_BODY(DefaultConstructor, Default,
    VaultRecordRef vrr;

    TEST_ASSERT(vrr.IsValid() == false);
)

TEST_BODY(CopyConstructor, FromEmpty,
    VaultRecordRef vrr1;
    VaultRecordRef vrr2(vrr1);

    TEST_ASSERT(vrr1 == vrr2);
    TEST_ASSERT(vrr1.IsValid() == false);
    TEST_ASSERT(vrr2.IsValid() == false);
)

TEST_BODY(CopyConstructor, FromFilled,
    Vault vlt;
    VaultRecordRef vrr1;

    vlt.AddKey("A", 1);
    vlt.CreateRecord(vrr1, {});

    VaultRecordRef vrr2(vrr1);

    TEST_ASSERT(vrr1 == vrr2);
    
    int i = 0;
    vrr2.GetData("A", i);
    TEST_ASSERT(i == 1);

    vrr1.GetData("A", i);
    TEST_ASSERT(i == 1);
)

TEST_BODY(MoveConstructor, FromEmpty,
    VaultRecordRef vrr1;
    VaultRecordRef vrr2(std::move(vrr1));

    TEST_ASSERT(vrr1 == vrr2);
    TEST_ASSERT(vrr1.IsValid() == false);
    TEST_ASSERT(vrr2.IsValid() == false);
)

TEST_BODY(MoveConstructor, FromFilled,
    Vault vlt;
    VaultRecordRef vrr1;

    vlt.AddKey("A", 1);
    vlt.CreateRecord(vrr1, {});

    VaultRecordRef vrr2(std::move(vrr1));

    TEST_ASSERT(vrr1 != vrr2);
    
    int i = 0;
    vrr2.GetData("A", i);
    TEST_ASSERT(i == 1);

    TEST_ASSERT(vrr1.IsValid() == false);
)

TEST_BODY(AssignmentOperator, AssignWithoutParents,
    VaultRecordRef vrr1, vrr2;

    vrr1 = vrr2;

    TEST_ASSERT(vrr1.IsValid() == false);
    TEST_ASSERT(vrr1.GetRecordUniqueId() == "null");
)

TEST_BODY(AssignmentOperator, AssignWithParentsToWithoutParent,
    Vault vlt;
    VaultRecordRef vrr1, vrr2;

    vlt.AddKey("A", 0);
    vlt.CreateRecord({{"A", 0}});
    vlt.GetRecord("A", 0, vrr1);

    vrr2 = vrr1;

    TEST_ASSERT(vrr2.IsValid() == true);
    TEST_ASSERT(vrr2.ToJson(false) == R"({"A":0})");
)

TEST_BODY(AssignmentOperator, AssignWithoutParentToWithParent,
    Vault vlt;
    VaultRecordRef vrr1, vrr2;

    vlt.AddKey("A", 0);
    vlt.CreateRecord({{"A", 0}});
    vlt.GetRecord("A", 0, vrr2);

    vrr2 = vrr1;

    TEST_ASSERT(vrr1.IsValid() == false);
    TEST_ASSERT(vrr1.GetRecordUniqueId() == "null");
)

TEST_BODY(AssignmentOperator, Assign,
    Vault vlt;
    VaultRecordRef vrr1, vrr2;

    vlt.AddKey("A", 0);
    vlt.CreateRecord({{"A", 0}});
    vlt.CreateRecord({{"A", 1}});
    vlt.GetRecord("A", 0, vrr1);
    vlt.GetRecord("A", 1, vrr2);

    vrr2 = vrr1;

    TEST_ASSERT(vrr2.IsValid() == true);
    TEST_ASSERT(vrr2.ToJson(false) == R"({"A":0})");
)


TEST_BODY(MoveAssignmentOperator, AssignWithoutParents,
    VaultRecordRef vrr1, vrr2;

    vrr1 = std::move(vrr2);

    TEST_ASSERT(vrr1.IsValid() == false);
    TEST_ASSERT(vrr1.GetRecordUniqueId() == "null");
)

TEST_BODY(MoveAssignmentOperator, AssignWithParentsToWithoutParent,
    Vault vlt;
    VaultRecordRef vrr1, vrr2;

    vlt.AddKey("A", 0);
    vlt.CreateRecord({{"A", 0}});
    vlt.GetRecord("A", 0, vrr1);

    vrr2 = std::move(vrr1);

    TEST_ASSERT(vrr1.IsValid() == false);
    TEST_ASSERT(vrr1.GetRecordUniqueId() == "null");
    TEST_ASSERT(vrr2.IsValid() == true);
    TEST_ASSERT(vrr2.ToJson(false) == R"({"A":0})");
)

TEST_BODY(MoveAssignmentOperator, AssignWithoutParentToWithParent,
    Vault vlt;
    VaultRecordRef vrr1, vrr2;

    vlt.AddKey("A", 0);
    vlt.CreateRecord({{"A", 0}});
    vlt.GetRecord("A", 0, vrr2);

    vrr2 = std::move(vrr1);

    TEST_ASSERT(vrr1.IsValid() == false);
    TEST_ASSERT(vrr1.GetRecordUniqueId() == "null");
)

TEST_BODY(MoveAssignmentOperator, Assign,
    Vault vlt;
    VaultRecordRef vrr1, vrr2;

    vlt.AddKey("A", 0);
    vlt.CreateRecord({{"A", 0}});
    vlt.CreateRecord({{"A", 1}});
    vlt.GetRecord("A", 0, vrr1);
    vlt.GetRecord("A", 1, vrr2);

    vrr2 = std::move(vrr1);

    TEST_ASSERT(vrr1.IsValid() == false);
    TEST_ASSERT(vrr1.GetRecordUniqueId() == "null");
    TEST_ASSERT(vrr2.IsValid() == true);
    TEST_ASSERT(vrr2.ToJson(false) == R"({"A":0})");
)

TEST_BODY(OperatorComparison, CompareWithoutParents,
    VaultRecordRef vrr1, vrr2;

    TEST_ASSERT(vrr1 == vrr2);
)

TEST_BODY(OperatorComparison, CompareWithParentToWithoutParent,
    Vault vlt;
    VaultRecordRef vrr1, vrr2;

    vlt.AddKey("A", 0);
    vlt.CreateRecord({{"A", 0}});
    vlt.GetRecord("A", 0, vrr2);

    TEST_ASSERT(!(vrr1 == vrr2));
    TEST_ASSERT(!(vrr2 == vrr1));
)

TEST_BODY(OperatorComparison, Compare,
    Vault vlt;
    VaultRecordRef vrr1, vrr2;

    vlt.AddKey("A", 0);
    vlt.CreateRecord({{"A", 0}});
    vlt.CreateRecord({{"A", 1}});
    vlt.GetRecord("A", 0, vrr1);
    vlt.GetRecord("A", 0, vrr2);

    TEST_ASSERT(vrr1 == vrr2);
    
    vlt.GetRecord("A", 1, vrr2);
    TEST_ASSERT(!(vrr1 == vrr2));
)

TEST_BODY(OperatorNonComparison, CompareWithoutParents,
    VaultRecordRef vrr1, vrr2;

    TEST_ASSERT(!(vrr1 != vrr2));
)

TEST_BODY(OperatorNonComparison, CompareWithParentToWithoutParent,
    Vault vlt;
    VaultRecordRef vrr1, vrr2;

    vlt.AddKey("A", 0);
    vlt.CreateRecord({{"A", 0}});
    vlt.GetRecord("A", 0, vrr2);

    TEST_ASSERT(vrr1 != vrr2);
    TEST_ASSERT(vrr2 != vrr1);
)

TEST_BODY(OperatorNonComparison, Compare,
    Vault vlt;
    VaultRecordRef vrr1, vrr2;

    vlt.AddKey("A", 0);
    vlt.CreateRecord({{"A", 0}});
    vlt.CreateRecord({{"A", 1}});
    vlt.GetRecord("A", 0, vrr1);
    vlt.GetRecord("A", 0, vrr2);

    TEST_ASSERT(!(vrr1 != vrr2));
    
    vlt.GetRecord("A", 1, vrr2);
    TEST_ASSERT(vrr1 != vrr2);
)
}

void VaultRecordRefUnitTests(const std::string& testName)
{
    DBG_LOG_ENTER();
    SetBackTraceFormat(BackTraceFormat::None);

    for (const auto& test : VaultRecordRefTestNs::Tests)
    {
        if (testName.empty() || testName == test.first)
            test.second();
    }
}