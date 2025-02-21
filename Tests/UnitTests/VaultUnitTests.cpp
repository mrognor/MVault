#include "VaultUnitTests.h"

TEST_BODY(DefaultConstructor, Default,
    Vault vlt;

    TEST_ASSERT(vlt.Size() == 0);
    TEST_ASSERT(vlt.GetKeys().size() == 0);

    COMPARE_VAULT(vlt, {});
)

TEST_BODY(CopyConstructor, CopyEmptyVault,
    Vault vlt1;
    Vault vlt2(vlt1);

    TEST_ASSERT(vlt2.Size() == 0);
    TEST_ASSERT(vlt2.GetKeys().size() == 0);

    COMPARE_VAULT(vlt2, {});
)

TEST_BODY(CopyConstructor, CopyFilledVault,
    Vault vlt1;

    vlt1.AddKey("A", 0);
    vlt1.AddKey<std::string>("B", "");
    
    vlt1.CreateRecord({ {"A", 1}, {"B", std::string("a")} });
    vlt1.CreateRecord({ {"A", 2}, {"B", std::string("b")} });
    vlt1.CreateRecord({ {"A", 3}, {"B", std::string("c")} });

    Vault vlt2(vlt1);

    TEST_ASSERT(vlt2.Size() == 3);
    TEST_ASSERT(vlt2.GetKeys().size() == 2);

    COMPARE_VAULT(vlt2, {
        {{"A", 1}, {"B",  std::string("a")}},
        {{"A", 2}, {"B",  std::string("b")}},
        {{"A", 3}, {"B",  std::string("c")}},
    });
)

TEST_BODY(AssignmentOperator, AssignEmptyVault,
    Vault vlt1;
    Vault vlt2;

    vlt2 = vlt1;
    TEST_ASSERT(vlt2.Size() == 0);
    TEST_ASSERT(vlt2.GetKeys().size() == 0);

    COMPARE_VAULT(vlt2, {});
)

TEST_BODY(AssignmentOperator, AssignFilledToEmpty,
    Vault vlt1;
    Vault vlt2;

    vlt1.AddKey("A", 0);
    vlt1.AddKey<std::string>("B", "");
    
    vlt1.CreateRecord({ {"A", 1}, {"B", std::string("a")} });
    vlt1.CreateRecord({ {"A", 2}, {"B", std::string("b")} });
    vlt1.CreateRecord({ {"A", 3}, {"B", std::string("c")} });

    vlt2 = vlt1;

    TEST_ASSERT(vlt2.Size() == 3);
    TEST_ASSERT(vlt2.GetKeys().size() == 2);

    COMPARE_VAULT(vlt2, {
        {{"A", 1}, {"B",  std::string("a")}},
        {{"A", 2}, {"B",  std::string("b")}},
        {{"A", 3}, {"B",  std::string("c")}},
    });
)

TEST_BODY(AssignmentOperator, AssignEmptyToFilled,
    Vault vlt1;
    Vault vlt2;

    vlt1.AddKey("A", 0);
    vlt1.AddKey<std::string>("B", "");
    
    vlt1.CreateRecord({ {"A", 1}, {"B", std::string("a")} });
    vlt1.CreateRecord({ {"A", 2}, {"B", std::string("b")} });
    vlt1.CreateRecord({ {"A", 3}, {"B", std::string("c")} });

    vlt1 = vlt2;

    TEST_ASSERT(vlt2.Size() == 0);
    TEST_ASSERT(vlt2.GetKeys().size() == 0);

    COMPARE_VAULT(vlt2, {});
)

TEST_BODY(MoveConstructor, MoveEmptyConstructor,
    Vault vlt1;
    Vault vlt2;

    vlt2 = vlt1;
    TEST_ASSERT(vlt2.Size() == 0);
    TEST_ASSERT(vlt2.GetKeys().size() == 0);

    COMPARE_VAULT(vlt2, {});
)

TEST_BODY(MoveConstructor, MoveFilledConstructor,
    Vault vlt1;

    vlt1.AddKey("A", 0);
    vlt1.AddKey<std::string>("B", "");
    
    vlt1.CreateRecord({ {"A", 1}, {"B", std::string("a")} });
    vlt1.CreateRecord({ {"A", 2}, {"B", std::string("b")} });
    vlt1.CreateRecord({ {"A", 3}, {"B", std::string("c")} });

    Vault vlt2(std::move(vlt1));

    TEST_ASSERT(vlt1.Size() == 0);
    TEST_ASSERT(vlt1.GetKeys().size() == 0);

    COMPARE_VAULT(vlt1, {});

    TEST_ASSERT(vlt2.Size() == 3);
    TEST_ASSERT(vlt2.GetKeys().size() == 2);

    COMPARE_VAULT(vlt2, {
        {{"A", 1}, {"B",  std::string("a")}},
        {{"A", 2}, {"B",  std::string("b")}},
        {{"A", 3}, {"B",  std::string("c")}},
    });
)

TEST_BODY(MoveAssignmentOperator, AssignEmptyVault,
    Vault vlt1;
    Vault vlt2;

    vlt2 = std::move(vlt1);
    TEST_ASSERT(vlt2.Size() == 0);
    TEST_ASSERT(vlt2.GetKeys().size() == 0);

    COMPARE_VAULT(vlt2, {});
)

TEST_BODY(MoveAssignmentOperator, AssignFilledToEmpty,
    Vault vlt1;
    Vault vlt2;

    vlt1.AddKey("A", 0);
    vlt1.AddKey<std::string>("B", "");
    
    vlt1.CreateRecord({ {"A", 1}, {"B", std::string("a")} });
    vlt1.CreateRecord({ {"A", 2}, {"B", std::string("b")} });
    vlt1.CreateRecord({ {"A", 3}, {"B", std::string("c")} });

    vlt2 = std::move(vlt1);

    TEST_ASSERT(vlt1.Size() == 0);
    TEST_ASSERT(vlt1.GetKeys().size() == 0);

    COMPARE_VAULT(vlt1, {});

    TEST_ASSERT(vlt2.Size() == 3);
    TEST_ASSERT(vlt2.GetKeys().size() == 2);

    COMPARE_VAULT(vlt2, {
        {{"A", 1}, {"B",  std::string("a")}},
        {{"A", 2}, {"B",  std::string("b")}},
        {{"A", 3}, {"B",  std::string("c")}},
    });
)

TEST_BODY(MoveAssignmentOperator, AssignEmptyToFilled,
    Vault vlt1;
    Vault vlt2;

    vlt1.AddKey("A", 0);
    vlt1.AddKey<std::string>("B", "");

    vlt1.CreateRecord({ {"A", 1}, {"B", std::string("a")} });
    vlt1.CreateRecord({ {"A", 2}, {"B", std::string("b")} });
    vlt1.CreateRecord({ {"A", 3}, {"B", std::string("c")} });

    vlt1 = std::move(vlt2);

    TEST_ASSERT(vlt2.Size() == 0);
    TEST_ASSERT(vlt2.GetKeys().size() == 0);

    COMPARE_VAULT(vlt2, {});
)

TEST_BODY(AddKey, CorrectAddKey,
    Vault vlt;
    bool res = false;

    res = vlt.AddKey("A", 0);
    TEST_ASSERT(res);
    TEST_ASSERT(vlt.GetKeys().size() == 1);

    res = vlt.AddKey<std::string>("B", "");
    TEST_ASSERT(res);
    TEST_ASSERT(vlt.GetKeys().size() == 2);
)

TEST_BODY(AddKey, IncorrectAddKey,
    Vault vlt;
    bool res = false;

    vlt.AddKey("A", 0);
    res = vlt.AddKey<std::string>("A", "");
    TEST_ASSERT(res == false);
)

TEST_BODY(AddKey, CorrectAddToNonEmpty,
    Vault vlt;

    vlt.AddKey("A", 0);

    vlt.CreateRecord({{"A", 1}});
    vlt.CreateRecord({{"A", 2}});
    vlt.CreateRecord({{"A", 3}});

    vlt.AddKey<std::string>("B", "added");

    TEST_ASSERT(vlt.GetKeys().size() == 2);

    COMPARE_VAULT(vlt, {
        {{"A", 1}, {"B",  std::string("added")}},
        {{"A", 2}, {"B",  std::string("added")}},
        {{"A", 3}, {"B",  std::string("added")}},
    });
)

TEST_BODY(AddKey, IncorrectAddToNonEmpty,
    Vault vlt;
    bool res = false;

    vlt.AddKey("A", 0);

    vlt.CreateRecord({{"A", 1}});
    vlt.CreateRecord({{"A", 2}});
    vlt.CreateRecord({{"A", 3}});

    res = vlt.AddKey<std::string>("A", "added");
    TEST_ASSERT(res == false);
)

TEST_BODY(AddUniqueKey, AddKeyToEmptyVault,
    Vault vlt;
    VaultOperationResult vor;

    vor = vlt.AddUniqueKey<int>("A");

    TEST_ASSERT(vlt.GetKeys().size() == 1);

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(void));
)

void VaultUnitTests()
{
    DefaultConstructor::Default();

    CopyConstructor::CopyEmptyVault();
    CopyConstructor::CopyFilledVault();

    AssignmentOperator::AssignEmptyVault();
    AssignmentOperator::AssignFilledToEmpty();
    AssignmentOperator::AssignEmptyToFilled();

    MoveConstructor::MoveEmptyConstructor();
    MoveConstructor::MoveFilledConstructor();

    MoveAssignmentOperator::AssignEmptyVault();
    MoveAssignmentOperator::AssignFilledToEmpty();
    MoveAssignmentOperator::AssignEmptyToFilled();

    AddKey::CorrectAddKey();
    AddKey::IncorrectAddKey();
    AddKey::CorrectAddToNonEmpty();
    AddKey::IncorrectAddToNonEmpty();

    AddUniqueKey::AddKeyToEmptyVault();
}