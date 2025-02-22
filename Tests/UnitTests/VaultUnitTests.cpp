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

TEST_BODY(AddUniqueKey, CorrectAddKeyToEmptyVault,
    Vault vlt;
    VaultOperationResult vor;

    vor = vlt.AddUniqueKey<int>("A");

    TEST_ASSERT(vlt.GetKeys().size() == 1);

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(void));
)

TEST_BODY(AddUniqueKey, IncorrectAddKeyToEmptyVault,
    Vault vlt;
    VaultOperationResult vor;

    vlt.AddUniqueKey<int>("A");

    TEST_ASSERT(vlt.GetKeys().size() == 1);

    vor = vlt.AddUniqueKey<std::string>("A");

    TEST_ASSERT(vlt.GetKeys().size() == 1);

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(std::string), ResultCode == VaultOperationResultCode::DuplicateKey, SavedType == typeid(int));
)

TEST_BODY(AddUniqueKey, CorrectAddKeyToNonEmptyVault,
    Vault vlt;
    VaultOperationResult vor;

    vlt.AddUniqueKey<std::string>("A");

    vlt.CreateRecord({{"A", std::string("a")}});
    vlt.CreateRecord({{"A", std::string("b")}});
    vlt.CreateRecord({{"A", std::string("c")}});

    vor = vlt.AddUniqueKey<std::string>("B", [](const std::size_t& counter, const VaultRecordRef& ref) -> std::string
    {
        std::string str;
        ref.GetData("A", str);
        return str += std::to_string(counter);
    });

    TEST_ASSERT(vlt.GetKeys().size() == 2);

    COMPARE_VAULT(vlt, {
        {{"A", std::string("a")}, {"B", std::string("a2")}},
        {{"A", std::string("b")}, {"B", std::string("b1")}},
        {{"A", std::string("c")}, {"B", std::string("c0")}},
    });

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "B", 
        RequestedType == typeid(std::string), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(void));
)

TEST_BODY(AddUniqueKey, IncorrectAddKeyToNonEmptyVault,
    Vault vlt;
    VaultOperationResult vor;

    vlt.AddUniqueKey<std::string>("A");

    vlt.CreateRecord({{"A", std::string("a")}});
    vlt.CreateRecord({{"A", std::string("b")}});
    vlt.CreateRecord({{"A", std::string("c")}});

    vor = vlt.AddUniqueKey<std::string>("B");

    TEST_ASSERT(vlt.GetKeys().size() == 1);

    COMPARE_VAULT(vlt, {
        {{"A", std::string("a")}},
        {{"A", std::string("b")}},
        {{"A", std::string("c")}},
    });

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "B", 
        RequestedType == typeid(std::string), ResultCode == VaultOperationResultCode::TryToAddUniqueKeyInNonEmptyVaultWithoutLambda, SavedType == typeid(void));
)

TEST_BODY(AddUniqueKey, CorrectAddKeyToNonEmptyVaultWithDuplicate,
    Vault vlt;
    VaultOperationResult vor;

    vlt.AddUniqueKey<std::string>("A");

    vlt.CreateRecord({{"A", std::string("a")}});
    vlt.CreateRecord({{"A", std::string("b")}});
    vlt.CreateRecord({{"A", std::string("c")}});

    vor = vlt.AddUniqueKey<int>("B", [](const std::size_t& counter, const VaultRecordRef& ref) -> int
    {
        if (counter > 0) return 1;
        else return counter;
    });

    TEST_ASSERT(vlt.GetKeys().size() == 1);

    COMPARE_VAULT(vlt, {
        {{"A", std::string("a")}},
        {{"A", std::string("b")}},
        {{"A", std::string("c")}},
    });

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "B", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::UniqueKeyValueAlredyInSet, SavedType == typeid(void));
)

TEST_BODY(UpdateKey, CorrectUpdateNonUniqueKey,
    Vault vlt;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddKey<std::string>("B", "none");

    vor = vlt.UpdateKey("A", -1);

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));

    vor = vlt.UpdateKey<std::string>("B", "empty");

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "B", 
        RequestedType == typeid(std::string), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(std::string));

    vlt.CreateRecord({});

    COMPARE_VAULT(vlt, {
        {{"A", -1}, {"B", std::string("empty")}}
    });
)

TEST_BODY(UpdateKey, WrongKeyUpdate,
    Vault vlt;
    VaultOperationResult vor;

    vor = vlt.UpdateKey("A", 0);

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongKey, SavedType == typeid(void));
)

TEST_BODY(UpdateKey, WrongType,
    Vault vlt;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);

    vor = vlt.UpdateKey<std::string>("A", "");

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(std::string), ResultCode == VaultOperationResultCode::WrongType, SavedType == typeid(int));
)

TEST_BODY(UpdateKey, UniqueKey,
    Vault vlt;
    VaultOperationResult vor;

    vlt.AddUniqueKey<std::string>("A");

    vor = vlt.UpdateKey<std::string>("A", "");

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(std::string), ResultCode == VaultOperationResultCode::UniqueKey, SavedType == typeid(std::string));
)

TEST_BODY(IsKeyExist, ExistingKey,
    Vault vlt;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<int>("B");

    TEST_ASSERT(vlt.IsKeyExist("A"));
    TEST_ASSERT(vlt.IsKeyExist("B"));
)

TEST_BODY(IsKeyExist, NonExistingKey,
    Vault vlt;
    TEST_ASSERT(vlt.IsKeyExist("A") == false);
)

TEST_BODY(GetKeyValue, ExistingKey,
    Vault vlt;
    VaultOperationResult vor;

    vlt.AddKey("A", -1);

    int i = 0;
    vor = vlt.GetKeyValue("A", i);

    TEST_ASSERT(i == -1);

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));
)

TEST_BODY(GetKeyValue, NonExistingKey,
    Vault vlt;
    VaultOperationResult vor;

    int i = 0;
    vor = vlt.GetKeyValue("A", i);

    TEST_ASSERT(i == 0);

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongKey, SavedType == typeid(void));
)

TEST_BODY(GetKeyValue, WrongType,
    Vault vlt;
    VaultOperationResult vor;

    vlt.AddKey("A", -1);

    std::string s;
    vor = vlt.GetKeyValue("A", s);

    TEST_ASSERT(s.empty());

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(std::string), ResultCode == VaultOperationResultCode::WrongType, SavedType == typeid(int));
)

TEST_BODY(GetKeyValue, UniqueKey,
    Vault vlt;
    VaultOperationResult vor;

    vlt.AddUniqueKey<int>("A");

    int i = 0;
    vor = vlt.GetKeyValue("A", i);

    TEST_ASSERT(i == 0);

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::UniqueKey, SavedType == typeid(int));
)

TEST_BODY(GetKeyType, ExistingKey,
    Vault vlt;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    std::type_index type = typeid(void);

    TEST_ASSERT(vlt.GetKeyType("A", type));
    TEST_ASSERT(type == typeid(int));

    TEST_ASSERT(vlt.GetKeyType("B", type));
    TEST_ASSERT(type == typeid(std::string));
)

TEST_BODY(GetKeyType, NonExistingKey,
    Vault vlt;

    std::type_index type = typeid(void);

    TEST_ASSERT(vlt.GetKeyType("A", type) == false);
    TEST_ASSERT(type == typeid(void));
)

TEST_BODY(GetKeys, Keys,
    Vault vlt;

    TEST_ASSERT(vlt.GetKeys() == std::vector<std::string>{});

    vlt.AddKey("A", 0);
    TEST_ASSERT(vlt.GetKeys() == std::vector<std::string>{"A"});

    vlt.AddKey("B", 0);
    TEST_ASSERT(vlt.GetKeys() == std::vector<std::string>({"A", "B"}));

    vlt.AddUniqueKey<std::string>("C");
    TEST_ASSERT(vlt.GetKeys() == std::vector<std::string>({"A", "B", "C"}));
)

TEST_BODY(GetUniqueKeys, UniqueKeys,
    Vault vlt;

    TEST_ASSERT(vlt.GetUniqueKeys() == std::vector<std::string>{});

    vlt.AddKey("A", 0);
    TEST_ASSERT(vlt.GetUniqueKeys() == std::vector<std::string>{});

    vlt.AddUniqueKey<int>("B");
    TEST_ASSERT(vlt.GetUniqueKeys() == std::vector<std::string>{"B"});

    vlt.AddUniqueKey<std::string>("C");
    TEST_ASSERT(vlt.GetUniqueKeys() == std::vector<std::string>({"B", "C"}));
)

TEST_BODY(RemoveKey, CorrectRemoveFromEmptyVault,
    Vault vlt;

    vlt.AddKey("A", 0);
    vlt.AddKey("B", 0);
    vlt.AddUniqueKey<std::string>("C");

    TEST_ASSERT(vlt.RemoveKey("B"));
    TEST_ASSERT(vlt.RemoveKey("C"));

    TEST_ASSERT(vlt.GetKeys().size() == 1);
)

TEST_BODY(RemoveKey, CorrectRemoveFromNonEmptyVault,
    Vault vlt;

    vlt.AddKey("A", 0);
    vlt.AddKey("B", 0);
    vlt.AddUniqueKey<std::string>("C");

    vlt.CreateRecord({{"A", 1}, {"B", 3}, {"C", std::string("a")}});
    vlt.CreateRecord({{"A", 2}, {"B", 2}, {"C", std::string("b")}});
    vlt.CreateRecord({{"A", 3}, {"B", 1}, {"C", std::string("c")}});
    
    TEST_ASSERT(vlt.RemoveKey("B"));
    TEST_ASSERT(vlt.RemoveKey("C"));

    TEST_ASSERT(vlt.GetKeys().size() == 1);
    COMPARE_VAULT(vlt, {
        {{"A", 1}},
        {{"A", 2}},
        {{"A", 3}},
    })
)

TEST_BODY(RemoveKey, IncorrectRemoveFromEmptyVault,
    Vault vlt;

    vlt.AddKey("A", 0);
    vlt.AddKey("B", 0);
    vlt.AddUniqueKey<std::string>("C");

    TEST_ASSERT(vlt.RemoveKey("D") == false);

    TEST_ASSERT(vlt.GetKeys().size() == 3);
)

TEST_BODY(RemoveKey, IncorrectRemoveFromNonEmptyVault,
    Vault vlt;

    vlt.AddKey("A", 0);
    vlt.AddKey("B", 0);
    vlt.AddUniqueKey<std::string>("C");

    vlt.CreateRecord({{"A", 1}, {"B", 3}, {"C", std::string("a")}});
    vlt.CreateRecord({{"A", 2}, {"B", 2}, {"C", std::string("b")}});
    vlt.CreateRecord({{"A", 3}, {"B", 1}, {"C", std::string("c")}});
    
    TEST_ASSERT(vlt.RemoveKey("D") == false);

    TEST_ASSERT(vlt.GetKeys().size() == 3);
    COMPARE_VAULT(vlt, {
        {{"A", 1}, {"B", 3}, {"C", std::string("a")}},
        {{"A", 2}, {"B", 2}, {"C", std::string("b")}},
        {{"A", 3}, {"B", 1}, {"C", std::string("c")}},
    })
)

TEST_BODY(CreateKey, CorrectParamsWithoutUniqueKey,
    Vault vlt;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddKey<std::string>("B", "none");

    vlt.CreateRecord({});
    vlt.CreateRecord({});
    vlt.CreateRecord({{"A", 2}});
    vlt.CreateRecord({{"B", std::string("c")}});
    vlt.CreateRecord({{"A", 4}, {"B", std::string("d")}});
    vor = vlt.CreateRecord({{"B", std::string("e")}, {"A", 5}});

    COMPARE_VAULT(vlt, {
        {{"A", 0}, {"B", std::string("none")}},
        {{"A", 0}, {"B", std::string("none")}},
        {{"A", 2}, {"B", std::string("none")}},
        {{"A", 0}, {"B", std::string("c")}},
        {{"A", 4}, {"B", std::string("d")}},
        {{"A", 5}, {"B", std::string("e")}},
    })

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(void));

    // Function version with VaultRecordRef
    vor = vlt.CreateRecord(vrr, {{"A", 10}, {"B", std::string("z")}});
    
    int a = 0;
    std::string b;

    vrr.GetData("A", a);
    TEST_ASSERT(a == 10);

    vrr.GetData("B", b);
    TEST_ASSERT(b == "z");

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "B", 
        RequestedType == typeid(std::string), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(void));
)

TEST_BODY(CreateKey, CorrectParamsWithUniqueKey,
    Vault vlt;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"B", std::string("a")}});
    vlt.CreateRecord({{"A", 1}, {"B", std::string("b")}});
    vor = vlt.CreateRecord({{"B", std::string("c")}, {"A", 2}});

    COMPARE_VAULT(vlt, {
        {{"A", 0}, {"B", std::string("a")}},
        {{"A", 1}, {"B", std::string("b")}},
        {{"A", 2}, {"B", std::string("c")}},
    })

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(void));

    // Function version with VaultRecordRef
    vor = vlt.CreateRecord(vrr, {{"A", 10}, {"B", std::string("z")}});
    
    int a = 0;
    std::string b;

    vrr.GetData("A", a);
    TEST_ASSERT(a == 10);

    vrr.GetData("B", b);
    TEST_ASSERT(b == "z");

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "B", 
        RequestedType == typeid(std::string), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(void));
)

TEST_BODY(CreateKey, WrongKeyWithoutUniqueKey,
    Vault vlt;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddKey<std::string>("B", "none");

    vor = vlt.CreateRecord({{"C", 0}});

    COMPARE_VAULT(vlt, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "C", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongKey, SavedType == typeid(void));
)

TEST_BODY(CreateKey, WrongTypeWithoutUniqueKey,
    Vault vlt;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddKey<std::string>("B", "none");

    vor = vlt.CreateRecord({{"B", 0}});

    COMPARE_VAULT(vlt, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "B", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongType, SavedType == typeid(std::string));
)

TEST_BODY(CreateKey, WrongKeyWithUniqueKey,
    Vault vlt;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vor = vlt.CreateRecord({{"C", 0}});

    COMPARE_VAULT(vlt, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "C", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongKey, SavedType == typeid(void));
)

TEST_BODY(CreateKey, WrongTypeWithUniqueKey,
    Vault vlt;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vor = vlt.CreateRecord({{"B", 0}});

    COMPARE_VAULT(vlt, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "B", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongType, SavedType == typeid(std::string));
)

TEST_BODY(CreateKey, DuplicateUniqueKeyValue,
    Vault vlt;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 1}, {"B", std::string("none")}});


    vor = vlt.CreateRecord({{"B", std::string("none")}});

    COMPARE_VAULT(vlt, {{{"A", 1}, {"B", std::string("none")}}});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "B", 
        RequestedType == typeid(std::string), ResultCode == VaultOperationResultCode::UniqueKeyValueAlredyInSet, SavedType == typeid(void));


    vor = vlt.CreateRecord({{"A", 2}, {"B", std::string("none")}});

    COMPARE_VAULT(vlt, {{{"A", 1}, {"B", std::string("none")}}});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "B", 
        RequestedType == typeid(std::string), ResultCode == VaultOperationResultCode::UniqueKeyValueAlredyInSet, SavedType == typeid(void));
)

void VaultUnitTests()
{
    DBG_LOG_ENTER();
    SetBackTraceFormat(BackTraceFormat::None);

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

    AddUniqueKey::CorrectAddKeyToEmptyVault();
    AddUniqueKey::IncorrectAddKeyToEmptyVault();
    AddUniqueKey::CorrectAddKeyToNonEmptyVault();
    AddUniqueKey::IncorrectAddKeyToNonEmptyVault();
    AddUniqueKey::CorrectAddKeyToNonEmptyVaultWithDuplicate();

    UpdateKey::CorrectUpdateNonUniqueKey();
    UpdateKey::WrongKeyUpdate();
    UpdateKey::WrongType();
    UpdateKey::UniqueKey();

    IsKeyExist::ExistingKey();
    IsKeyExist::NonExistingKey();

    GetKeyValue::ExistingKey();
    GetKeyValue::NonExistingKey();
    GetKeyValue::WrongType();
    GetKeyValue::UniqueKey();

    GetKeyType::ExistingKey();
    GetKeyType::NonExistingKey();

    GetKeys::Keys();

    GetUniqueKeys::UniqueKeys();

    RemoveKey::CorrectRemoveFromEmptyVault();
    RemoveKey::CorrectRemoveFromNonEmptyVault();
    RemoveKey::IncorrectRemoveFromEmptyVault();
    RemoveKey::IncorrectRemoveFromNonEmptyVault();

    CreateKey::CorrectParamsWithoutUniqueKey();
    CreateKey::CorrectParamsWithUniqueKey();
    CreateKey::WrongKeyWithoutUniqueKey();
    CreateKey::WrongTypeWithoutUniqueKey();
    CreateKey::WrongKeyWithUniqueKey();
    CreateKey::WrongTypeWithUniqueKey();
    CreateKey::DuplicateUniqueKeyValue();
}