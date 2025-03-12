#include "VaultUnitTests.h"

class VaultSetDataToRecordTest : public Vault
{
public:
    template <class T>
    VaultOperationResult SetDataToRecord(VaultRecord* dataRecord, const std::string& key, const T& data) noexcept
    {
        return Vault::SetDataToRecord(dataRecord, key, data);
    }
};

TEST_BODY(SetDataToRecord, IncorrectDataRecord,
    VaultSetDataToRecordTest vlt;
    VaultOperationResult vor;

    VaultRecord* recordPtr = nullptr;
    vor = vlt.SetDataToRecord(recordPtr, "A", 0);

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::DataRecordNotValid, SavedType == typeid(void));
)

TEST_BODY(SetDataToRecord, NotExistedKey,
    VaultSetDataToRecordTest vlt;
    VaultOperationResult vor;
    VaultRecord record;

    vor = vlt.SetDataToRecord(&record, "A", 0);

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongKey, SavedType == typeid(void));
)

TEST_BODY(SetDataToRecord, IncorrectType,
    VaultSetDataToRecordTest vlt;
    VaultOperationResult vor;
    VaultRecord record;

    vlt.AddKey("A", 0);

    vor = vlt.SetDataToRecord(&record, "A", std::string(""));

    vor.Print();
    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(std::string), ResultCode == VaultOperationResultCode::WrongType, SavedType == typeid(int));
)

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
    TEST_ASSERT(vlt.GetKeys() == std::vector<std::string>({"A"}));

    res = vlt.AddKey<std::string>("B", "");

    TEST_ASSERT(res);
    TEST_ASSERT(vlt.GetKeys() == std::vector<std::string>({"A", "B"}));
)

TEST_BODY(AddKey, IncorrectAddKey,
    Vault vlt;
    bool res = false;

    vlt.AddKey("A", 0);
    res = vlt.AddKey<std::string>("A", "");

    TEST_ASSERT(res == false);
    TEST_ASSERT(vlt.GetKeys() == std::vector<std::string>({"A"}));
)

TEST_BODY(AddKey, CorrectAddToNonEmpty,
    Vault vlt;

    vlt.AddKey("A", 0);

    vlt.CreateRecord({{"A", 1}});
    vlt.CreateRecord({{"A", 2}});
    vlt.CreateRecord({{"A", 3}});

    vlt.AddKey<std::string>("B", "added");

    TEST_ASSERT(vlt.GetKeys() == std::vector<std::string>({"A", "B"}));

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
    TEST_ASSERT(vlt.GetKeys() == std::vector<std::string>({"A"}));
)

TEST_BODY(AddKey, AddKeyWithDependentSets,
    Vault vlt;
    VaultRecordSet vrs;
    bool res = false;

    vlt.AddKey("A", 0);
    vlt.AddKey("B", 0);

    vlt.CreateRecord({{"A", 1}});
    vlt.CreateRecord({{"A", 2}});
    vlt.CreateRecord({{"A", 3}});

    vlt.RequestEqual("B", 0, vrs);

    res = vlt.AddKey<std::string>("C", "added");

    TEST_ASSERT(res == true);

    TEST_ASSERT(vrs.GetKeys() == std::vector<std::string>({"A", "B", "C"}));

    COMPARE_VAULT(vrs, {
        {{"A", 1}, {"B", 0}, {"C", std::string("added")}},
        {{"A", 2}, {"B", 0}, {"C", std::string("added")}},
        {{"A", 3}, {"B", 0}, {"C", std::string("added")}},
    });
)

TEST_BODY(AddKey, IncorrectAddKeyWithDependentSets,
    Vault vlt;
    VaultRecordSet vrs;
    bool res = false;

    vlt.AddKey("A", 0);
    vlt.AddKey("B", 0);

    vlt.CreateRecord({{"A", 1}});
    vlt.CreateRecord({{"A", 2}});
    vlt.CreateRecord({{"A", 3}});

    vlt.RequestEqual("B", 0, vrs);

    res = vlt.AddKey<std::string>("A", "added");

    TEST_ASSERT(res == false);

    TEST_ASSERT(vlt.GetKeys() == std::vector<std::string>({"A", "B"}));

    COMPARE_VAULT(vrs, {
        {{"A", 1}, {"B", 0}},
        {{"A", 2}, {"B", 0}},
        {{"A", 3}, {"B", 0}},
    });
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

TEST_BODY(AddUniqueKey, AddUniqueKeyWithDependentSets,
    Vault vlt;
    VaultRecordSet vrs;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddKey("B", 0);

    vlt.CreateRecord({{"A", 1}});
    vlt.CreateRecord({{"A", 2}});
    vlt.CreateRecord({{"A", 3}});

    vlt.RequestEqual("B", 0, vrs);

    vor = vlt.AddUniqueKey<std::string>("C", [](const std::size_t& counter, const VaultRecordRef& ref) -> std::string
    {
        int a = 0;
        ref.GetData("A", a);
        return std::to_string(a);
    });

    TEST_ASSERT(vlt.GetKeys() == std::vector<std::string>({"A", "B", "C"}));

    COMPARE_VAULT(vrs, {
        {{"A", 1}, {"B", 0}, {"C", std::string("1")}},
        {{"A", 2}, {"B", 0}, {"C", std::string("2")}},
        {{"A", 3}, {"B", 0}, {"C", std::string("3")}},
    });

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "C", 
        RequestedType == typeid(std::string), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(void));
)

TEST_BODY(AddUniqueKey, IncorrectAddKeyWithDependentSets,
    Vault vlt;
    VaultRecordSet vrs;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddKey("B", 0);

    vlt.CreateRecord({{"A", 1}});
    vlt.CreateRecord({{"A", 2}});
    vlt.CreateRecord({{"A", 3}});

    vlt.RequestEqual("B", 0, vrs);

    vor = vlt.AddUniqueKey<std::string>("A", [](const std::size_t& counter, const VaultRecordRef& ref) -> std::string
    {
        int a = 0;
        ref.GetData("A", a);
        return std::to_string(a);
    });

    TEST_ASSERT(vlt.GetKeys() == std::vector<std::string>({"A", "B"}));

    COMPARE_VAULT(vrs, {
        {{"A", 1}, {"B", 0}},
        {{"A", 2}, {"B", 0}},
        {{"A", 3}, {"B", 0}},
    });

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(std::string), ResultCode == VaultOperationResultCode::DuplicateKey, SavedType == typeid(int));
)

TEST_BODY(AddUniqueKey, CorrectAddKeyWithDependentSetsWithDuplicate,
    Vault vlt;
    VaultRecordSet vrs;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddKey("B", 0);

    vlt.CreateRecord({{"A", 1}});
    vlt.CreateRecord({{"A", 2}});
    vlt.CreateRecord({{"A", 3}});

    vlt.RequestEqual("B", 0, vrs);

    vor = vlt.AddUniqueKey<std::string>("C", [](const std::size_t& counter, const VaultRecordRef& ref) -> std::string
    {
        if (counter > 0) return std::to_string(1);
        else return std::to_string(counter);
    });

    TEST_ASSERT(vlt.GetKeys() == std::vector<std::string>({"A", "B"}));

    COMPARE_VAULT(vrs, {
        {{"A", 1}, {"B", 0}},
        {{"A", 2}, {"B", 0}},
        {{"A", 3}, {"B", 0}},
    });

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "C", 
        RequestedType == typeid(std::string), ResultCode == VaultOperationResultCode::UniqueKeyValueAlredyInSet, SavedType == typeid(void));
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

TEST_BODY(RemoveKey, CorrectRemoveWithDependentSets,
    Vault vlt;
    VaultRecordSet vrs;

    vlt.AddKey("A", 0);
    vlt.AddKey("B", 0);
    vlt.AddUniqueKey<std::string>("C");

    vlt.CreateRecord({{"A", 0}, {"B", 3}, {"C", std::string("a")}});
    vlt.CreateRecord({{"A", 0}, {"B", 2}, {"C", std::string("b")}});
    vlt.CreateRecord({{"A", 0}, {"B", 1}, {"C", std::string("c")}});

    vlt.RequestEqual("A", 0, vrs);

    TEST_ASSERT(vlt.RemoveKey("B"));
    TEST_ASSERT(vlt.RemoveKey("C"));

    TEST_ASSERT(vrs.GetKeys().size() == 1);
    COMPARE_VAULT(vrs, {
        {{"A", 0}},
        {{"A", 0}},
        {{"A", 0}},
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

TEST_BODY(CreateRecord, CorrectParamsWithoutUniqueKey,
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

TEST_BODY(CreateRecord, CorrectParamsWithUniqueKey,
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

TEST_BODY(CreateRecord, DuplicateParams,
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

TEST_BODY(CreateRecord, WrongKeyWithoutUniqueKey,
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

TEST_BODY(CreateRecord, WrongTypeWithoutUniqueKey,
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

TEST_BODY(CreateRecord, WrongKeyWithUniqueKey,
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

TEST_BODY(CreateRecord, WrongTypeWithUniqueKey,
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

TEST_BODY(CreateRecord, DuplicateUniqueKeyValue,
    Vault vlt;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");
    vlt.AddUniqueKey<int>("C");

    vlt.CreateRecord({{"A", 1}, {"B", std::string("none")}, {"C", 0}});


    vor = vlt.CreateRecord({{"B", std::string("none")}});

    COMPARE_VAULT(vlt, {{{"A", 1}, {"B", std::string("none")}, {"C", 0}}});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "B", 
        RequestedType == typeid(std::string), ResultCode == VaultOperationResultCode::UniqueKeyValueAlredyInSet, SavedType == typeid(void));


    vor = vlt.CreateRecord({{"A", 2}, {"B", std::string("none")}});

    COMPARE_VAULT(vlt, {{{"A", 1}, {"B", std::string("none")}, {"C", 0}}});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "B", 
        RequestedType == typeid(std::string), ResultCode == VaultOperationResultCode::UniqueKeyValueAlredyInSet, SavedType == typeid(void));


    vor = vlt.CreateRecord({{"A", 2}, {"B", std::string("none2")}, {"C", 0}});

    COMPARE_VAULT(vlt, {{{"A", 1}, {"B", std::string("none")}, {"C", 0}}});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "C", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::UniqueKeyValueAlredyInSet, SavedType == typeid(void));
)

TEST_BODY(CreateRecord, EmptyUniqueKeyValue,
    Vault vlt;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vor = vlt.CreateRecord({{"A", 1}});

    COMPARE_VAULT(vlt, {{{"A", 1}, {"B", std::string("")}}});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(void));
    

    vor = vlt.CreateRecord({{"A", 1}});

    COMPARE_VAULT(vlt, {{{"A", 1}, {"B", std::string("")}}});

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

    vlt.AddKey("A", 0);

    // ...1...3...5...
    vlt.CreateRecord({{"A", 1}});
    vlt.CreateRecord({{"A", 3}});
    vlt.CreateRecord({{"A", 5}});

    //  x
    // ...1...3...5...
    vlt.RequestGreater("A", 0, vrs);
    COMPARE_VAULT(vrs, {{{"A", 1}}, {{"A", 3}}, {{"A", 5}}});

    //    x
    // ...1...3...5...
    vlt.RequestGreater("A", 1, vrs);
    COMPARE_VAULT(vrs, {{{"A", 3}}, {{"A", 5}}});

    //      x
    // ...1...3...5...
    vlt.RequestGreater("A", 2, vrs);
    COMPARE_VAULT(vrs, {{{"A", 3}}, {{"A", 5}}});

    //        x
    // ...1...3...5...
    vlt.RequestGreater("A", 3, vrs);
    COMPARE_VAULT(vrs, {{{"A", 5}}});

    //          x
    // ...1...3...5...
    vlt.RequestGreater("A", 4, vrs);
    COMPARE_VAULT(vrs, {{{"A", 5}}});

    //            x
    // ...1...3...5...
    vlt.RequestGreater("A", 5, vrs);
    COMPARE_VAULT(vrs, {});

    //              x
    // ...1...3...5...
    vlt.RequestGreater("A", 6, vrs);
    COMPARE_VAULT(vrs, {});
)

TEST_BODY(RequestGreater, WrongKey,
    Vault vlt;
    VaultRecordSet vrs;
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

    vlt.AddKey("A", 0);

    // ...1...3...5...
    vlt.CreateRecord({{"A", 1}});
    vlt.CreateRecord({{"A", 3}});
    vlt.CreateRecord({{"A", 5}});

    //  x
    // ...1...3...5...
    vlt.RequestGreaterOrEqual("A", 0, vrs);
    COMPARE_VAULT(vrs, {{{"A", 1}}, {{"A", 3}}, {{"A", 5}}});

    //    x
    // ...1...3...5...
    vlt.RequestGreaterOrEqual("A", 1, vrs);
    COMPARE_VAULT(vrs, {{{"A", 1}}, {{"A", 3}}, {{"A", 5}}});

    //      x
    // ...1...3...5...
    vlt.RequestGreaterOrEqual("A", 2, vrs);
    COMPARE_VAULT(vrs, {{{"A", 3}}, {{"A", 5}}});

    //        x
    // ...1...3...5...
    vlt.RequestGreaterOrEqual("A", 3, vrs);
    COMPARE_VAULT(vrs, {{{"A", 3}}, {{"A", 5}}});

    //          x
    // ...1...3...5...
    vlt.RequestGreaterOrEqual("A", 4, vrs);
    COMPARE_VAULT(vrs, {{{"A", 5}}});

    //            x
    // ...1...3...5...
    vlt.RequestGreaterOrEqual("A", 5, vrs);
    COMPARE_VAULT(vrs, {{{"A", 5}}});

    //              x
    // ...1...3...5...
    vlt.RequestGreaterOrEqual("A", 6, vrs);
    COMPARE_VAULT(vrs, {});
)

TEST_BODY(RequestGreaterOrEqual, WrongKey,
    Vault vlt;
    VaultRecordSet vrs;
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

    vlt.AddKey("A", 0);

    // ...1...3...5...
    vlt.CreateRecord({{"A", 1}});
    vlt.CreateRecord({{"A", 3}});
    vlt.CreateRecord({{"A", 5}});

    //  x
    // ...1...3...5...
    vlt.RequestLess("A", 0, vrs);
    COMPARE_VAULT(vrs, {});

    //    x
    // ...1...3...5...
    vlt.RequestLess("A", 1, vrs);
    COMPARE_VAULT(vrs, {});

    //      x
    // ...1...3...5...
    vlt.RequestLess("A", 2, vrs);
    COMPARE_VAULT(vrs, {{{"A", 1}}});

    //        x
    // ...1...3...5...
    vlt.RequestLess("A", 3, vrs);
    COMPARE_VAULT(vrs, {{{"A", 1}}});

    //          x
    // ...1...3...5...
    vlt.RequestLess("A", 4, vrs);
    COMPARE_VAULT(vrs, {{{"A", 1}}, {{"A", 3}}});

    //            x
    // ...1...3...5...
    vlt.RequestLess("A", 5, vrs);
    COMPARE_VAULT(vrs, {{{"A", 1}}, {{"A", 3}}});

    //              x
    // ...1...3...5...
    vlt.RequestLess("A", 6, vrs);
    COMPARE_VAULT(vrs, {{{"A", 1}}, {{"A", 3}}, {{"A", 5}}});
)

TEST_BODY(RequestLess, WrongKey,
    Vault vlt;
    VaultRecordSet vrs;
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

    vlt.AddKey("A", 0);

    // ...1...3...5...
    vlt.CreateRecord({{"A", 1}});
    vlt.CreateRecord({{"A", 3}});
    vlt.CreateRecord({{"A", 5}});

    //  x
    // ...1...3...5...
    vlt.RequestLessOrEqual("A", 0, vrs);
    COMPARE_VAULT(vrs, {});

    //    x
    // ...1...3...5...
    vlt.RequestLessOrEqual("A", 1, vrs);
    COMPARE_VAULT(vrs, {{{"A", 1}}});

    //      x
    // ...1...3...5...
    vlt.RequestLessOrEqual("A", 2, vrs);
    COMPARE_VAULT(vrs, {{{"A", 1}}});

    //        x
    // ...1...3...5...
    vlt.RequestLessOrEqual("A", 3, vrs);
    COMPARE_VAULT(vrs, {{{"A", 1}}, {{"A", 3}}});

    //          x
    // ...1...3...5...
    vlt.RequestLessOrEqual("A", 4, vrs);
    COMPARE_VAULT(vrs, {{{"A", 1}}, {{"A", 3}}});

    //            x
    // ...1...3...5...
    vlt.RequestLessOrEqual("A", 5, vrs);
    COMPARE_VAULT(vrs, {{{"A", 1}}, {{"A", 3}}, {{"A", 5}}});

    //              x
    // ...1...3...5...
    vlt.RequestLessOrEqual("A", 6, vrs);
    COMPARE_VAULT(vrs, {{{"A", 1}}, {{"A", 3}}, {{"A", 5}}});
)

TEST_BODY(RequestLessOrEqual, WrongKey,
    Vault vlt;
    VaultRecordSet vrs;
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

TEST_BODY(RequestInterval, CorrectRequest,
    Vault vlt;
    VaultRecordSet vrs;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 1}, {"B", std::string("a")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("b")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("bb")}});
    vlt.CreateRecord({{"A", 3}, {"B", std::string("c")}});
    vlt.CreateRecord({{"A", 4}, {"B", std::string("d")}});
    vlt.CreateRecord({{"A", 4}, {"B", std::string("dd")}});
    vlt.CreateRecord({{"A", 5}, {"B", std::string("ee")}});

    vor = vlt.RequestInterval("A", 2, 4, vrs);

    TEST_ASSERT(vrs.Size() == 5);

    COMPARE_VAULT(vrs, {
        {{"A", 2}, {"B", std::string("b")}},
        {{"A", 2}, {"B", std::string("bb")}},
        {{"A", 3}, {"B", std::string("c")}},
        {{"A", 4}, {"B", std::string("d")}},
        {{"A", 4}, {"B", std::string("dd")}},
    });

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));
)

TEST_BODY(RequestInterval, CorrectRequestWithoutLeftBoundary,
    Vault vlt;
    VaultRecordSet vrs;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 1}, {"B", std::string("a")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("b")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("bb")}});
    vlt.CreateRecord({{"A", 3}, {"B", std::string("c")}});
    vlt.CreateRecord({{"A", 4}, {"B", std::string("d")}});
    vlt.CreateRecord({{"A", 4}, {"B", std::string("dd")}});
    vlt.CreateRecord({{"A", 5}, {"B", std::string("ee")}});

    vor = vlt.RequestInterval("A", 2, 4, vrs, false);

    TEST_ASSERT(vrs.Size() == 3);

    COMPARE_VAULT(vrs, {
        {{"A", 3}, {"B", std::string("c")}},
        {{"A", 4}, {"B", std::string("d")}},
        {{"A", 4}, {"B", std::string("dd")}},
    });

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));
)

TEST_BODY(RequestInterval, CorrectRequestWithoutRightBoundary,
    Vault vlt;
    VaultRecordSet vrs;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 1}, {"B", std::string("a")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("b")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("bb")}});
    vlt.CreateRecord({{"A", 3}, {"B", std::string("c")}});
    vlt.CreateRecord({{"A", 4}, {"B", std::string("d")}});
    vlt.CreateRecord({{"A", 4}, {"B", std::string("dd")}});
    vlt.CreateRecord({{"A", 5}, {"B", std::string("ee")}});

    vor = vlt.RequestInterval("A", 2, 4, vrs, true, false);

    TEST_ASSERT(vrs.Size() == 3);

    COMPARE_VAULT(vrs, {
        {{"A", 2}, {"B", std::string("b")}},
        {{"A", 2}, {"B", std::string("bb")}},
        {{"A", 3}, {"B", std::string("c")}},
    });

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));
)

TEST_BODY(RequestInterval, CorrectRequestWithoutBoundaries,
    Vault vlt;
    VaultRecordSet vrs;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 1}, {"B", std::string("a")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("b")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("bb")}});
    vlt.CreateRecord({{"A", 3}, {"B", std::string("c")}});
    vlt.CreateRecord({{"A", 4}, {"B", std::string("d")}});
    vlt.CreateRecord({{"A", 4}, {"B", std::string("dd")}});
    vlt.CreateRecord({{"A", 5}, {"B", std::string("ee")}});

    vor = vlt.RequestInterval("A", 2, 4, vrs, false, false);

    TEST_ASSERT(vrs.Size() == 1);

    COMPARE_VAULT(vrs, {
        {{"A", 3}, {"B", std::string("c")}},
    });

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));
)

TEST_BODY(RequestInterval, CorrectRequestNotAllRecords,
    Vault vlt;
    VaultRecordSet vrs;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 1}, {"B", std::string("a")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("b")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("bb")}});
    vlt.CreateRecord({{"A", 3}, {"B", std::string("c")}});
    vlt.CreateRecord({{"A", 4}, {"B", std::string("d")}});
    vlt.CreateRecord({{"A", 4}, {"B", std::string("dd")}});
    vlt.CreateRecord({{"A", 5}, {"B", std::string("ee")}});

    vor = vlt.RequestInterval("A", 2, 4, vrs, true, true, 3);

    TEST_ASSERT(vrs.Size() == 3);

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));


    vor = vlt.RequestInterval("A", 2, 4, vrs, true, true, 8);

    TEST_ASSERT(vrs.Size() == 5);

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));
)

TEST_BODY(RequestInterval, CorrectRequestWithPredicat,
    Vault vlt;
    VaultRecordSet vrs;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 1}, {"B", std::string("a")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("b")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("bb")}});
    vlt.CreateRecord({{"A", 3}, {"B", std::string("c")}});
    vlt.CreateRecord({{"A", 4}, {"B", std::string("d")}});
    vlt.CreateRecord({{"A", 4}, {"B", std::string("dd")}});
    vlt.CreateRecord({{"A", 5}, {"B", std::string("ee")}});

    vor = vlt.RequestInterval("A", 2, 4, vrs, true, true, -1, [](const VaultRecordRef& ref) -> bool
    {
        std::string s;
        ref.GetData("B", s);
        if (s.length() > 1) return true;
        else return false;
    });

    TEST_ASSERT(vrs.Size() == 2);

    COMPARE_VAULT(vrs, {
        {{"A", 2}, {"B", std::string("bb")}},
        {{"A", 4}, {"B", std::string("dd")}},
    });

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));
)

TEST_BODY(RequestInterval, BoundaryValues,
    Vault vlt;
    VaultRecordSet vrs;

    vlt.AddKey("A", 0);

    // ....1....5....9....
    vlt.CreateRecord({{"A", 1}});
    vlt.CreateRecord({{"A", 5}});
    vlt.CreateRecord({{"A", 9}});

    //  xx
    // ....1....5....9....
    vlt.RequestInterval("A", -1, 0, vrs);
    COMPARE_VAULT(vrs, {});

    vlt.RequestInterval("A", -1, 0, vrs, false);
    COMPARE_VAULT(vrs, {});

    vlt.RequestInterval("A", -1, 0, vrs, true, false);
    COMPARE_VAULT(vrs, {});

    vlt.RequestInterval("A", -1, 0, vrs, false, false);
    COMPARE_VAULT(vrs, {});

    //  x  x
    // ....1....5....9....
    vlt.RequestInterval("A", -1, 1, vrs);
    COMPARE_VAULT(vrs, {{{"A", 1}}});

    vlt.RequestInterval("A", -1, 1, vrs, false);
    COMPARE_VAULT(vrs, {{{"A", 1}}});

    vlt.RequestInterval("A", -1, 1, vrs, true, false);
    COMPARE_VAULT(vrs, {});

    vlt.RequestInterval("A", -1, 1, vrs, false, false);
    COMPARE_VAULT(vrs, {});

    //  x    x
    // ....1....5....9....
    vlt.RequestInterval("A", -1, 2, vrs);
    COMPARE_VAULT(vrs, {{{"A", 1}}});

    vlt.RequestInterval("A", -1, 2, vrs, false);
    COMPARE_VAULT(vrs, {{{"A", 1}}});

    vlt.RequestInterval("A", -1, 2, vrs, true, false);
    COMPARE_VAULT(vrs, {{{"A", 1}}});

    vlt.RequestInterval("A", -1, 2, vrs, false, false);
    COMPARE_VAULT(vrs, {{{"A", 1}}});

    //     x x
    // ....1....5....9....
    vlt.RequestInterval("A", 1, 2, vrs);
    COMPARE_VAULT(vrs, {{{"A", 1}}});

    vlt.RequestInterval("A", 1, 2, vrs, false);
    COMPARE_VAULT(vrs, {});

    vlt.RequestInterval("A", 1, 2, vrs, true, false);
    COMPARE_VAULT(vrs, {{{"A", 1}}});

    vlt.RequestInterval("A", 1, 2, vrs, false, false);
    COMPARE_VAULT(vrs, {});

    //       xx
    // ....1....5....9....
    vlt.RequestInterval("A", 2, 3, vrs);
    COMPARE_VAULT(vrs, {});

    vlt.RequestInterval("A", 2, 3, vrs, false);
    COMPARE_VAULT(vrs, {});

    vlt.RequestInterval("A", 2, 3, vrs, true, false);
    COMPARE_VAULT(vrs, {});

    vlt.RequestInterval("A", 2, 3, vrs, false, false);
    COMPARE_VAULT(vrs, {});

    //       x  x
    // ....1....5....9....
    vlt.RequestInterval("A", 2, 5, vrs);
    COMPARE_VAULT(vrs, {{{"A", 5}}});

    vlt.RequestInterval("A", 2, 5, vrs, false);
    COMPARE_VAULT(vrs, {{{"A", 5}}});

    vlt.RequestInterval("A", 2, 5, vrs, true, false);
    COMPARE_VAULT(vrs, {});

    vlt.RequestInterval("A", 2, 5, vrs, false, false);
    COMPARE_VAULT(vrs, {});

    //       x    x
    // ....1....5....9....
    vlt.RequestInterval("A", 2, 6, vrs);
    COMPARE_VAULT(vrs, {{{"A", 5}}});

    vlt.RequestInterval("A", 2, 6, vrs, false);
    COMPARE_VAULT(vrs, {{{"A", 5}}});

    vlt.RequestInterval("A", 2, 6, vrs, true, false);
    COMPARE_VAULT(vrs, {{{"A", 5}}});

    vlt.RequestInterval("A", 2, 6, vrs, false, false);
    COMPARE_VAULT(vrs, {{{"A", 5}}});

    //          x  x
    // ....1....5....9....
    vlt.RequestInterval("A", 5, 6, vrs);
    COMPARE_VAULT(vrs, {{{"A", 5}}});

    vlt.RequestInterval("A", 5, 6, vrs, false);
    COMPARE_VAULT(vrs, {});

    vlt.RequestInterval("A", 5, 6, vrs, true, false);
    COMPARE_VAULT(vrs, {{{"A", 5}}});

    vlt.RequestInterval("A", 5, 6, vrs, false, false);
    COMPARE_VAULT(vrs, {});

    //            xx
    // ....1....5....9....
    vlt.RequestInterval("A", 6, 7, vrs);
    COMPARE_VAULT(vrs, {});

    vlt.RequestInterval("A", 6, 7, vrs, false);
    COMPARE_VAULT(vrs, {});

    vlt.RequestInterval("A", 6, 7, vrs, true, false);
    COMPARE_VAULT(vrs, {});

    vlt.RequestInterval("A", 6, 7, vrs, false, false);
    COMPARE_VAULT(vrs, {});

    //            x  x
    // ....1....5....9....
    vlt.RequestInterval("A", 6, 9, vrs);
    COMPARE_VAULT(vrs, {{{"A", 9}}});

    vlt.RequestInterval("A", 6, 9, vrs, false);
    COMPARE_VAULT(vrs, {{{"A", 9}}});

    vlt.RequestInterval("A", 6, 9, vrs, true, false);
    COMPARE_VAULT(vrs, {});

    vlt.RequestInterval("A", 6, 9, vrs, false, false);
    COMPARE_VAULT(vrs, {});

    //            x    x
    // ....1....5....9....
    vlt.RequestInterval("A", 6, 10, vrs);
    COMPARE_VAULT(vrs, {{{"A", 9}}});

    vlt.RequestInterval("A", 6, 10, vrs, false);
    COMPARE_VAULT(vrs, {{{"A", 9}}});

    vlt.RequestInterval("A", 6, 10, vrs, true, false);
    COMPARE_VAULT(vrs, {{{"A", 9}}});

    vlt.RequestInterval("A", 6, 10, vrs, false, false);
    COMPARE_VAULT(vrs, {{{"A", 9}}});

    //               x x
    // ....1....5....9....
    vlt.RequestInterval("A", 9, 10, vrs);
    COMPARE_VAULT(vrs, {{{"A", 9}}});

    vlt.RequestInterval("A", 9, 10, vrs, false);
    COMPARE_VAULT(vrs, {});

    vlt.RequestInterval("A", 9, 10, vrs, true, false);
    COMPARE_VAULT(vrs, {{{"A", 9}}});

    vlt.RequestInterval("A", 9, 10, vrs, false, false);
    COMPARE_VAULT(vrs, {});

    //                 xx
    // ....1....5....9....
    vlt.RequestInterval("A", 10, 11, vrs);
    COMPARE_VAULT(vrs, {});

    vlt.RequestInterval("A", 10, 11, vrs, false);
    COMPARE_VAULT(vrs, {});

    vlt.RequestInterval("A", 10, 11, vrs, true, false);
    COMPARE_VAULT(vrs, {});

    vlt.RequestInterval("A", 10, 11, vrs, false, false);
    COMPARE_VAULT(vrs, {});
)

TEST_BODY(RequestInterval, SameBoundaries,
    Vault vlt;
    VaultRecordSet vrs;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 1}, {"B", std::string("a")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("b")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("bb")}});
    vlt.CreateRecord({{"A", 3}, {"B", std::string("c")}});

    vor = vlt.RequestInterval("A", 2, 2, vrs);

    TEST_ASSERT(vrs.Size() == 2);

    COMPARE_VAULT(vrs, {
        {{"A", 2}, {"B", std::string("b")}},
        {{"A", 2}, {"B", std::string("bb")}},
    });

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));


    vor = vlt.RequestInterval("A", 2, 2, vrs, false);

    TEST_ASSERT(vrs.Size() == 0);
    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));


    vor = vlt.RequestInterval("A", 2, 2, vrs, true, false);

    TEST_ASSERT(vrs.Size() == 0);
    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));


    vor = vlt.RequestInterval("A", 2, 2, vrs, false, false);

    TEST_ASSERT(vrs.Size() == 0);
    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));
)

TEST_BODY(RequestInterval, IncorrectBoundaries,
    Vault vlt;
    VaultRecordSet vrs;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 1}, {"B", std::string("a")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("b")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("bb")}});
    vlt.CreateRecord({{"A", 3}, {"B", std::string("c")}});

    vor = vlt.RequestInterval("A", 2, -2, vrs);

    TEST_ASSERT(vrs.Size() == 0);
    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));
)

TEST_BODY(RequestInterval, WrongKey,
    Vault vlt;
    VaultRecordSet vrs;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 1}, {"B", std::string("a")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("b")}});
    vlt.CreateRecord({{"A", 3}, {"B", std::string("c")}});

    vor = vlt.RequestInterval("Z", 1, 1, vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "Z", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongKey, SavedType == typeid(void));
)

TEST_BODY(RequestInterval, WrongType,
    Vault vlt;
    VaultRecordSet vrs;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 1}, {"B", std::string("a")}});
    vlt.CreateRecord({{"A", 2}, {"B", std::string("b")}});
    vlt.CreateRecord({{"A", 3}, {"B", std::string("c")}});

    vor = vlt.RequestInterval("B", 1, 1, vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "B", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongType, SavedType == typeid(std::string));
)

TEST_BODY(Request, CorrectRequest,
    Vault vlt;
    VaultRecordSet vrs;
    VaultOperationResult vor;
    std::vector<std::vector<std::pair<std::string, TypeWrapper>>> records;

    vlt.AddKey("A", 0);

    for (int i = 0; i < 10; ++i) vlt.CreateRecord({{"A", i}});

    // Equal
    COMPARE_CORRECT_REQUEST(Equal("A", 2), {{{"A", 2}}})

    // Greater
    COMPARE_CORRECT_REQUEST(Greater("A", 7), {{{"A", 8}}, {{"A", 9}}});

    // Greater or equal
    COMPARE_CORRECT_REQUEST(GreaterOrEqual("A", 7), {{{"A", 7}}, {{"A", 8}}, {{"A", 9}}});

    // Less
    COMPARE_CORRECT_REQUEST(Less("A", 2), {{{"A", 0}}, {{"A", 1}}});

    // Less or equal
    COMPARE_CORRECT_REQUEST(LessOrEqual("A", 2), {{{"A", 0}}, {{"A", 1}}, {{"A", 2}}});
)

TEST_BODY(Request, CorrectRequestWithPredicat,
    Vault vlt;
    VaultRecordSet vrs;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddKey("B", 0);

    for (int i = 0; i < 10; ++i) vlt.CreateRecord({{"A", i}, {"B", 100 - i}});

    for (int i = 0; i < 4; ++i) vlt.CreateRecord({{"A", 5}, {"B", i}});

    // Equal
    vor = vlt.Request(Equal("A", 5, [](const VaultRecordRef& ref) -> bool
    {
        int b = 0;
        ref.GetData("B", b);
        if (b % 2 == 0) return true;
        else return false;
    }), vrs);

    TEST_ASSERT(vrs.Size() == 2);

    COMPARE_VAULT(vrs, {
        {{"A", 5}, {"B", 0}},
        {{"A", 5}, {"B", 2}},
    });

    COMPARE_OPERATION(vor, IsOperationSuccess == true, ResultCode == VaultOperationResultCode::Success);


    // Greater
    vor = vlt.Request(Greater("A", 7, [](const VaultRecordRef& ref) -> bool
    {
        int b = 0;
        ref.GetData("B", b);
        if (b % 2 == 0) return true;
        else return false;
    }), vrs);

    TEST_ASSERT(vrs.Size() == 1);

    COMPARE_VAULT(vrs, {{{"A", 8}, {"B", 92}}});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, ResultCode == VaultOperationResultCode::Success);


    // Greater or equal
    vor = vlt.Request(GreaterOrEqual("A", 7, [](const VaultRecordRef& ref) -> bool
    {
        int b = 0;
        ref.GetData("B", b);
        if (b % 2 == 0) return true;
        else return false;
    }), vrs);

    TEST_ASSERT(vrs.Size() == 1);

    COMPARE_VAULT(vrs, {{{"A", 8}, {"B", 92}}});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, ResultCode == VaultOperationResultCode::Success);


    // Less
    vor = vlt.Request(Less("A", 2, [](const VaultRecordRef& ref) -> bool
    {
        int b = 0;
        ref.GetData("B", b);
        if (b % 2 == 0) return true;
        else return false;
    }), vrs);

    TEST_ASSERT(vrs.Size() == 1);

    COMPARE_VAULT(vrs, {{{"A", 0}, {"B", 100}}});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, ResultCode == VaultOperationResultCode::Success);


    // Less or equal
    vor = vlt.Request(LessOrEqual("A", 2, [](const VaultRecordRef& ref) -> bool
    {
        int b = 0;
        ref.GetData("B", b);
        if (b % 2 == 0) return true;
        else return false;
    }), vrs);

    TEST_ASSERT(vrs.Size() == 2);

    COMPARE_VAULT(vrs, {{{"A", 0}, {"B", 100}}, {{"A", 2}, {"B", 98}}});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, ResultCode == VaultOperationResultCode::Success);
)

TEST_BODY(Request, BoundaryValues,
    Vault vlt;
    VaultRecordSet vrs;
    VaultOperationResult vor;
    std::vector<std::vector<std::pair<std::string, TypeWrapper>>> records;

    vlt.AddKey("A", 0);

    // ...1...3...
    vlt.CreateRecord({{"A", 1}});
    vlt.CreateRecord({{"A", 3}});


    // Greater
    //  x
    // ...1...3...
    COMPARE_CORRECT_REQUEST(Greater("A", 0), {{{"A", 1}}, {{"A", 3}}});

    //    x
    // ...1...3...
    COMPARE_CORRECT_REQUEST(Greater("A", 1), {{{"A", 3}}});

    //      x
    // ...1...3...
    COMPARE_CORRECT_REQUEST(Greater("A", 2), {{{"A", 3}}});

    //        x
    // ...1...3...
    COMPARE_CORRECT_REQUEST(Greater("A", 3), {});

    //          x
    // ...1...3...
    COMPARE_CORRECT_REQUEST(Greater("A", 4), {});


    // Greater or equal
    //  x
    // ...1...3...
    COMPARE_CORRECT_REQUEST(GreaterOrEqual("A", 0), {{{"A", 1}}, {{"A", 3}}});

    //    x
    // ...1...3...
    COMPARE_CORRECT_REQUEST(GreaterOrEqual("A", 1), {{{"A", 1}}, {{"A", 3}}});

    //      x
    // ...1...3...
    COMPARE_CORRECT_REQUEST(GreaterOrEqual("A", 2), {{{"A", 3}}});

    //        x
    // ...1...3...
    COMPARE_CORRECT_REQUEST(GreaterOrEqual("A", 3), {{{"A", 3}}});

    //          x
    // ...1...3...
    COMPARE_CORRECT_REQUEST(GreaterOrEqual("A", 4), {});


    // Less
    //  x
    // ...1...3...
    COMPARE_CORRECT_REQUEST(Less("A", 0), {});

    //    x
    // ...1...3...
    COMPARE_CORRECT_REQUEST(Less("A", 1), {});

    //      x
    // ...1...3...
    COMPARE_CORRECT_REQUEST(Less("A", 2), {{{"A", 1}}});

    //        x
    // ...1...3...
    COMPARE_CORRECT_REQUEST(Less("A", 3), {{{"A", 1}}});

    //          x
    // ...1...3...
    COMPARE_CORRECT_REQUEST(Less("A", 4), {{{"A", 1}}, {{"A", 3}}});


    // Less or equal
    //  x
    // ...1...3...
    COMPARE_CORRECT_REQUEST(LessOrEqual("A", 0), {});

    //    x
    // ...1...3...
    COMPARE_CORRECT_REQUEST(LessOrEqual("A", 1), {{{"A", 1}}});

    //      x
    // ...1...3...
    COMPARE_CORRECT_REQUEST(LessOrEqual("A", 2), {{{"A", 1}}});

    //        x
    // ...1...3...
    COMPARE_CORRECT_REQUEST(LessOrEqual("A", 3), {{{"A", 1}}, {{"A", 3}}});

    //          x
    // ...1...3...
    COMPARE_CORRECT_REQUEST(LessOrEqual("A", 4), {{{"A", 1}}, {{"A", 3}}});
)

TEST_BODY(Request, WrongKey,
    Vault vlt;
    VaultRecordSet vrs;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);

    for (int i = 0; i < 10; ++i) vlt.CreateRecord({{"A", i}});

    // Equal
    vor = vlt.Request(Equal("Z", 2), vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "Z", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongKey, SavedType == typeid(void));


    // Greater
    vor = vlt.Request(Greater("Z", 7), vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "Z", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongKey, SavedType == typeid(void));


    // Greater or equal
    vor = vlt.Request(GreaterOrEqual("Z", 7), vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "Z", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongKey, SavedType == typeid(void));


    // Less
    vor = vlt.Request(Less("Z", 2), vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "Z", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongKey, SavedType == typeid(void));


    // Less or equal
    vor = vlt.Request(LessOrEqual("Z", 2), vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "Z", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongKey, SavedType == typeid(void));
)

TEST_BODY(Request, WrongType,
    Vault vlt;
    VaultRecordSet vrs;
    VaultOperationResult vor;
    std::string s;

    vlt.AddKey("A", 0);

    for (int i = 0; i < 10; ++i) vlt.CreateRecord({{"A", i}});

    // Equal
    vor = vlt.Request(Equal("A", s), vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(std::string), ResultCode == VaultOperationResultCode::WrongType, SavedType == typeid(int));


    // Greater
    vor = vlt.Request(Greater("A", s), vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(std::string), ResultCode == VaultOperationResultCode::WrongType, SavedType == typeid(int));


    // Greater or equal
    vor = vlt.Request(GreaterOrEqual("A", s), vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(std::string), ResultCode == VaultOperationResultCode::WrongType, SavedType == typeid(int));


    // Less
    vor = vlt.Request(Less("A", s), vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(std::string), ResultCode == VaultOperationResultCode::WrongType, SavedType == typeid(int));


    // Less or equal
    vor = vlt.Request(LessOrEqual("A", s), vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(std::string), ResultCode == VaultOperationResultCode::WrongType, SavedType == typeid(int));
)

TEST_BODY(Request, ComplexRequestAnd,
    Vault vlt;
    VaultRecordSet vrs;
    VaultOperationResult vor;
    std::vector<std::vector<std::pair<std::string, TypeWrapper>>> records;

    vlt.AddKey("A", 0);
    vlt.AddKey("B", 0);

    for (int i = 0; i < 10; ++i) vlt.CreateRecord({{"A", i}, {"B", i}});

    // Equal
    COMPARE_CORRECT_REQUEST(Equal("A", 2) && Equal("B", 2), {{{"A", 2}, {"B", 2}}});

    COMPARE_CORRECT_REQUEST(Equal("A", 2) && Equal("B", 3), {});

    COMPARE_CORRECT_REQUEST(Equal("A", 2) && Greater("B", 1), {{{"A", 2}, {"B", 2}}});

    COMPARE_CORRECT_REQUEST(Equal("A", 2) && GreaterOrEqual("B", 2), {{{"A", 2}, {"B", 2}}});

    COMPARE_CORRECT_REQUEST(Equal("A", 2) && Less("B", 5), {{{"A", 2}, {"B", 2}}});

    COMPARE_CORRECT_REQUEST(Equal("A", 2) && LessOrEqual("B", 2), {{{"A", 2}, {"B", 2}}});


    // Greater
    COMPARE_CORRECT_REQUEST(Greater("A", 2) && Equal("B", 9), {{{"A", 9}, {"B", 9}}});

    COMPARE_CORRECT_REQUEST(Greater("A", 7) && Greater("B", 1), {{{"A", 8}, {"B", 8}}, {{"A", 9}, {"B", 9}}});

    COMPARE_CORRECT_REQUEST(Greater("A", 8) && GreaterOrEqual("B", 8), {{{"A", 9}, {"B", 9}}});

    COMPARE_CORRECT_REQUEST(Greater("A", 2) && Less("B", 4), {{{"A", 3}, {"B", 3}}});

    COMPARE_CORRECT_REQUEST(Greater("A", 2) && LessOrEqual("B", 3), {{{"A", 3}, {"B", 3}}});


    // Greater or equal
    COMPARE_CORRECT_REQUEST(GreaterOrEqual("A", 2) && Equal("B", 5), {{{"A", 5}, {"B", 5}}});

    COMPARE_CORRECT_REQUEST(GreaterOrEqual("A", 8) && Greater("B", 1), {{{"A", 8}, {"B", 8}}, {{"A", 9}, {"B", 9}}});

    COMPARE_CORRECT_REQUEST(GreaterOrEqual("A", 8) && GreaterOrEqual("B", 9), {{{"A", 9}, {"B", 9}}});

    COMPARE_CORRECT_REQUEST(GreaterOrEqual("A", 2) && Less("B", 4), {{{"A", 2}, {"B", 2}}, {{"A", 3}, {"B", 3}}});

    COMPARE_CORRECT_REQUEST(GreaterOrEqual("A", 2) && LessOrEqual("B", 3), {{{"A", 2}, {"B", 2}}, {{"A", 3}, {"B", 3}}});


    // Less
    COMPARE_CORRECT_REQUEST(Less("A", 7) && Equal("B", 5), {{{"A", 5}, {"B", 5}}});

    COMPARE_CORRECT_REQUEST(Less("A", 3) && Greater("B", 1), {{{"A", 2}, {"B", 2}}});

    COMPARE_CORRECT_REQUEST(Less("A", 8) && GreaterOrEqual("B", 7), {{{"A", 7}, {"B", 7}}});

    COMPARE_CORRECT_REQUEST(Less("A", 2) && Less("B", 4), {{{"A", 0}, {"B", 0}}, {{"A", 1}, {"B", 1}}});

    COMPARE_CORRECT_REQUEST(Less("A", 2) && LessOrEqual("B", 1), {{{"A", 0}, {"B", 0}}, {{"A", 1}, {"B", 1}}});


    // Less or equal
    COMPARE_CORRECT_REQUEST(LessOrEqual("A", 7) && Equal("B", 5), {{{"A", 5}, {"B", 5}}});

    COMPARE_CORRECT_REQUEST(LessOrEqual("A", 3) && Greater("B", 2), {{{"A", 3}, {"B", 3}}});

    COMPARE_CORRECT_REQUEST(LessOrEqual("A", 8) && GreaterOrEqual("B", 8), {{{"A", 8}, {"B", 8}}});

    COMPARE_CORRECT_REQUEST(LessOrEqual("A", 2) && Less("B", 4), {{{"A", 0}, {"B", 0}}, {{"A", 1}, {"B", 1}}, {{"A", 2}, {"B", 2}}});

    COMPARE_CORRECT_REQUEST(LessOrEqual("A", 2) && LessOrEqual("B", 1), {{{"A", 0}, {"B", 0}}, {{"A", 1}, {"B", 1}}});
)

TEST_BODY(Request, ComplexRequestOr,
    Vault vlt;
    VaultRecordSet vrs;
    VaultOperationResult vor;
    std::vector<std::vector<std::pair<std::string, TypeWrapper>>> records;

    vlt.AddKey("A", 0);
    vlt.AddKey("B", 0);

    for (int i = 0; i < 10; ++i) vlt.CreateRecord({{"A", i}, {"B", i}});

    // Equal
    COMPARE_CORRECT_REQUEST(Equal("A", 2) || Equal("B", 3), {{{"A", 2}, {"B", 2}}, {{"A", 3}, {"B", 3}}});

    COMPARE_CORRECT_REQUEST(Equal("A", 2) || Greater("B", 8), {{{"A", 2}, {"B", 2}}, {{"A", 9}, {"B", 9}}});

    COMPARE_CORRECT_REQUEST(Equal("A", 2) || GreaterOrEqual("B", 9), {{{"A", 2}, {"B", 2}}, {{"A", 9}, {"B", 9}}});

    COMPARE_CORRECT_REQUEST(Equal("A", 2) || Less("B", 2), {{{"A", 0}, {"B", 0}}, {{"A", 1}, {"B", 1}}, {{"A", 2}, {"B", 2}}});

    COMPARE_CORRECT_REQUEST(Equal("A", 2) || LessOrEqual("B", 1), {{{"A", 0}, {"B", 0}}, {{"A", 1}, {"B", 1}}, {{"A", 2}, {"B", 2}}});


    // Greater
    COMPARE_CORRECT_REQUEST(Greater("A", 8) || Equal("B", 0), {{{"A", 0}, {"B", 0}}, {{"A", 9}, {"B", 9}}});

    COMPARE_CORRECT_REQUEST(Greater("A", 8) || Greater("B", 10), {{{"A", 9}, {"B", 9}}});

    COMPARE_CORRECT_REQUEST(Greater("A", 8) || GreaterOrEqual("B", 9), {{{"A", 9}, {"B", 9}}});

    COMPARE_CORRECT_REQUEST(Greater("A", 8) || Less("B", 1), {{{"A", 0}, {"B", 0}}, {{"A", 9}, {"B", 9}}});

    COMPARE_CORRECT_REQUEST(Greater("A", 8) || LessOrEqual("B", 0), {{{"A", 0}, {"B", 0}}, {{"A", 9}, {"B", 9}}});


    // Greater or equal
    COMPARE_CORRECT_REQUEST(GreaterOrEqual("A", 9) || Equal("B", 5), {{{"A", 5}, {"B", 5}}, {{"A", 9}, {"B", 9}}});

    COMPARE_CORRECT_REQUEST(GreaterOrEqual("A", 8) || Greater("B", 8), {{{"A", 8}, {"B", 8}}, {{"A", 9}, {"B", 9}}});

    COMPARE_CORRECT_REQUEST(GreaterOrEqual("A", 8) || GreaterOrEqual("B", 9), {{{"A", 8}, {"B", 8}}, {{"A", 9}, {"B", 9}}});

    COMPARE_CORRECT_REQUEST(GreaterOrEqual("A", 9) || Less("B", 1), {{{"A", 0}, {"B", 0}}, {{"A", 9}, {"B", 9}}});

    COMPARE_CORRECT_REQUEST(GreaterOrEqual("A", 9) || LessOrEqual("B", 0), {{{"A", 0}, {"B", 0}}, {{"A", 9}, {"B", 9}}});


    // Less
    COMPARE_CORRECT_REQUEST(Less("A", 1) || Equal("B", 5), {{{"A", 0}, {"B", 0}}, {{"A", 5}, {"B", 5}}});

    COMPARE_CORRECT_REQUEST(Less("A", 1) || Greater("B", 8), {{{"A", 0}, {"B", 0}}, {{"A", 9}, {"B", 9}}});

    COMPARE_CORRECT_REQUEST(Less("A", 1) || GreaterOrEqual("B", 9), {{{"A", 0}, {"B", 0}}, {{"A", 9}, {"B", 9}}});

    COMPARE_CORRECT_REQUEST(Less("A", 2) || Less("B", 1), {{{"A", 0}, {"B", 0}}, {{"A", 1}, {"B", 1}}});

    COMPARE_CORRECT_REQUEST(Less("A", 1) || LessOrEqual("B", 1), {{{"A", 0}, {"B", 0}}, {{"A", 1}, {"B", 1}}});


    // Less or equal
    COMPARE_CORRECT_REQUEST(LessOrEqual("A", 1) || Equal("B", 5), {{{"A", 0}, {"B", 0}}, {{"A", 1}, {"B", 1}}, {{"A", 5}, {"B", 5}}});

    COMPARE_CORRECT_REQUEST(LessOrEqual("A", 1) || Greater("B", 8), {{{"A", 0}, {"B", 0}}, {{"A", 1}, {"B", 1}}, {{"A", 9}, {"B", 9}}});

    COMPARE_CORRECT_REQUEST(LessOrEqual("A", 1) || GreaterOrEqual("B", 9), {{{"A", 0}, {"B", 0}}, {{"A", 1}, {"B", 1}}, {{"A", 9}, {"B", 9}}});

    COMPARE_CORRECT_REQUEST(LessOrEqual("A", 1) || Less("B", 3), {{{"A", 0}, {"B", 0}}, {{"A", 1}, {"B", 1}}, {{"A", 2}, {"B", 2}}});

    COMPARE_CORRECT_REQUEST(LessOrEqual("A", 2) || LessOrEqual("B", 1), {{{"A", 0}, {"B", 0}}, {{"A", 1}, {"B", 1}}, {{"A", 2}, {"B", 2}}});
)

TEST_BODY(Request, ComplexRequest,
    Vault vlt;
    VaultRecordSet vrs;
    VaultOperationResult vor;
    VaultRecordRef vrr;

    vlt.AddKey("A", 0);
    vlt.AddKey("B", 0);
    vlt.AddKey<std::string>("C", "");
    vlt.AddKey<bool>("D", false);

    for (int i = 0; i < 10; ++i)
    {
        vlt.CreateRecord(vrr, {{"A", i}, {"B", i}, {"C", std::to_string(i)}});
        if (i == 5 || i == 9) vrr.SetData("D", true);
    }
    
    vlt.Request(Greater("A", 2) && Less("B", 8) && (Equal("C", std::string("3")) || Equal("C", std::string("4"))) || Equal("D", true), vrs);
    
    CompareVault(vrs, {
        {{"A", 3}, {"B", 3}, {"C", std::string("3")}, {"D", false}},
        {{"A", 4}, {"B", 4}, {"C", std::string("4")}, {"D", false}},
        {{"A", 5}, {"B", 5}, {"C", std::string("5")}, {"D", true}},
        {{"A", 9}, {"B", 9}, {"C", std::string("9")}, {"D", true}},
    });
)

TEST_BODY(Request, ComplexRequestWrongKey,
    Vault vlt;
    VaultRecordSet vrs;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddKey("B", 0);

    for (int i = 0; i < 10; ++i) vlt.CreateRecord({{"A", i}, {"B", i}});

    vor = vlt.Request(Equal("A", 0) && Equal("Z", 0), vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "Z", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongKey, SavedType == typeid(void));


    vor = vlt.Request(Equal("Z", 0) && Equal("Y", 0), vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "Z", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongKey, SavedType == typeid(void));
)

TEST_BODY(Request, ComplexRequestWrongType,
    Vault vlt;
    VaultRecordSet vrs;
    VaultOperationResult vor;
    std::string s;

    vlt.AddKey("A", 0);
    vlt.AddKey("B", 0);

    for (int i = 0; i < 10; ++i) vlt.CreateRecord({{"A", i}, {"B", i}});

    vor = vlt.Request(Equal("A", 0) && Equal("B", s), vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "B", 
        RequestedType == typeid(std::string), ResultCode == VaultOperationResultCode::WrongType, SavedType == typeid(int));


    vor = vlt.Request(Equal("A", s) && Equal("B", 0), vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(std::string), ResultCode == VaultOperationResultCode::WrongType, SavedType == typeid(int));
)

TEST_BODY(DropVault, Drop,
    Vault vlt;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<int>("B");

    for (int i = 0; i < 1000; ++i) vlt.CreateRecord({{"A", i}, {"B", i}});

    vlt.DropVault();

    TEST_ASSERT(vlt.Size() == 0);
    TEST_ASSERT(vlt.GetKeys() == std::vector<std::string>());
    TEST_ASSERT(vlt.GetUniqueKeys() == std::vector<std::string>());

    COMPARE_VAULT(vlt, {});
)

TEST_BODY(DropVault, DropEmpty,
    Vault vlt;

    vlt.DropVault();

    TEST_ASSERT(vlt.Size() == 0);
    TEST_ASSERT(vlt.GetKeys() == std::vector<std::string>());
    TEST_ASSERT(vlt.GetUniqueKeys() == std::vector<std::string>());

    COMPARE_VAULT(vlt, {});
)

TEST_BODY(DropVault, DropSecond,
    Vault vlt;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<int>("B");

    for (int i = 0; i < 1000; ++i) vlt.CreateRecord({{"A", i}, {"B", i}});

    vlt.DropVault();
    vlt.DropVault();

    TEST_ASSERT(vlt.Size() == 0);
    TEST_ASSERT(vlt.GetKeys() == std::vector<std::string>());
    TEST_ASSERT(vlt.GetUniqueKeys() == std::vector<std::string>());

    COMPARE_VAULT(vlt, {});
)

TEST_BODY(DropVault, DropWithDependent,
    Vault vlt;
    VaultRecordSet vrs;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<int>("B");

    for (int i = 0; i < 1000; ++i) vlt.CreateRecord({{"A", 0}, {"B", i}});

    vlt.RequestEqual("A", 0, vrs);
    TEST_ASSERT(vrs.Size() == 1000);

    vlt.DropVault();

    TEST_ASSERT(vrs.Size() == 0);
    TEST_ASSERT(vrs.GetKeys() == std::vector<std::string>());
    TEST_ASSERT(vrs.GetUniqueKeys() == std::vector<std::string>());

    COMPARE_VAULT(vrs, {});
)

TEST_BODY(DropData, Drop,
    Vault vlt;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<int>("B");

    for (int i = 0; i < 1000; ++i) vlt.CreateRecord({{"A", i}, {"B", i}});

    vlt.DropData();

    TEST_ASSERT(vlt.Size() == 0);
    TEST_ASSERT(vlt.GetKeys() == std::vector<std::string>({"A", "B"}));
    TEST_ASSERT(vlt.GetUniqueKeys() == std::vector<std::string>({"B"}));

    COMPARE_VAULT(vlt, {});
)

TEST_BODY(DropData, DropEmpty,
    Vault vlt;

    vlt.DropData();

    TEST_ASSERT(vlt.Size() == 0);
    TEST_ASSERT(vlt.GetKeys() == std::vector<std::string>());
    TEST_ASSERT(vlt.GetUniqueKeys() == std::vector<std::string>());

    COMPARE_VAULT(vlt, {});
)

TEST_BODY(DropData, DropSecond,
    Vault vlt;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<int>("B");

    for (int i = 0; i < 1000; ++i) vlt.CreateRecord({{"A", i}, {"B", i}});

    vlt.DropData();
    vlt.DropData();

    TEST_ASSERT(vlt.Size() == 0);
    TEST_ASSERT(vlt.GetKeys() == std::vector<std::string>({"A", "B"}));
    TEST_ASSERT(vlt.GetUniqueKeys() == std::vector<std::string>({"B"}));

    COMPARE_VAULT(vlt, {});
)

TEST_BODY(DropData, DropWithDependent,
    Vault vlt;
    VaultRecordSet vrs;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<int>("B");

    for (int i = 0; i < 1000; ++i) vlt.CreateRecord({{"A", 0}, {"B", i}});

    vlt.RequestEqual("A", 0, vrs);
    TEST_ASSERT(vrs.Size() == 1000);

    vlt.DropData();

    TEST_ASSERT(vrs.Size() == 0);
    TEST_ASSERT(vrs.GetKeys() == std::vector<std::string>({"A", "B"}));
    TEST_ASSERT(vrs.GetUniqueKeys() == std::vector<std::string>({"B"}));

    COMPARE_VAULT(vrs, {});
)

TEST_BODY(EraseRecord, CorrectEraseByRef,
    Vault vlt;
    VaultRecordRef vrr;
    VaultOperationResult vor;
    bool res = false;

    vlt.AddKey("A", 0);

    vlt.CreateRecord(vrr, {{"A", 0}});

    res = vlt.EraseRecord(vrr);

    TEST_ASSERT(res);

    TEST_ASSERT(vlt.Size() == 0);

    COMPARE_VAULT(vlt, {});


    for (int i = 0; i < 1000; ++i) vlt.CreateRecord({{"A", i}});

    for (int i = 0; i < 1000; ++i) 
    {
        vlt.GetRecord("A", i, vrr);
        res = vlt.EraseRecord(vrr);

        TEST_ASSERT(res == true);
        TEST_ASSERT(vrr.IsValid() == false);

        vor = vlt.GetRecord("A", i, vrr);
        COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
            RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongValue, SavedType == typeid(int));
    }

    TEST_ASSERT(vlt.Size() == 0);

    COMPARE_VAULT(vlt, {});
)

TEST_BODY(EraseRecord, IncorrectEraseByRef,
    Vault vlt;
    VaultRecordRef vrr;
    bool res = false;

    res = vlt.EraseRecord(vrr);

    TEST_ASSERT(res == false);


    vlt.AddKey("A", 0);

    vlt.CreateRecord(vrr, {{"A", 0}});

    vlt.EraseRecord(vrr);
    res = vlt.EraseRecord(vrr);

    TEST_ASSERT(res == false);

    TEST_ASSERT(vlt.Size() == 0);

    COMPARE_VAULT(vlt, {});
)

TEST_BODY(EraseRecord, CorrectEraseByKeyAndValue,
    Vault vlt;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);

    vlt.CreateRecord(vrr, {{"A", 0}});

    vor = vlt.EraseRecord("A", 0);

    TEST_ASSERT(vlt.Size() == 0);

    COMPARE_VAULT(vlt, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));


    for (int i = 0; i < 1000; ++i) vlt.CreateRecord({{"A", i}});

    for (int i = 0; i < 1000; ++i) 
        vlt.EraseRecord("A", i);
    

    TEST_ASSERT(vlt.Size() == 0);

    COMPARE_VAULT(vlt, {});
)

TEST_BODY(EraseRecord, WrongKeyEraseByKeyAndValue,
    Vault vlt;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);

    vlt.CreateRecord(vrr, {{"A", 0}});

    vor = vlt.EraseRecord("Z", 0);

    TEST_ASSERT(vlt.Size() == 1);

    COMPARE_VAULT(vlt, {{{"A", 0}}});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "Z", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongKey, SavedType == typeid(void));
)

TEST_BODY(EraseRecord, WrongTypeEraseByKeyAndValue,
    Vault vlt;
    VaultRecordRef vrr;
    VaultOperationResult vor;
    std::string s;

    vlt.AddKey("A", 0);

    vlt.CreateRecord(vrr, {{"A", 0}});

    vor = vlt.EraseRecord("A", s);

    TEST_ASSERT(vlt.Size() == 1);

    COMPARE_VAULT(vlt, {{{"A", 0}}});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(std::string), ResultCode == VaultOperationResultCode::WrongType, SavedType == typeid(int));
)

TEST_BODY(EraseRecord, WrongValueEraseByKeyAndValue,
    Vault vlt;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);

    vlt.CreateRecord(vrr, {{"A", 0}});

    vor = vlt.EraseRecord("A", 1);

    TEST_ASSERT(vlt.Size() == 1);

    COMPARE_VAULT(vlt, {{{"A", 0}}});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongValue, SavedType == typeid(int));
)

TEST_BODY(EraseRecords, Erase,
    Vault vlt;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);

    for (int i = 0; i < 1000; ++i) vlt.CreateRecord({{"A", 0}});

    vor = vlt.EraseRecords("A", 0);

    TEST_ASSERT(vlt.Size() == 0);

    COMPARE_VAULT(vlt, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));
)

TEST_BODY(EraseRecords, WrongKeyErase,
    Vault vlt;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);

    vlt.CreateRecord(vrr, {{"A", 0}});

    vor = vlt.EraseRecords("Z", 0);

    TEST_ASSERT(vlt.Size() == 1);

    COMPARE_VAULT(vlt, {{{"A", 0}}});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "Z", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongKey, SavedType == typeid(void));
)

TEST_BODY(EraseRecords, WrongTypeErase,
    Vault vlt;
    VaultRecordRef vrr;
    VaultOperationResult vor;
    std::string s;

    vlt.AddKey("A", 0);

    vlt.CreateRecord(vrr, {{"A", 0}});

    vor = vlt.EraseRecords("A", s);

    TEST_ASSERT(vlt.Size() == 1);

    COMPARE_VAULT(vlt, {{{"A", 0}}});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(std::string), ResultCode == VaultOperationResultCode::WrongType, SavedType == typeid(int));
)

TEST_BODY(EraseRecords, WrongValueErase,
    Vault vlt;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);

    vlt.CreateRecord(vrr, {{"A", 0}});

    vor = vlt.EraseRecords("A", 1);

    TEST_ASSERT(vlt.Size() == 1);

    COMPARE_VAULT(vlt, {{{"A", 0}}});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongValue, SavedType == typeid(int));
)

TEST_BODY(Size, Default,
    Vault vlt;

    vlt.AddKey<std::size_t>("A", 0);

    for (std::size_t i = 0; i < 1000; ++i) 
    {
        TEST_ASSERT(vlt.Size() == i);
        vlt.CreateRecord({{"A", i}});
    }

    for (std::size_t i = 0; i < 1000; ++i)
    {
        TEST_ASSERT(vlt.Size() == 1000 - i);
        vlt.EraseRecord("A", i);
    }
)

TEST_BODY(GetSortedRecords, GetRecords,
    Vault vlt;
    std::vector<VaultRecordRef> refs;

    vlt.AddKey("A", 0);

    refs = vlt.GetSortedRecords("A");

    TEST_ASSERT(refs.size() == 0);


    for (int i = 0; i < 1000; ++i) vlt.CreateRecord({{"A", i}});
    
    refs = vlt.GetSortedRecords("A");

    TEST_ASSERT(refs.size() == 1000);

    for (int i = 0; i < 1000; ++i)
    {
        int a = 0;
        refs[i].GetData("A", a);
        TEST_ASSERT(a == i);
    }
)

TEST_BODY(GetSortedRecords, GetRecordsReverse,
    Vault vlt;
    std::vector<VaultRecordRef> refs;

    vlt.AddKey("A", 0);

    for (int i = 0; i < 1000; ++i) vlt.CreateRecord({{"A", i}});
    
    refs = vlt.GetSortedRecords("A", true);

    TEST_ASSERT(refs.size() == 1000);

    for (int i = 0; i < 1000; ++i)
    {
        int a = 0;
        refs[i].GetData("A", a);
        TEST_ASSERT(a == 999 - i);
    }
)

TEST_BODY(GetSortedRecords, GetNotAllRecords,
    Vault vlt;
    std::vector<VaultRecordRef> refs;

    vlt.AddKey("A", 0);

    for (int i = 0; i < 1000; ++i) vlt.CreateRecord({{"A", i}});
    
    refs = vlt.GetSortedRecords("A", false, 100);

    TEST_ASSERT(refs.size() == 100);

    for (int i = 0; i < 100; ++i)
    {
        int a = 0;
        refs[i].GetData("A", a);
        TEST_ASSERT(a == i);
    }
)

TEST_BODY(GetSortedRecords, WrongKey,
    Vault vlt;
    std::vector<VaultRecordRef> refs;

    vlt.AddKey("A", 0);

    for (int i = 0; i < 1000; ++i) vlt.CreateRecord({{"A", i}});
    
    refs = vlt.GetSortedRecords("Z");

    TEST_ASSERT(refs.size() == 0);
)

TEST_BODY(SortBy, Sort,
    Vault vlt;

    vlt.AddKey("A", 0);

    for (int i = 0; i < 1000; ++i) vlt.CreateRecord({{"A", i}});

    int counter = 0;
    vlt.SortBy("A", [&](const VaultRecordRef& ref) -> bool
    {
        int i = 0;
        ref.GetData("A", i);
        TEST_ASSERT(counter == i);
        ++counter;
        return true;
    });

    TEST_ASSERT(counter == 1000);
)

TEST_BODY(SortBy, SortReverse,
    Vault vlt;

    vlt.AddKey("A", 0);

    for (int i = 0; i < 1000; ++i) vlt.CreateRecord({{"A", i}});

    int counter = 999;
    vlt.SortBy("A", [&](const VaultRecordRef& ref) -> bool
    {
        int i = 0;
        ref.GetData("A", i);
        TEST_ASSERT(counter == i);
        --counter;
        return true;
    }, true);

    TEST_ASSERT(counter == -1);
)

TEST_BODY(SortBy, SortNotAllRecords,
    Vault vlt;

    vlt.AddKey("A", 0);

    for (int i = 0; i < 1000; ++i) vlt.CreateRecord({{"A", i}});

    int counter = 0;
    vlt.SortBy("A", [&](const VaultRecordRef& ref) -> bool
    {
        int i = 0;
        ref.GetData("A", i);
        TEST_ASSERT(counter == i);
        ++counter;
        return true;
    }, false, 100);

    TEST_ASSERT(counter == 100);
)

TEST_BODY(SortBy, WrongKey,
    Vault vlt;

    vlt.AddKey("A", 0);

    for (int i = 0; i < 1000; ++i) vlt.CreateRecord({{"A", i}});

    int counter = 0;
    vlt.SortBy("Z", [&](const VaultRecordRef& ref) -> bool
    {
        int i = 0;
        ref.GetData("A", i);
        TEST_ASSERT(counter == i);
        ++counter;
        return true;
    });

    TEST_ASSERT(counter == 0);
)

TEST_BODY(ToJson, Default,
    Vault vlt;

    vlt.AddUniqueKey<std::size_t>("A");
    vlt.AddKey<std::string>("B", "none");

    vlt.CreateRecord({ {"A", std::size_t(1)}, {"B", std::string("a")} });
    vlt.CreateRecord({ {"A", std::size_t(2)}, {"B", std::string("b")} });
    vlt.CreateRecord({ {"A", std::size_t(3)}, {"B", std::string("c")} });

    std::string res;
    res = vlt.ToJson();

    TEST_ASSERT(res == R"({"Record0":{"A":3,"B":"c"},"Record1":{"A":2,"B":"b"},"Record2":{"A":1,"B":"a"}})");
)

TEST_BODY(ToJson, Format,
    Vault vlt;

    vlt.AddUniqueKey<std::size_t>("A");
    vlt.AddKey<std::string>("B", "none");

    vlt.CreateRecord({ {"A", std::size_t(1)}, {"B", std::string("a")} });
    vlt.CreateRecord({ {"A", std::size_t(2)}, {"B", std::string("b")} });
    vlt.CreateRecord({ {"A", std::size_t(3)}, {"B", std::string("c")} });

    std::string res;
    res = vlt.ToJson(true);

    TEST_ASSERT(res == R"({
  "Record0": {
    "A": 3,
    "B": "c"
  },
  "Record1": {
    "A": 2,
    "B": "b"
  },
  "Record2": {
    "A": 1,
    "B": "a"
  }
})");
)

TEST_BODY(ToJson, DiffTabSize,
    Vault vlt;

    vlt.AddUniqueKey<std::size_t>("A");
    vlt.AddKey<std::string>("B", "none");

    vlt.CreateRecord({ {"A", std::size_t(1)}, {"B", std::string("a")} });
    vlt.CreateRecord({ {"A", std::size_t(2)}, {"B", std::string("b")} });
    vlt.CreateRecord({ {"A", std::size_t(3)}, {"B", std::string("c")} });

    std::string res;
    res = vlt.ToJson(true, 1);

    TEST_ASSERT(res == R"({
 "Record0": {
  "A": 3,
  "B": "c"
 },
 "Record1": {
  "A": 2,
  "B": "b"
 },
 "Record2": {
  "A": 1,
  "B": "a"
 }
})");
)

TEST_BODY(ToJson, RecordTemplate,
    Vault vlt;

    vlt.AddUniqueKey<std::size_t>("A");
    vlt.AddKey<std::string>("B", "none");

    vlt.CreateRecord({ {"A", std::size_t(1)}, {"B", std::string("a")} });
    vlt.CreateRecord({ {"A", std::size_t(2)}, {"B", std::string("b")} });
    vlt.CreateRecord({ {"A", std::size_t(3)}, {"B", std::string("c")} });

    std::string res;
    res = vlt.ToJson(true, 2, true, "Rec");

    TEST_ASSERT(res == R"({
  "Rec0": {
    "A": 3,
    "B": "c"
  },
  "Rec1": {
    "A": 2,
    "B": "b"
  },
  "Rec2": {
    "A": 1,
    "B": "a"
  }
})");
)

TEST_BODY(ToJson, Array,
    Vault vlt;

    vlt.AddUniqueKey<std::size_t>("A");
    vlt.AddKey<std::string>("B", "none");

    vlt.CreateRecord({ {"A", std::size_t(1)}, {"B", std::string("a")} });
    vlt.CreateRecord({ {"A", std::size_t(2)}, {"B", std::string("b")} });
    vlt.CreateRecord({ {"A", std::size_t(3)}, {"B", std::string("c")} });

    std::string res;
    res = vlt.ToJson(false, 2, true, "Record", true);

    TEST_ASSERT(res == R"([{"A":3,"B":"c"},{"A":2,"B":"b"},{"A":1,"B":"a"}])");
)

TEST_BODY(ToJson, ArrayFormat,
    Vault vlt;

    vlt.AddUniqueKey<std::size_t>("A");
    vlt.AddKey<std::string>("B", "none");

    vlt.CreateRecord({ {"A", std::size_t(1)}, {"B", std::string("a")} });
    vlt.CreateRecord({ {"A", std::size_t(2)}, {"B", std::string("b")} });
    vlt.CreateRecord({ {"A", std::size_t(3)}, {"B", std::string("c")} });

    std::string res;
    res = vlt.ToJson(true, 2, true, "Record", true);

    TEST_ASSERT(res == R"([
  {
    "A": 3,
    "B": "c"
  },
  {
    "A": 2,
    "B": "b"
  },
  {
    "A": 1,
    "B": "a"
  }
])");
)

TEST_BODY(ToJson, ArrayDiffTabSize,
    Vault vlt;

    vlt.AddUniqueKey<std::size_t>("A");
    vlt.AddKey<std::string>("B", "none");

    vlt.CreateRecord({ {"A", std::size_t(1)}, {"B", std::string("a")} });
    vlt.CreateRecord({ {"A", std::size_t(2)}, {"B", std::string("b")} });
    vlt.CreateRecord({ {"A", std::size_t(3)}, {"B", std::string("c")} });

    std::string res;
    res = vlt.ToJson(true, 1, true, "Record", true);

    TEST_ASSERT(res == R"([
 {
  "A": 3,
  "B": "c"
 },
 {
  "A": 2,
  "B": "b"
 },
 {
  "A": 1,
  "B": "a"
 }
])");
)

TEST_BODY(ToJson, RecordId,
    Vault vlt;

    vlt.AddUniqueKey<std::size_t>("A");
    vlt.AddKey<std::string>("B", "none");

    vlt.CreateRecord({ {"A", std::size_t(1)}, {"B", std::string("a")} });
    vlt.CreateRecord({ {"A", std::size_t(2)}, {"B", std::string("b")} });
    vlt.CreateRecord({ {"A", std::size_t(3)}, {"B", std::string("c")} });

    std::string res;
    res = vlt.ToJson(true, 1, false);
)

TEST_BODY(ToStrings, EmptyVault,
    Vault vlt;

    TEST_ASSERT(vlt.ToStrings() == decltype(vlt.ToStrings())());
)

TEST_BODY(ToStrings, KeysWithoutRecords,
    Vault vlt;

    vlt.AddUniqueKey<int>("A");
    vlt.AddKey<std::string>("B", "none");

    TEST_ASSERT(vlt.ToStrings() == decltype(vlt.ToStrings())());
)

TEST_BODY(ToStrings, FilledVault,
    Vault vlt;

    vlt.AddUniqueKey<int>("A");
    vlt.AddKey<std::string>("B", "none");

    vlt.CreateRecord({ {"A", 1}, {"B", std::string("a")} });
    vlt.CreateRecord({ {"A", 2}, {"B", std::string("b")} });
    vlt.CreateRecord({ {"A", 3}, {"B", std::string("c")} });

    // Compare vault use ToStrings
    COMPARE_VAULT(vlt, {
        {{"A", 1}, {"B", std::string("a")}},
        {{"A", 2}, {"B", std::string("b")}},
        {{"A", 3}, {"B", std::string("c")}},
    });
)

TEST_BODY(Print, Empty,
    Vault vlt;

    TEST_COUT(vlt.Print(), R"{{{(Vault does not contain keys!
 (0 records)
){{{");
)

TEST_BODY(Print, KeysWithoutRecords,
    Vault vlt;

    vlt.AddUniqueKey<int>("A");
    vlt.AddKey<std::string>("B", "none");

    TEST_COUT(vlt.Print(), R"{{{(+---+---+
| A | B |
+---+---+
+---+---+
 (0 records)
){{{");
)

TEST_BODY(Print, FilledVaultWithDefaultArgs,
    Vault vlt;

    vlt.AddUniqueKey<int>("A");
    vlt.AddKey<std::string>("B", "none");

    vlt.CreateRecord({ {"A", 1}, {"B", std::string("a")} });
    vlt.CreateRecord({ {"A", 2}, {"B", std::string("b")} });
    vlt.CreateRecord({ {"A", 3}, {"B", std::string("c")} });

    TEST_COUT(vlt.Print(), R"{{{(+---+---+
| A | B |
+---+---+
| 1 | a |
| 2 | b |
| 3 | c |
+---+---+
 (3 records)
){{{");
)

TEST_BODY(Print, NotAllRecords,
    Vault vlt;

    vlt.AddUniqueKey<int>("A");
    vlt.AddKey<std::string>("B", "none");

    vlt.CreateRecord({ {"A", 1}, {"B", std::string("a")} });
    vlt.CreateRecord({ {"A", 2}, {"B", std::string("b")} });
    vlt.CreateRecord({ {"A", 3}, {"B", std::string("c")} });

    TEST_COUT(vlt.Print(false, 2), R"{{{(+---+---+
| A | B |
+---+---+
| 1 | a |
| 2 | b |
$~~~$~~~$
 (3 records)
){{{");
)

TEST_BODY(Print, PrimaryKey,
    Vault vlt;

    vlt.AddUniqueKey<int>("A");
    vlt.AddKey<std::string>("B", "none");

    vlt.CreateRecord({ {"A", 1}, {"B", std::string("c")} });
    vlt.CreateRecord({ {"A", 2}, {"B", std::string("b")} });
    vlt.CreateRecord({ {"A", 3}, {"B", std::string("a")} });

    TEST_COUT(vlt.Print(false, 2, "B"), R"{{{(+---+---+
| A | B |
+---+---+
| 3 | a |
| 2 | b |
$~~~$~~~$
 (3 records)
){{{");
)

TEST_BODY(Print, Reverse,
    Vault vlt;

    vlt.AddUniqueKey<int>("A");
    vlt.AddKey<std::string>("B", "none");

    vlt.CreateRecord({ {"A", 1}, {"B", std::string("a")} });
    vlt.CreateRecord({ {"A", 2}, {"B", std::string("b")} });
    vlt.CreateRecord({ {"A", 3}, {"B", std::string("c")} });

    TEST_COUT(vlt.Print(false, 2, "B", true), R"{{{(+---+---+
| A | B |
+---+---+
| 3 | c |
| 2 | b |
$~~~$~~~$
 (3 records)
){{{");
)

TEST_BODY(Print, NotAllKeys,
    Vault vlt;

    vlt.AddUniqueKey<int>("A");
    vlt.AddKey<std::string>("B", "none");
    vlt.AddKey("C", false);

    vlt.CreateRecord({ {"A", 1}, {"B", std::string("a")}, {"C", true} });
    vlt.CreateRecord({ {"A", 2}, {"B", std::string("b")}, {"C", false} });
    vlt.CreateRecord({ {"A", 3}, {"B", std::string("c")}, {"C", true} });

    TEST_COUT(vlt.Print(false, 2, "B", true, {"C", "B"}), R"{{{(+-------+---+
| C     | B |
+-------+---+
| true  | c |
| false | b |
$~~~~~~~$~~~$
 (3 records)
){{{");
)

TEST_BODY(Print, PrintSet,
    Vault vlt;
    VaultRecordSet vrs;

    vlt.AddKey("A", 0);
    
    vlt.CreateRecord({});

    vlt.RequestEqual("A", 0, vrs);

    vlt.EraseRecord("A", 0);
    vlt.RemoveKey("A");

    TEST_COUT(vrs.Print(), "VaultRecordSet does not contain keys!\n (0 records)\n");
)

TEST_BODY(Print, PrintId,
    Vault vlt;

    vlt.AddUniqueKey<int>("A");
    vlt.AddKey<std::string>("B", "none");

    vlt.CreateRecord({ {"A", 1}, {"B", std::string("a")} });
    vlt.CreateRecord({ {"A", 2}, {"B", std::string("b")} });
    vlt.CreateRecord({ {"A", 3}, {"B", std::string("c")} });

    SUPPRESS_COUT(vlt.Print(true));
)

TEST_BODY(SaveToFile, Empty,
    Vault vlt;
    std::string fileName = GenTmpFileName(std::string("MVault_") + "Vault_" + __FUNCTION__ + "_");
    bool res = false;

    res = vlt.SaveToFile(fileName);

    TEST_ASSERT(res == true);

    COMPARE_FILE(fileName, true, "\r\n");
)

TEST_BODY(SaveToFile, KeysWithoutRecords,
    Vault vlt;

    vlt.AddKey("A", 0);

    std::string fileName = GenTmpFileName(std::string("MVault_") + "Vault_" + __FUNCTION__ + "_");
    bool res = false;

    res = vlt.SaveToFile(fileName);

    TEST_ASSERT(res == true);

    COMPARE_FILE(fileName, true, "A\r\n");
)

TEST_BODY(SaveToFile, FilledVault,
    Vault vlt;

    vlt.AddKey("A", 0);
    vlt.CreateRecord({{"A", 0}});

    std::string fileName = GenTmpFileName(std::string("MVault_") + "Vault_" + __FUNCTION__ + "_");
    bool res = false;

    res = vlt.SaveToFile(fileName);

    TEST_ASSERT(res == true);

    COMPARE_FILE(fileName, true, "A\r\n"
                                 "0\r\n");
)

TEST_BODY(SaveToFile, ReverseNotAllKeys,
    Vault vlt;

    vlt.AddKey("A", 0);
    vlt.AddKey("B", 0);
    vlt.AddUniqueKey<int>("C");

    vlt.CreateRecord({{"A", 0}, {"B", 1}, {"C", 2}});

    std::string fileName = GenTmpFileName(std::string("MVault_") + "Vault_" + __FUNCTION__ + "_");
    bool res = false;

    res = vlt.SaveToFile(fileName, {"C", "A"});

    TEST_ASSERT(res == true);

    COMPARE_FILE(fileName, true, "C,A\r\n"
                                 "2,0\r\n");
)

TEST_BODY(SaveToFile, Separator,
    Vault vlt;

    vlt.AddKey("A", 0);
    vlt.AddKey("B", 0);
    vlt.AddUniqueKey<int>("C");

    vlt.CreateRecord({{"A", 0}, {"B", 1}, {"C", 2}});

    std::string fileName = GenTmpFileName(std::string("MVault_") + "Vault_" + __FUNCTION__ + "_");
    bool res = false;

    res = vlt.SaveToFile(fileName, {}, ";");

    TEST_ASSERT(res == true);

    COMPARE_FILE(fileName, true, "A;B;C\r\n"
                                 "0;1;2\r\n");
)

TEST_BODY(SaveToFile, NotSaveKeys,
    Vault vlt;

    vlt.AddKey("A", 0);
    vlt.AddKey("B", 0);
    vlt.AddUniqueKey<int>("C");

    vlt.CreateRecord({{"A", 0}, {"B", 1}, {"C", 2}});

    std::string fileName = GenTmpFileName(std::string("MVault_") + "Vault_" + __FUNCTION__ + "_");
    bool res = false;

    res = vlt.SaveToFile(fileName, {}, ",", false);

    TEST_ASSERT(res == true);

    COMPARE_FILE(fileName, true, "0,1,2\r\n");
)

TEST_BODY(SaveToFile, FailedToOpenFile,
    Vault vlt;

    vlt.AddKey("A", 0);
    vlt.AddKey("B", 0);
    vlt.AddUniqueKey<int>("C");

    vlt.CreateRecord({{"A", 0}, {"B", 1}, {"C", 2}});

    TEST_ASSERT(vlt.SaveToFile("") == false);
)

TEST_BODY(ReadFile, CorrectCrLf,
    std::string fileName = GenTmpFileName(std::string("MVault_") + "Vault_" + __FUNCTION__ + "_");
    SAVE_FILE("A,B\r\n1,1\r\n2,2");


    Vault vlt;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<int>("B");

    vlt.CreateRecord({{"A", 0}, {"B", 0}});

    TEST_ASSERT(vlt.ReadFile(fileName));

    COMPARE_VAULT(vlt, {
        {{"A", 0}, {"B", 0}},
        {{"A", 1}, {"B", 1}},
        {{"A", 2}, {"B", 2}},
    });
)

TEST_BODY(ReadFile, CorrectLf,
    std::string fileName = GenTmpFileName(std::string("MVault_") + "Vault_" + __FUNCTION__ + "_");
    SAVE_FILE("A,B\n1,1\n2,2");


    Vault vlt;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<int>("B");

    vlt.CreateRecord({{"A", 0}, {"B", 0}});

    TEST_ASSERT(vlt.ReadFile(fileName));

    COMPARE_VAULT(vlt, {
        {{"A", 0}, {"B", 0}},
        {{"A", 1}, {"B", 1}},
        {{"A", 2}, {"B", 2}},
    });
)

TEST_BODY(ReadFile, LoadTypes,
    std::string fileName = GenTmpFileName(std::string("MVault_") + "Vault_" + __FUNCTION__ + "_");
    SAVE_FILE("A,B,C\n1,0.9,true\n-2,-1.2,false");


    Vault vlt;

    vlt.AddKey("A", 0);
    vlt.AddKey("B", 0.0);
    vlt.AddKey("C", true);

    TEST_ASSERT(vlt.ReadFile(fileName));

    COMPARE_VAULT(vlt, {
        {{"A", 1}, {"B", 0.9}, {"C", true}},
        {{"A", -2}, {"B", -1.2}, {"C", false}},
    });
)

TEST_BODY(ReadFile, SemicolonSeparator,
    std::string fileName = GenTmpFileName(std::string("MVault_") + "Vault_" + __FUNCTION__ + "_");
    SAVE_FILE("A;B\r\n1;1\r\n2;2");


    Vault vlt;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<int>("B");

    vlt.CreateRecord({{"A", 0}, {"B", 0}});

    TEST_ASSERT(vlt.ReadFile(fileName, ';'));

    COMPARE_VAULT(vlt, {
        {{"A", 0}, {"B", 0}},
        {{"A", 1}, {"B", 1}},
        {{"A", 2}, {"B", 2}},
    });
)

TEST_BODY(ReadFile, NotLoadKeys,
    std::string fileName = GenTmpFileName(std::string("MVault_") + "Vault_" + __FUNCTION__ + "_");
    SAVE_FILE("1,1\r\n2,2");


    Vault vlt;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<int>("B");

    vlt.CreateRecord({{"A", 0}, {"B", 0}});

    TEST_ASSERT(vlt.ReadFile(fileName, ',', false));

    COMPARE_VAULT(vlt, {
        {{"A", 0}, {"B", 0}},
        {{"A", 1}, {"B", 1}},
        {{"A", 2}, {"B", 2}},
    });
)

TEST_BODY(ReadFile, NotAllKeys,
    std::string fileName = GenTmpFileName(std::string("MVault_") + "Vault_" + __FUNCTION__ + "_");
    SAVE_FILE("1,9\r\n2,8");


    Vault vlt;

    vlt.AddKey("A", 0);
    vlt.AddKey("B", 0);
    vlt.AddUniqueKey<int>("C");
    

    vlt.CreateRecord({{"A", 0}, {"B", 0}, {"C", 0}});

    TEST_ASSERT(vlt.ReadFile(fileName, ',', false, {"C", "A"}));

    COMPARE_VAULT(vlt, {
        {{"A", 0}, {"B", 0}, {"C", 0}},
        {{"A", 9}, {"B", 0}, {"C", 1}},
        {{"A", 8}, {"B", 0}, {"C", 2}},
    });
)

TEST_BODY(ReadFile, HandleRecords,
    std::string fileName = GenTmpFileName(std::string("MVault_") + "Vault_" + __FUNCTION__ + "_");
    SAVE_FILE("A,B\r\n1,1\r\n2,2");


    Vault vlt;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<int>("B");

    vlt.CreateRecord({{"A", 0}, {"B", 0}});

    TEST_ASSERT(vlt.ReadFile(fileName, ',', [&](const std::vector<std::string>& keys, std::vector<std::string>& values) -> void
    {
        for (std::size_t i = 0; i < keys.size(); ++i)
        {
            if (keys[i] == "A") values[i] += "0";
            else values[i] += "00";
        }
    }));

    COMPARE_VAULT(vlt, {
        {{"A", 0}, {"B", 0}},
        {{"A", 10}, {"B", 100}},
        {{"A", 20}, {"B", 200}},
    });
)

TEST_BODY(ReadFile, NotExistedFile,
    Vault vlt;
    TEST_ASSERT(vlt.ReadFile(Uuid()) == false);
)

TEST_BODY(ReadFile, AllIncorrectRecords,
    std::string fileName = GenTmpFileName(std::string("MVault_") + "Vault_" + __FUNCTION__ + "_");
    SAVE_FILE("A,B\r\na,1\r\nb,2");


    Vault vlt;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<int>("B");

    vlt.CreateRecord({{"A", 0}, {"B", 0}});

    TEST_ASSERT(vlt.ReadFile(fileName));

    COMPARE_VAULT(vlt, {{{"A", 0}, {"B", 0}}});
)

TEST_BODY(ReadFile, NotAllIncorrectRecords,
    std::string fileName = GenTmpFileName(std::string("MVault_") + "Vault_" + __FUNCTION__ + "_");
    SAVE_FILE("A,B\r\na,1\r\n2,2");


    Vault vlt;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<int>("B");

    vlt.CreateRecord({{"A", 0}, {"B", 0}});

    TEST_ASSERT(vlt.ReadFile(fileName));

    COMPARE_VAULT(vlt, {{{"A", 0}, {"B", 0}}, {{"A", 2}, {"B", 2}}});
)

TEST_BODY(ReadFile, DuplicateUniqueKeyValue,
    std::string fileName = GenTmpFileName(std::string("MVault_") + "Vault_" + __FUNCTION__ + "_");
    SAVE_FILE("A,B,C\r\n1,1,1\r\n2,2,1");


    Vault vlt;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<int>("B");
    vlt.AddUniqueKey<int>("C");

    vlt.CreateRecord({{"A", 0}, {"B", 0}, {"C", 0}});

    TEST_ASSERT(vlt.ReadFile(fileName));

    COMPARE_VAULT(vlt, {{{"A", 0}, {"B", 0}, {"C", 0}}, {{"A", 1}, {"B", 1}, {"C", 1}}});
)

TEST_BODY(GetErrorsInLastReadedFile, Correct,
    std::string fileName = GenTmpFileName(std::string("MVault_") + "Vault_" + __FUNCTION__ + "_");
    SAVE_FILE("A,B\r\n1,1\r\n2,2");


    Vault vlt;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<int>("B");

    TEST_ASSERT(vlt.ReadFile(fileName));

    std::vector<std::pair<std::size_t, std::string>> res = {};
    TEST_ASSERT(vlt.GetErrorsInLastReadedFile() == res);
)

TEST_BODY(GetErrorsInLastReadedFile, AllIncorrectRecords,
    std::string fileName = GenTmpFileName(std::string("MVault_") + "Vault_" + __FUNCTION__ + "_");
    SAVE_FILE("A,B\r\na,1\r\n2,b");


    Vault vlt;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<int>("B");

    TEST_ASSERT(vlt.ReadFile(fileName));

    std::vector<std::pair<std::size_t, std::string>> res = {{2, "A"}, {3, "B"}};
    TEST_ASSERT(vlt.GetErrorsInLastReadedFile() == res);
)

TEST_BODY(GetErrorsInLastReadedFile, NotAllIncorrectRecords,
    std::string fileName = GenTmpFileName(std::string("MVault_") + "Vault_" + __FUNCTION__ + "_");
    SAVE_FILE("A,B\r\n1,1\r\na,2");


    Vault vlt;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<int>("B");

    TEST_ASSERT(vlt.ReadFile(fileName));

    std::vector<std::pair<std::size_t, std::string>> res = {{3, "A"}};
    TEST_ASSERT(vlt.GetErrorsInLastReadedFile() == res);
)

TEST_BODY(GetErrorsInLastReadedFile, DuplicateUniqueKeyValue,
    std::string fileName = GenTmpFileName(std::string("MVault_") + "Vault_" + __FUNCTION__ + "_");
    SAVE_FILE("A,B\r\n1,1\r\n1,1");


    Vault vlt;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<int>("B");

    TEST_ASSERT(vlt.ReadFile(fileName));

    std::vector<std::pair<std::size_t, std::string>> res = {{2, "B"}};
    TEST_ASSERT(vlt.GetErrorsInLastReadedFile() == res);
)

TEST_BODY(GetErrorsInLastReadedFile, AllIncorrectRecordsWithoutKeys,
    std::string fileName = GenTmpFileName(std::string("MVault_") + "Vault_" + __FUNCTION__ + "_");
    SAVE_FILE("a,1\r\n2,b");


    Vault vlt;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<int>("B");

    TEST_ASSERT(vlt.ReadFile(fileName, ',', false));

    std::vector<std::pair<std::size_t, std::string>> res = {{1, "A"}, {2, "B"}};
    TEST_ASSERT(vlt.GetErrorsInLastReadedFile() == res);
)

TEST_BODY(GetErrorsInLastReadedFile, NotAllIncorrectRecordsWithoutKeys,
    std::string fileName = GenTmpFileName(std::string("MVault_") + "Vault_" + __FUNCTION__ + "_");
    SAVE_FILE("1,1\r\na,2");


    Vault vlt;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<int>("B");

    TEST_ASSERT(vlt.ReadFile(fileName, ',', false));

    std::vector<std::pair<std::size_t, std::string>> res = {{2, "A"}};
    TEST_ASSERT(vlt.GetErrorsInLastReadedFile() == res);
)

TEST_BODY(GetErrorsInLastReadedFile, DuplicateUniqueKeyValueWithoutKeys,
    std::string fileName = GenTmpFileName(std::string("MVault_") + "Vault_" + __FUNCTION__ + "_");
    SAVE_FILE("1,1\r\n1,1");


    Vault vlt;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<int>("B");

    TEST_ASSERT(vlt.ReadFile(fileName, ',', false));

    std::vector<std::pair<std::size_t, std::string>> res = {{2, "B"}};
    TEST_ASSERT(vlt.GetErrorsInLastReadedFile() == res);
)

TEST_BODY(GetErrorsInLastReadedFile, AllIncorrectRecordsNotAllKeys,
    std::string fileName = GenTmpFileName(std::string("MVault_") + "Vault_" + __FUNCTION__ + "_");
    SAVE_FILE("a,1\r\n2,b");


    Vault vlt;

    vlt.AddKey("A", 0);
    vlt.AddKey("B", 0);
    vlt.AddUniqueKey<int>("C");

    TEST_ASSERT(vlt.ReadFile(fileName, ',', false, {"C", "A"}));

    std::vector<std::pair<std::size_t, std::string>> res = {{1, "C"}, {2, "A"}};
    TEST_ASSERT(vlt.GetErrorsInLastReadedFile() == res);
)

TEST_BODY(GetErrorsInLastReadedFile, NotAllIncorrectRecordsNotAllKeys,
    std::string fileName = GenTmpFileName(std::string("MVault_") + "Vault_" + __FUNCTION__ + "_");
    SAVE_FILE("1,1\r\na,2");


    Vault vlt;

    vlt.AddKey("A", 0);
    vlt.AddKey("B", 0);
    vlt.AddUniqueKey<int>("C");

    TEST_ASSERT(vlt.ReadFile(fileName, ',', false, {"C", "A"}));

    std::vector<std::pair<std::size_t, std::string>> res = {{2, "C"}};
    TEST_ASSERT(vlt.GetErrorsInLastReadedFile() == res);
)

TEST_BODY(GetErrorsInLastReadedFile, DuplicateUniqueKeyValueNotAllKeys,
    std::string fileName = GenTmpFileName(std::string("MVault_") + "Vault_" + __FUNCTION__ + "_");
    SAVE_FILE("1,1\r\n1,1");


    Vault vlt;

    vlt.AddKey("A", 0);
    vlt.AddKey("B", 0);
    vlt.AddUniqueKey<int>("C");

    TEST_ASSERT(vlt.ReadFile(fileName, ',', false, {"C", "A"}));

    std::vector<std::pair<std::size_t, std::string>> res = {{2, "C"}};
    TEST_ASSERT(vlt.GetErrorsInLastReadedFile() == res);
)

void VaultUnitTests()
{
    DBG_LOG_ENTER();
    SetBackTraceFormat(BackTraceFormat::None);

    SetDataToRecord::IncorrectDataRecord();
    SetDataToRecord::NotExistedKey();
    SetDataToRecord::IncorrectType();

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
    AddKey::AddKeyWithDependentSets();
    AddKey::IncorrectAddKeyWithDependentSets();

    AddUniqueKey::CorrectAddKeyToEmptyVault();
    AddUniqueKey::IncorrectAddKeyToEmptyVault();
    AddUniqueKey::CorrectAddKeyToNonEmptyVault();
    AddUniqueKey::IncorrectAddKeyToNonEmptyVault();
    AddUniqueKey::CorrectAddKeyToNonEmptyVaultWithDuplicate();
    AddUniqueKey::AddUniqueKeyWithDependentSets();
    AddUniqueKey::IncorrectAddKeyWithDependentSets();
    AddUniqueKey::CorrectAddKeyWithDependentSetsWithDuplicate();

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
    RemoveKey::CorrectRemoveWithDependentSets();
    RemoveKey::IncorrectRemoveFromEmptyVault();
    RemoveKey::IncorrectRemoveFromNonEmptyVault();

    CreateRecord::CorrectParamsWithoutUniqueKey();
    CreateRecord::CorrectParamsWithUniqueKey();
    CreateRecord::DuplicateParams();
    CreateRecord::WrongKeyWithoutUniqueKey();
    CreateRecord::WrongTypeWithoutUniqueKey();
    CreateRecord::WrongKeyWithUniqueKey();
    CreateRecord::WrongTypeWithUniqueKey();
    CreateRecord::DuplicateUniqueKeyValue();
    CreateRecord::EmptyUniqueKeyValue();

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

    RequestInterval::CorrectRequest();
    RequestInterval::CorrectRequestWithoutLeftBoundary();
    RequestInterval::CorrectRequestWithoutRightBoundary();
    RequestInterval::CorrectRequestWithoutBoundaries();
    RequestInterval::CorrectRequestNotAllRecords();
    RequestInterval::CorrectRequestWithPredicat();
    RequestInterval::BoundaryValues();
    RequestInterval::SameBoundaries();
    RequestInterval::IncorrectBoundaries();
    RequestInterval::WrongKey();
    RequestInterval::WrongType();

    Request::CorrectRequest();
    Request::CorrectRequestWithPredicat();
    Request::BoundaryValues();
    Request::WrongKey();
    Request::WrongType();
    Request::ComplexRequestAnd();
    Request::ComplexRequestOr();
    Request::ComplexRequest();
    Request::ComplexRequestWrongKey();
    Request::ComplexRequestWrongType();

    DropVault::Drop();
    DropVault::DropEmpty();
    DropVault::DropSecond();
    DropVault::DropWithDependent();

    DropData::Drop();
    DropData::DropEmpty();
    DropData::DropSecond();
    DropData::DropWithDependent();

    EraseRecord::CorrectEraseByRef();
    EraseRecord::IncorrectEraseByRef();
    EraseRecord::CorrectEraseByKeyAndValue();
    EraseRecord::WrongKeyEraseByKeyAndValue();
    EraseRecord::WrongTypeEraseByKeyAndValue();
    EraseRecord::WrongValueEraseByKeyAndValue();

    EraseRecords::Erase();
    EraseRecords::WrongKeyErase();
    EraseRecords::WrongTypeErase();
    EraseRecords::WrongValueErase();

    Size::Default();

    GetSortedRecords::GetRecords();
    GetSortedRecords::GetRecordsReverse();
    GetSortedRecords::GetNotAllRecords();
    GetSortedRecords::WrongKey();

    SortBy::Sort();
    SortBy::SortReverse();
    SortBy::SortNotAllRecords();
    SortBy::WrongKey();

    ToJson::Default();
    ToJson::Format();
    ToJson::DiffTabSize();
    ToJson::RecordTemplate();
    ToJson::Array();
    ToJson::ArrayFormat();
    ToJson::ArrayDiffTabSize();
    ToJson::RecordId();

    ToStrings::EmptyVault();
    ToStrings::KeysWithoutRecords();
    ToStrings::FilledVault();

    Print::Empty();
    Print::KeysWithoutRecords();
    Print::FilledVaultWithDefaultArgs();
    Print::NotAllRecords();
    Print::PrimaryKey();
    Print::Reverse();
    Print::NotAllKeys();
    Print::PrintSet();
    Print::PrintId();

    SaveToFile::Empty();
    SaveToFile::KeysWithoutRecords();
    SaveToFile::FilledVault();
    SaveToFile::ReverseNotAllKeys();
    SaveToFile::Separator();
    SaveToFile::NotSaveKeys();
    SaveToFile::FailedToOpenFile();

    ReadFile::CorrectCrLf();
    ReadFile::CorrectLf();
    ReadFile::LoadTypes();
    ReadFile::SemicolonSeparator();
    ReadFile::NotLoadKeys();
    ReadFile::NotAllKeys();
    ReadFile::HandleRecords();
    ReadFile::NotExistedFile();
    ReadFile::AllIncorrectRecords();
    ReadFile::NotAllIncorrectRecords();
    ReadFile::DuplicateUniqueKeyValue();

    GetErrorsInLastReadedFile::Correct();
    GetErrorsInLastReadedFile::AllIncorrectRecords();
    GetErrorsInLastReadedFile::NotAllIncorrectRecords();
    GetErrorsInLastReadedFile::DuplicateUniqueKeyValue();
    GetErrorsInLastReadedFile::AllIncorrectRecordsWithoutKeys();
    GetErrorsInLastReadedFile::NotAllIncorrectRecordsWithoutKeys();
    GetErrorsInLastReadedFile::DuplicateUniqueKeyValueWithoutKeys();
    GetErrorsInLastReadedFile::AllIncorrectRecordsNotAllKeys();
    GetErrorsInLastReadedFile::NotAllIncorrectRecordsNotAllKeys();
    GetErrorsInLastReadedFile::DuplicateUniqueKeyValueNotAllKeys();
}