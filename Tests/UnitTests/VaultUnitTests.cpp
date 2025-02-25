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

TEST_BODY(CreateKey, DuplicateParams,
    Vault vlt;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);

    vlt.CreateRecord({{"A", 1}, {"A", 2}});
    vlt.CreateRecord({{"A", 1}, {"A", 2}, {"A", 3}});
    vor = vlt.CreateRecord({{"A", 2}, {"A", 1}});

    COMPARE_VAULT(vlt, {
        {{"A", 1}},
        {{"A", 2}},
        {{"A", 3}},
    })

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(void));
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

TEST_BODY(GetRecord, CorrectGetRecord,
    Vault vlt;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 1}, {"B", std::string("a")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("b")}});
    vlt.CreateRecord({{"A", 3}, {"B", std::string("c")}});

    vor = vlt.GetRecord("A", 1, vrr);
    
    COMPARE_REF(vrr, {{{"A", 1}, {"B", std::string("a")}}});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));


    vor = vlt.GetRecord<std::string>("B", "c", vrr);
    
    COMPARE_REF(vrr, {{{"A", 3}, {"B", std::string("c")}}});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "B", 
        RequestedType == typeid(std::string), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(std::string));
)

TEST_BODY(GetRecord, WrongKey,
    Vault vlt;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 1}, {"B", std::string("a")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("b")}});
    vlt.CreateRecord({{"A", 3}, {"B", std::string("c")}});

    vor = vlt.GetRecord("Z", 1, vrr);

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "Z", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongKey, SavedType == typeid(void));
)

TEST_BODY(GetRecord, WrongType,
    Vault vlt;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 1}, {"B", std::string("a")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("b")}});
    vlt.CreateRecord({{"A", 3}, {"B", std::string("c")}});

    vor = vlt.GetRecord("B", 1, vrr);

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "B", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongType, SavedType == typeid(std::string));
)

TEST_BODY(GetRecord, WrongValue,
    Vault vlt;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 1}, {"B", std::string("a")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("b")}});
    vlt.CreateRecord({{"A", 3}, {"B", std::string("c")}});

    vor = vlt.GetRecord("A", 99, vrr);

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongValue, SavedType == typeid(int));
)

TEST_BODY(GetRecords, CorrectGetRecords,
    Vault vlt;
    std::vector<VaultRecordRef> records;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 1}, {"B", std::string("a")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("b")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("bb")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("bbb")}});

    vor = vlt.GetRecords("A", 1, records);
    
    TEST_ASSERT(records.size() == 1);
    COMPARE_REF(records[0], {{{"A", 1}, {"B", std::string("a")}}});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));


    vor = vlt.GetRecords("A", 2, records);
    
    TEST_ASSERT(records.size() == 3);

    int resI = 0;
    std::string resS;

    for (const VaultRecordRef& ref : records)
    {
        int i = 0;
        std::string s;

        ref.GetData("A", i);
        ref.GetData("B", s);

        resI += i;
        resS += s;
    }

    TEST_ASSERT(resI == 6);
    TEST_ASSERT(resS == "bbbbbb");

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));
)

TEST_BODY(GetRecords, CorrectGetNotAllRecords,
    Vault vlt;
    std::vector<VaultRecordRef> records;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 1}, {"B", std::string("a")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("b")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("bb")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("bbb")}});

    vor = vlt.GetRecords("A", 1, records, 2);
    
    TEST_ASSERT(records.size() == 1);
    COMPARE_REF(records[0], {{{"A", 1}, {"B", std::string("a")}}});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
    RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));


    vor = vlt.GetRecords("A", 2, records, 2);

    TEST_ASSERT(records.size() == 2);

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));
)

TEST_BODY(GetRecords, WrongKey,
    Vault vlt;
    std::vector<VaultRecordRef> records;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 1}, {"B", std::string("a")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("b")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("bb")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("bbb")}});

    vor = vlt.GetRecords("Z", 1, records);
    
    TEST_ASSERT(records.size() == 0);

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "Z", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongKey, SavedType == typeid(void));
)

TEST_BODY(GetRecords, WrongType,
    Vault vlt;
    std::vector<VaultRecordRef> records;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 1}, {"B", std::string("a")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("b")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("bb")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("bbb")}});

    vor = vlt.GetRecords("B", 1, records);
    
    TEST_ASSERT(records.size() == 0);

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "B", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongType, SavedType == typeid(std::string));
)

TEST_BODY(GetRecords, WrongValue,
    Vault vlt;
    std::vector<VaultRecordRef> records;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 1}, {"B", std::string("a")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("b")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("bb")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("bbb")}});

    vor = vlt.GetRecords("A", 99, records);
    
    TEST_ASSERT(records.size() == 0);

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongValue, SavedType == typeid(int));
)

TEST_BODY(RequestEqual, CorrectRequest,
    Vault vlt;
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 1}, {"B", std::string("a")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("b")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("bb")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("bbb")}});
    vlt.CreateRecord({{"A", 3}, {"B", std::string("c")}});

    vor = vlt.RequestEqual("A", 1, vrs);

    TEST_ASSERT(vrs.Size() == 1);

    COMPARE_VAULT(vrs, {{{"A", 1}, {"B", std::string("a")}}});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));


    vor = vlt.RequestEqual("A", 2, vrs);

    TEST_ASSERT(vrs.Size() == 3);

    COMPARE_VAULT(vrs, {
        {{"A", 2}, {"B", std::string("b")}},
        {{"A", 2}, {"B", std::string("bb")}},
        {{"A", 2}, {"B", std::string("bbb")}},
    });

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));
)

TEST_BODY(RequestEqual, CorrectRequestNotAllRecords,
    Vault vlt;
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 1}, {"B", std::string("a")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("b")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("bb")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("bbb")}});
    vlt.CreateRecord({{"A", 3}, {"B", std::string("c")}});

    vor = vlt.RequestEqual("A", 1, vrs, 2);

    TEST_ASSERT(vrs.Size() == 1);

    COMPARE_VAULT(vrs, {{{"A", 1}, {"B", std::string("a")}}});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));


    vor = vlt.RequestEqual("A", 2, vrs, 2);

    TEST_ASSERT(vrs.Size() == 2);

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));
)

TEST_BODY(RequestEqual, CorrectRequestWithPredicat,
    Vault vlt;
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 1}, {"B", std::string("a")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("b")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("bb")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("bbb")}});
    vlt.CreateRecord({{"A", 3}, {"B", std::string("c")}});

    vor = vlt.RequestEqual("A", 2, vrs, -1, [](const VaultRecordRef& ref) -> bool
    {
        std::string s;
        ref.GetData("B", s);
        if (s.length() <= 2) return true;
        else return false;
    });

    TEST_ASSERT(vrs.Size() == 2);

    COMPARE_VAULT(vrs, {
        {{"A", 2}, {"B", std::string("b")}},
        {{"A", 2}, {"B", std::string("bb")}},
    });

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));
)

TEST_BODY(RequestEqual, WrongKey,
    Vault vlt;
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 1}, {"B", std::string("a")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("b")}});
    vlt.CreateRecord({{"A", 3}, {"B", std::string("c")}});

    vor = vlt.RequestEqual("Z", 1, vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "Z", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongKey, SavedType == typeid(void));
)

TEST_BODY(RequestEqual, WrongType,
    Vault vlt;
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 1}, {"B", std::string("a")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("b")}});
    vlt.CreateRecord({{"A", 3}, {"B", std::string("c")}});

    vor = vlt.RequestEqual("B", 1, vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "B", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongType, SavedType == typeid(std::string));
)

TEST_BODY(RequestEqual, ValueNotInVault,
    Vault vlt;
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 1}, {"B", std::string("a")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("b")}});
    vlt.CreateRecord({{"A", 3}, {"B", std::string("c")}});

    vor = vlt.RequestEqual("A", 4, vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));
)

TEST_BODY(RequestGreater, CorrectRequest,
    Vault vlt;
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 1}, {"B", std::string("a")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("b")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("bb")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("bbb")}});
    vlt.CreateRecord({{"A", 3}, {"B", std::string("c")}});

    vor = vlt.RequestGreater("A", 1, vrs);

    TEST_ASSERT(vrs.Size() == 4);

    COMPARE_VAULT(vrs, {
        {{"A", 2}, {"B", std::string("b")}},
        {{"A", 2}, {"B", std::string("bb")}},
        {{"A", 2}, {"B", std::string("bbb")}},
        {{"A", 3}, {"B", std::string("c")}},
    });

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));


    vor = vlt.RequestGreater("A", 2, vrs);

    TEST_ASSERT(vrs.Size() == 1);

    COMPARE_VAULT(vrs, {{{"A", 3}, {"B", std::string("c")}}});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));
)

TEST_BODY(RequestGreater, CorrectRequestNotAllRecords,
    Vault vlt;
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 1}, {"B", std::string("a")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("b")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("bb")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("bbb")}});
    vlt.CreateRecord({{"A", 3}, {"B", std::string("c")}});

    vor = vlt.RequestGreater("A", 1, vrs, 2);

    TEST_ASSERT(vrs.Size() == 2);

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));


    vor = vlt.RequestGreater("A", 2, vrs, 4);

    TEST_ASSERT(vrs.Size() == 1);

    COMPARE_VAULT(vrs, {{{"A", 3}, {"B", std::string("c")}}});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));
)

TEST_BODY(RequestGreater, CorrectRequestWithPredicat,
    Vault vlt;
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 1}, {"B", std::string("a")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("b")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("bb")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("bbb")}});
    vlt.CreateRecord({{"A", 3}, {"B", std::string("c")}});

    vor = vlt.RequestGreater("A", 0, vrs, -1, [](const VaultRecordRef& ref) -> bool
    {
        std::string s;
        ref.GetData("B", s);
        if (s.length() > 1) return true;
        else return false;
    });

    TEST_ASSERT(vrs.Size() == 2);

    COMPARE_VAULT(vrs, {
        {{"A", 2}, {"B", std::string("bb")}},
        {{"A", 2}, {"B", std::string("bbb")}},
    });

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));
)

TEST_BODY(RequestGreater, BoundaryValues,
    Vault vlt;
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);

    // ...1...3...5...
    vlt.CreateRecord({{"A", 1}});
    vlt.CreateRecord({{"A", 3}});
    vlt.CreateRecord({{"A", 5}});

    // x
    // 0...1...2...3...4...5...6
    vor = vlt.RequestGreater("A", 0, vrs);
    COMPARE_VAULT(vrs, {{{"A", 1}}, {{"A", 3}}, {{"A", 5}}});

    //     x
    // 0...1...2...3...4...5...6
    vor = vlt.RequestGreater("A", 1, vrs);
    COMPARE_VAULT(vrs, {{{"A", 3}}, {{"A", 5}}});

    //         x
    // 0...1...2...3...4...5...6
    vor = vlt.RequestGreater("A", 2, vrs);
    COMPARE_VAULT(vrs, {{{"A", 3}}, {{"A", 5}}});

    //             x
    // 0...1...2...3...4...5...6
    vor = vlt.RequestGreater("A", 3, vrs);
    COMPARE_VAULT(vrs, {{{"A", 5}}});

    //                 x
    // 0...1...2...3...4...5...6
    vor = vlt.RequestGreater("A", 4, vrs);
    COMPARE_VAULT(vrs, {{{"A", 5}}});

    //                     x
    // 0...1...2...3...4...5...6
    vor = vlt.RequestGreater("A", 5, vrs);
    COMPARE_VAULT(vrs, {});

    //                         x
    // 0...1...2...3...4...5...6
    vor = vlt.RequestGreater("A", 6, vrs);
    COMPARE_VAULT(vrs, {});
)

TEST_BODY(RequestGreater, WrongKey,
    Vault vlt;
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 1}, {"B", std::string("a")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("b")}});
    vlt.CreateRecord({{"A", 3}, {"B", std::string("c")}});

    vor = vlt.RequestGreater("Z", 1, vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "Z", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongKey, SavedType == typeid(void));
)

TEST_BODY(RequestGreater, WrongType,
    Vault vlt;
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 1}, {"B", std::string("a")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("b")}});
    vlt.CreateRecord({{"A", 3}, {"B", std::string("c")}});

    vor = vlt.RequestGreater("B", 1, vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "B", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongType, SavedType == typeid(std::string));
)

TEST_BODY(RequestGreater, ValueNotInVault,
    Vault vlt;
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 1}, {"B", std::string("a")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("b")}});
    vlt.CreateRecord({{"A", 3}, {"B", std::string("c")}});

    vor = vlt.RequestGreater("A", 4, vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));
)

TEST_BODY(RequestGreaterOrEqual, CorrectRequest,
    Vault vlt;
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 1}, {"B", std::string("a")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("b")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("bb")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("bbb")}});
    vlt.CreateRecord({{"A", 3}, {"B", std::string("c")}});

    vor = vlt.RequestGreaterOrEqual("A", 1, vrs);

    TEST_ASSERT(vrs.Size() == 5);

    COMPARE_VAULT(vrs, {
        {{"A", 1}, {"B", std::string("a")}},
        {{"A", 2}, {"B", std::string("b")}},
        {{"A", 2}, {"B", std::string("bb")}},
        {{"A", 2}, {"B", std::string("bbb")}},
        {{"A", 3}, {"B", std::string("c")}},
    });

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));


    vor = vlt.RequestGreaterOrEqual("A", 3, vrs);

    TEST_ASSERT(vrs.Size() == 1);

    COMPARE_VAULT(vrs, {{{"A", 3}, {"B", std::string("c")}}});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));
)

TEST_BODY(RequestGreaterOrEqual, CorrectRequestNotAllRecords,
    Vault vlt;
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 1}, {"B", std::string("a")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("b")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("bb")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("bbb")}});
    vlt.CreateRecord({{"A", 3}, {"B", std::string("c")}});

    vor = vlt.RequestGreaterOrEqual("A", 2, vrs, 2);

    TEST_ASSERT(vrs.Size() == 2);

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));


    vor = vlt.RequestGreaterOrEqual("A", 3, vrs, 4);

    TEST_ASSERT(vrs.Size() == 1);

    COMPARE_VAULT(vrs, {{{"A", 3}, {"B", std::string("c")}}});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));
)

TEST_BODY(RequestGreaterOrEqual, CorrectRequestWithPredicat,
    Vault vlt;
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 1}, {"B", std::string("a")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("b")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("bb")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("bbb")}});
    vlt.CreateRecord({{"A", 3}, {"B", std::string("c")}});

    vor = vlt.RequestGreaterOrEqual("A", 2, vrs, -1, [](const VaultRecordRef& ref) -> bool
    {
        std::string s;
        ref.GetData("B", s);
        if (s.length() > 1) return true;
        else return false;
    });

    TEST_ASSERT(vrs.Size() == 2);

    COMPARE_VAULT(vrs, {
        {{"A", 2}, {"B", std::string("bb")}},
        {{"A", 2}, {"B", std::string("bbb")}},
    });

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));
)

TEST_BODY(RequestGreaterOrEqual, BoundaryValues,
    Vault vlt;
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);

    // ...1...3...5...
    vlt.CreateRecord({{"A", 1}});
    vlt.CreateRecord({{"A", 3}});
    vlt.CreateRecord({{"A", 5}});

    // x
    // 0...1...2...3...4...5...6
    vor = vlt.RequestGreaterOrEqual("A", 0, vrs);
    COMPARE_VAULT(vrs, {{{"A", 1}}, {{"A", 3}}, {{"A", 5}}});

    //     x
    // 0...1...2...3...4...5...6
    vor = vlt.RequestGreaterOrEqual("A", 1, vrs);
    COMPARE_VAULT(vrs, {{{"A", 1}}, {{"A", 3}}, {{"A", 5}}});

    //         x
    // 0...1...2...3...4...5...6
    vor = vlt.RequestGreaterOrEqual("A", 2, vrs);
    COMPARE_VAULT(vrs, {{{"A", 3}}, {{"A", 5}}});

    //             x
    // 0...1...2...3...4...5...6
    vor = vlt.RequestGreaterOrEqual("A", 3, vrs);
    COMPARE_VAULT(vrs, {{{"A", 3}}, {{"A", 5}}});

    //                 x
    // 0...1...2...3...4...5...6
    vor = vlt.RequestGreaterOrEqual("A", 4, vrs);
    COMPARE_VAULT(vrs, {{{"A", 5}}});

    //                     x
    // 0...1...2...3...4...5...6
    vor = vlt.RequestGreaterOrEqual("A", 5, vrs);
    COMPARE_VAULT(vrs, {{{"A", 5}}});

    //                         x
    // 0...1...2...3...4...5...6
    vor = vlt.RequestGreaterOrEqual("A", 6, vrs);
    COMPARE_VAULT(vrs, {});
)

TEST_BODY(RequestGreaterOrEqual, WrongKey,
    Vault vlt;
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 1}, {"B", std::string("a")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("b")}});
    vlt.CreateRecord({{"A", 3}, {"B", std::string("c")}});

    vor = vlt.RequestGreaterOrEqual("Z", 1, vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "Z", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongKey, SavedType == typeid(void));
)

TEST_BODY(RequestGreaterOrEqual, WrongType,
    Vault vlt;
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 1}, {"B", std::string("a")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("b")}});
    vlt.CreateRecord({{"A", 3}, {"B", std::string("c")}});

    vor = vlt.RequestGreaterOrEqual("B", 1, vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "B", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongType, SavedType == typeid(std::string));
)

TEST_BODY(RequestGreaterOrEqual, ValueNotInVault,
    Vault vlt;
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 1}, {"B", std::string("a")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("b")}});
    vlt.CreateRecord({{"A", 3}, {"B", std::string("c")}});

    vor = vlt.RequestGreaterOrEqual("A", 4, vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));
)

TEST_BODY(RequestLess, CorrectRequest,
    Vault vlt;
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 1}, {"B", std::string("a")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("b")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("bb")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("bbb")}});
    vlt.CreateRecord({{"A", 3}, {"B", std::string("c")}});

    vor = vlt.RequestLess("A", 3, vrs);

    TEST_ASSERT(vrs.Size() == 4);

    COMPARE_VAULT(vrs, {
        {{"A", 1}, {"B", std::string("a")}},
        {{"A", 2}, {"B", std::string("b")}},
        {{"A", 2}, {"B", std::string("bb")}},
        {{"A", 2}, {"B", std::string("bbb")}},
    });

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));

    vor = vlt.RequestLess("A", 2, vrs);

    TEST_ASSERT(vrs.Size() == 1);

    COMPARE_VAULT(vrs, {{{"A", 1}, {"B", std::string("a")}}});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));
)

TEST_BODY(RequestLess, CorrectRequestNotAllRecords,
    Vault vlt;
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 1}, {"B", std::string("a")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("b")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("bb")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("bbb")}});
    vlt.CreateRecord({{"A", 3}, {"B", std::string("c")}});

    vor = vlt.RequestLess("A", 3, vrs, 2);

    TEST_ASSERT(vrs.Size() == 2);

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));


    vor = vlt.RequestLess("A", 2, vrs, 4);

    TEST_ASSERT(vrs.Size() == 1);

    COMPARE_VAULT(vrs, {{{"A", 1}, {"B", std::string("a")}}});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));
)

TEST_BODY(RequestLess, CorrectRequestWithPredicat,
    Vault vlt;
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 1}, {"B", std::string("a")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("b")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("bb")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("bbb")}});
    vlt.CreateRecord({{"A", 3}, {"B", std::string("c")}});

    vor = vlt.RequestLess("A", 3, vrs, -1, [](const VaultRecordRef& ref) -> bool
    {
        std::string s;
        ref.GetData("B", s);
        if (s.length() > 1) return true;
        else return false;
    });

    TEST_ASSERT(vrs.Size() == 2);

    COMPARE_VAULT(vrs, {
        {{"A", 2}, {"B", std::string("bb")}},
        {{"A", 2}, {"B", std::string("bbb")}},
    });

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));
)

TEST_BODY(RequestLess, BoundaryValues,
    Vault vlt;
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);

    // ...1...3...5...
    vlt.CreateRecord({{"A", 1}});
    vlt.CreateRecord({{"A", 3}});
    vlt.CreateRecord({{"A", 5}});

    // x
    // 0...1...2...3...4...5...6
    vor = vlt.RequestLess("A", 0, vrs);
    COMPARE_VAULT(vrs, {});

    //     x
    // 0...1...2...3...4...5...6
    vor = vlt.RequestLess("A", 1, vrs);
    COMPARE_VAULT(vrs, {});

    //         x
    // 0...1...2...3...4...5...6
    vor = vlt.RequestLess("A", 2, vrs);
    COMPARE_VAULT(vrs, {{{"A", 1}}});

    //             x
    // 0...1...2...3...4...5...6
    vor = vlt.RequestLess("A", 3, vrs);
    COMPARE_VAULT(vrs, {{{"A", 1}}});

    //                 x
    // 0...1...2...3...4...5...6
    vor = vlt.RequestLess("A", 4, vrs);
    COMPARE_VAULT(vrs, {{{"A", 1}}, {{"A", 3}}});

    //                     x
    // 0...1...2...3...4...5...6
    vor = vlt.RequestLess("A", 5, vrs);
    COMPARE_VAULT(vrs, {{{"A", 1}}, {{"A", 3}}});

    //                         x
    // 0...1...2...3...4...5...6
    vor = vlt.RequestLess("A", 6, vrs);
    COMPARE_VAULT(vrs, {{{"A", 1}}, {{"A", 3}}, {{"A", 5}}});
)

TEST_BODY(RequestLess, WrongKey,
    Vault vlt;
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 1}, {"B", std::string("a")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("b")}});
    vlt.CreateRecord({{"A", 3}, {"B", std::string("c")}});

    vor = vlt.RequestLess("Z", 1, vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "Z", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongKey, SavedType == typeid(void));
)

TEST_BODY(RequestLess, WrongType,
    Vault vlt;
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 1}, {"B", std::string("a")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("b")}});
    vlt.CreateRecord({{"A", 3}, {"B", std::string("c")}});

    vor = vlt.RequestLess("B", 1, vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "B", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongType, SavedType == typeid(std::string));
)

TEST_BODY(RequestLess, ValueNotInVault,
    Vault vlt;
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 1}, {"B", std::string("a")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("b")}});
    vlt.CreateRecord({{"A", 3}, {"B", std::string("c")}});

    vor = vlt.RequestLess("A", 1, vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));
)

TEST_BODY(RequestLessOrEqual, CorrectRequest,
    Vault vlt;
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 1}, {"B", std::string("a")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("b")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("bb")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("bbb")}});
    vlt.CreateRecord({{"A", 3}, {"B", std::string("c")}});

    vor = vlt.RequestLessOrEqual("A", 2, vrs);

    TEST_ASSERT(vrs.Size() == 4);

    COMPARE_VAULT(vrs, {
        {{"A", 1}, {"B", std::string("a")}},
        {{"A", 2}, {"B", std::string("b")}},
        {{"A", 2}, {"B", std::string("bb")}},
        {{"A", 2}, {"B", std::string("bbb")}},
    });

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));

    vor = vlt.RequestLessOrEqual("A", 1, vrs);

    TEST_ASSERT(vrs.Size() == 1);

    COMPARE_VAULT(vrs, {{{"A", 1}, {"B", std::string("a")}}});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));
)

TEST_BODY(RequestLessOrEqual, CorrectRequestNotAllRecords,
    Vault vlt;
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 1}, {"B", std::string("a")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("b")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("bb")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("bbb")}});
    vlt.CreateRecord({{"A", 3}, {"B", std::string("c")}});

    vor = vlt.RequestLessOrEqual("A", 3, vrs, 2);

    TEST_ASSERT(vrs.Size() == 2);

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));


    vor = vlt.RequestLessOrEqual("A", 1, vrs, 4);

    TEST_ASSERT(vrs.Size() == 1);

    COMPARE_VAULT(vrs, {{{"A", 1}, {"B", std::string("a")}}});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));
)

TEST_BODY(RequestLessOrEqual, CorrectRequestWithPredicat,
    Vault vlt;
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 1}, {"B", std::string("a")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("b")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("bb")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("bbb")}});
    vlt.CreateRecord({{"A", 3}, {"B", std::string("c")}});

    vor = vlt.RequestLessOrEqual("A", 2, vrs, -1, [](const VaultRecordRef& ref) -> bool
    {
        std::string s;
        ref.GetData("B", s);
        if (s.length() > 1) return true;
        else return false;
    });

    TEST_ASSERT(vrs.Size() == 2);

    COMPARE_VAULT(vrs, {
        {{"A", 2}, {"B", std::string("bb")}},
        {{"A", 2}, {"B", std::string("bbb")}},
    });

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));
)

TEST_BODY(RequestLessOrEqual, BoundaryValues,
    Vault vlt;
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);

    // ...1...3...5...
    vlt.CreateRecord({{"A", 1}});
    vlt.CreateRecord({{"A", 3}});
    vlt.CreateRecord({{"A", 5}});

    // x
    // 0...1...2...3...4...5...6
    vor = vlt.RequestLessOrEqual("A", 0, vrs);
    COMPARE_VAULT(vrs, {});

    //     x
    // 0...1...2...3...4...5...6
    vor = vlt.RequestLessOrEqual("A", 1, vrs);
    COMPARE_VAULT(vrs, {{{"A", 1}}});

    //         x
    // 0...1...2...3...4...5...6
    vor = vlt.RequestLessOrEqual("A", 2, vrs);
    COMPARE_VAULT(vrs, {{{"A", 1}}});

    //             x
    // 0...1...2...3...4...5...6
    vor = vlt.RequestLessOrEqual("A", 3, vrs);
    COMPARE_VAULT(vrs, {{{"A", 1}}, {{"A", 3}}});

    //                 x
    // 0...1...2...3...4...5...6
    vor = vlt.RequestLessOrEqual("A", 4, vrs);
    COMPARE_VAULT(vrs, {{{"A", 1}}, {{"A", 3}}});

    //                     x
    // 0...1...2...3...4...5...6
    vor = vlt.RequestLessOrEqual("A", 5, vrs);
    COMPARE_VAULT(vrs, {{{"A", 1}}, {{"A", 3}}, {{"A", 5}}});

    //                         x
    // 0...1...2...3...4...5...6
    vor = vlt.RequestLessOrEqual("A", 6, vrs);
    COMPARE_VAULT(vrs, {{{"A", 1}}, {{"A", 3}}, {{"A", 5}}});
)

TEST_BODY(RequestLessOrEqual, WrongKey,
    Vault vlt;
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 1}, {"B", std::string("a")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("b")}});
    vlt.CreateRecord({{"A", 3}, {"B", std::string("c")}});

    vor = vlt.RequestLessOrEqual("Z", 1, vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "Z", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongKey, SavedType == typeid(void));
)

TEST_BODY(RequestLessOrEqual, WrongType,
    Vault vlt;
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 1}, {"B", std::string("a")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("b")}});
    vlt.CreateRecord({{"A", 3}, {"B", std::string("c")}});

    vor = vlt.RequestLessOrEqual("B", 1, vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "B", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongType, SavedType == typeid(std::string));
)

TEST_BODY(RequestLessOrEqual, ValueNotInVault,
    Vault vlt;
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 1}, {"B", std::string("a")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("b")}});
    vlt.CreateRecord({{"A", 3}, {"B", std::string("c")}});

    vor = vlt.RequestLessOrEqual("A", 0, vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));
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
    CreateKey::DuplicateParams();
    CreateKey::WrongKeyWithoutUniqueKey();
    CreateKey::WrongTypeWithoutUniqueKey();
    CreateKey::WrongKeyWithUniqueKey();
    CreateKey::WrongTypeWithUniqueKey();
    CreateKey::DuplicateUniqueKeyValue();

    GetRecord::CorrectGetRecord();
    GetRecord::WrongKey();
    GetRecord::WrongType();
    GetRecord::WrongValue();

    GetRecords::CorrectGetRecords();
    GetRecords::CorrectGetNotAllRecords();
    GetRecords::WrongKey();
    GetRecords::WrongType();
    GetRecords::WrongValue();

    RequestEqual::CorrectRequest();
    RequestEqual::CorrectRequestNotAllRecords();
    RequestEqual::CorrectRequestWithPredicat();
    RequestEqual::WrongKey();
    RequestEqual::WrongType();
    RequestEqual::ValueNotInVault();

    RequestGreater::CorrectRequest();
    RequestGreater::CorrectRequestNotAllRecords();
    RequestGreater::CorrectRequestWithPredicat();
    RequestGreater::BoundaryValues();
    RequestGreater::WrongKey();
    RequestGreater::WrongType();
    RequestGreater::ValueNotInVault();

    RequestGreaterOrEqual::CorrectRequest();
    RequestGreaterOrEqual::CorrectRequestNotAllRecords();
    RequestGreaterOrEqual::CorrectRequestWithPredicat();
    RequestGreaterOrEqual::BoundaryValues();
    RequestGreaterOrEqual::WrongKey();
    RequestGreaterOrEqual::WrongType();
    RequestGreaterOrEqual::ValueNotInVault();

    RequestLess::CorrectRequest();
    RequestLess::CorrectRequestNotAllRecords();
    RequestLess::CorrectRequestWithPredicat();
    RequestLess::BoundaryValues();
    RequestLess::WrongKey();
    RequestLess::WrongType();
    RequestLess::ValueNotInVault();

    RequestLessOrEqual::CorrectRequest();
    RequestLessOrEqual::CorrectRequestNotAllRecords();
    RequestLessOrEqual::CorrectRequestWithPredicat();
    RequestLessOrEqual::BoundaryValues();
    RequestLessOrEqual::WrongKey();
    RequestLessOrEqual::WrongType();
    RequestLessOrEqual::ValueNotInVault();
}