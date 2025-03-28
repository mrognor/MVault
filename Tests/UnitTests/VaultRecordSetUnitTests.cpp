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

TEST_BODY(GetIsParentVaultValid, Invalid,
    VaultRecordSet vrs;

    TEST_ASSERT(vrs.GetIsParentVaultValid() == false);
)

TEST_BODY(GetIsParentVaultValid, Valid,
    Vault vlt;
    GENERATE_SET(vrs);

    TEST_ASSERT(vrs.GetIsParentVaultValid() == true);
)

TEST_BODY(GetIsParentVaultValid, Invalidate,
    Vault vlt;
    GENERATE_SET(vrs);
    vlt.DropVault();

    TEST_ASSERT(vrs.GetIsParentVaultValid() == false);
)

TEST_BODY(GetParentVaultUniqueId, Invalid,
    VaultRecordSet vrs;

    TEST_ASSERT(vrs.GetParentVaultUniqueId() == "null");
)

TEST_BODY(GetParentVaultUniqueId, Valid,
    Vault vlt;
    GENERATE_SET(vrs);

    TEST_ASSERT(vrs.GetParentVaultUniqueId() != "null");
)

TEST_BODY(GetParentVaultUniqueId, Invalidate,
    Vault vlt;
    GENERATE_SET(vrs);
    vlt.DropVault();

    TEST_ASSERT(vrs.GetParentVaultUniqueId() == "null");
)

TEST_BODY(IsKeyExist, Invalid,
    VaultRecordSet vrs;
    TEST_ASSERT(vrs.IsKeyExist("A") == false);
)

TEST_BODY(IsKeyExist, ExistingKey,
    Vault vlt;
    GENERATE_SET(vrs);

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    TEST_ASSERT(vrs.IsKeyExist("A") == true);
    TEST_ASSERT(vrs.IsKeyExist("B") == true);
)

TEST_BODY(IsKeyExist, NonExistingKey,
    Vault vlt;
    GENERATE_SET(vrs);

    TEST_ASSERT(vrs.IsKeyExist("A") == false);
)

TEST_BODY(GetKeyValue, Invalid,
    VaultRecordSet vrs;
    VaultOperationResult vor;
    int a = 0;

    vor = vrs.GetKeyValue("A", a);
    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::ParentVaultNotValid, SavedType == typeid(void));
)

TEST_BODY(GetKeyValue, ExistingKey,
    Vault vlt;
    GENERATE_SET(vrs);
    VaultOperationResult vor;
    int a = -1;

    vlt.AddKey("A", 0);

    vor = vrs.GetKeyValue("A", a);

    TEST_ASSERT(a == 0);
    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));
)

TEST_BODY(GetKeyValue, NonExistingKey,
    Vault vlt;
    GENERATE_SET(vrs);
    VaultOperationResult vor;
    int a = -1;

    vor = vrs.GetKeyValue("A", a);

    TEST_ASSERT(a == -1);
    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongKey, SavedType == typeid(void));
)

TEST_BODY(GetKeyValue, WrongType,
    Vault vlt;
    GENERATE_SET(vrs);
    VaultOperationResult vor;
    std::string a;

    vlt.AddKey("A", 0);

    vor = vrs.GetKeyValue("A", a);

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(std::string), ResultCode == VaultOperationResultCode::WrongType, SavedType == typeid(int));
)

TEST_BODY(GetKeyValue, UniqueKey,
    Vault vlt;
    GENERATE_SET(vrs);
    VaultOperationResult vor;
    int a = -1;

    vlt.AddUniqueKey<int>("A");

    vor = vrs.GetKeyValue("A", a);

    TEST_ASSERT(a == -1);
    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::UniqueKey, SavedType == typeid(int));
)

TEST_BODY(GetKeyType, Invalid,
    VaultRecordSet vrs;

    std::type_index type = typeid(void);

    TEST_ASSERT(vrs.GetKeyType("A", type) == false);
    TEST_ASSERT(type == typeid(void));
)

TEST_BODY(GetKeyType, ExistingKey,
    Vault vlt;
    GENERATE_SET(vrs);
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    std::type_index type = typeid(void);

    TEST_ASSERT(vrs.GetKeyType("A", type));
    TEST_ASSERT(type == typeid(int));

    TEST_ASSERT(vrs.GetKeyType("B", type));
    TEST_ASSERT(type == typeid(std::string));
)

TEST_BODY(GetKeyType, NonExistingKey,
    Vault vlt;
    GENERATE_SET(vrs);

    std::type_index type = typeid(void);

    TEST_ASSERT(vrs.GetKeyType("A", type) == false);
    TEST_ASSERT(type == typeid(void));
)

TEST_BODY(AddRecord, Invalid,
    Vault vlt;
    GENERATE_SET(vrs);
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.CreateRecord(vrr, {});
    vlt.DropVault();

    vor = vrs.AddRecord(vrr);

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "", 
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::ParentVaultNotValid, SavedType == typeid(void));
    
    TEST_ASSERT(vrs.Size() == 0);
    COMPARE_VAULT(vrs, {});
)

TEST_BODY(AddRecord, Correct,
    Vault vlt;
    GENERATE_SET(vrs);
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.CreateRecord(vrr, {});

    vor = vrs.AddRecord(vrr);

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "", 
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(void));
    
    TEST_ASSERT(vrs.Size() == 1);

    COMPARE_VAULT(vrs, {
        {{"A", 0}}
    });
)

TEST_BODY(AddRecord, InvalidRef,
    Vault vlt;
    GENERATE_SET(vrs);
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.CreateRecord({});

    vor = vrs.AddRecord(vrr);

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "", 
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::DataRecordNotValid, SavedType == typeid(void));
    
    TEST_ASSERT(vrs.Size() == 0);
    COMPARE_VAULT(vrs, {});
)

TEST_BODY(AddRecord, OtherVaultParent,
    Vault vlt1, vlt2;
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt1.AddKey("A", 0);
    vlt1.CreateRecord({});
    vlt1.GetRecord("A", 0, vrr);

    vlt2.AddKey("A", 0);
    vlt2.CreateRecord({});
    vlt2.RequestEqual("A", 0, vrs);
    vlt2.EraseRecord("A", 0);

    vor = vrs.AddRecord(vrr);

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "", 
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::ParentVaultNotMatch, SavedType == typeid(void));
    
    TEST_ASSERT(vrs.Size() == 0);
    COMPARE_VAULT(vrs, {});
)

TEST_BODY(GetRecord, Invalid,
    VaultRecordSet vrs;
    VaultOperationResult vor;
    VaultRecordRef vrr;

    vor = vrs.GetRecord("A", 0, vrr);

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::ParentVaultNotValid, SavedType == typeid(void));
)

TEST_BODY(GetRecord, CorrectGetRecord,
    Vault vlt;
    GENERATE_SET(vrs);
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord(vrr, {{"A", 1}, {"B", std::string("a")}});
    vrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("b")}});
    vrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 3}, {"B", std::string("c")}});
    vrs.AddRecord(vrr);

    vor = vrs.GetRecord("A", 1, vrr);
    
    COMPARE_REF(vrr, {{{"A", 1}, {"B", std::string("a")}}});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));


    vor = vrs.GetRecord<std::string>("B", "c", vrr);
    
    COMPARE_REF(vrr, {{{"A", 3}, {"B", std::string("c")}}});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "B", 
        RequestedType == typeid(std::string), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(std::string));
)

TEST_BODY(GetRecord, WrongKey,
    Vault vlt;
    GENERATE_SET(vrs);
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord(vrr, {{"A", 1}, {"B", std::string("a")}});
    vrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("b")}});
    vrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 3}, {"B", std::string("c")}});
    vrs.AddRecord(vrr);

    vor = vrs.GetRecord("Z", 1, vrr);

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "Z", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongKey, SavedType == typeid(void));
)

TEST_BODY(GetRecord, WrongType,
    Vault vlt;
    GENERATE_SET(vrs);
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord(vrr, {{"A", 1}, {"B", std::string("a")}});
    vrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("b")}});
    vrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 3}, {"B", std::string("c")}});
    vrs.AddRecord(vrr);

    vor = vrs.GetRecord("B", 1, vrr);

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "B", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongType, SavedType == typeid(std::string));
)

TEST_BODY(GetRecord, WrongValue,
    Vault vlt;
    GENERATE_SET(vrs);
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord(vrr, {{"A", 1}, {"B", std::string("a")}});
    vrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("b")}});
    vrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 3}, {"B", std::string("c")}});
    vrs.AddRecord(vrr);

    vor = vrs.GetRecord("A", 99, vrr);

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongValue, SavedType == typeid(int));
)

TEST_BODY(GetRecords, Invalid,
    VaultRecordSet vrs;
    VaultOperationResult vor;
    std::vector<VaultRecordRef> refs;

    vor = vrs.GetRecords("A", 0, refs);

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::ParentVaultNotValid, SavedType == typeid(void));
)

TEST_BODY(GetRecords, CorrectGetRecords,
    Vault vlt;
    GENERATE_SET(vrs);
    std::vector<VaultRecordRef> records;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord(vrr, {{"A", 1}, {"B", std::string("a")}});
    vrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("b")}});
    vrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bb")}});
    vrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bbb")}});
    vrs.AddRecord(vrr);

    vor = vrs.GetRecords("A", 1, records);
    
    TEST_ASSERT(records.size() == 1);
    COMPARE_REF(records[0], {{{"A", 1}, {"B", std::string("a")}}});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));


    vor = vrs.GetRecords("A", 2, records);
    
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
    GENERATE_SET(vrs);
    std::vector<VaultRecordRef> records;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord(vrr, {{"A", 1}, {"B", std::string("a")}});
    vrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("b")}});
    vrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bb")}});
    vrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bbb")}});
    vrs.AddRecord(vrr);

    vor = vrs.GetRecords("A", 1, records, 2);
    
    TEST_ASSERT(records.size() == 1);
    COMPARE_REF(records[0], {{{"A", 1}, {"B", std::string("a")}}});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
    RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));


    vor = vrs.GetRecords("A", 2, records, 2);

    TEST_ASSERT(records.size() == 2);

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));
)

TEST_BODY(GetRecords, WrongKey,
    Vault vlt;
    GENERATE_SET(vrs);
    std::vector<VaultRecordRef> records;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord(vrr, {{"A", 1}, {"B", std::string("a")}});
    vrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("b")}});
    vrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bb")}});
    vrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bbb")}});
    vrs.AddRecord(vrr);

    vor = vrs.GetRecords("Z", 1, records);
    
    TEST_ASSERT(records.size() == 0);

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "Z", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongKey, SavedType == typeid(void));
)

TEST_BODY(GetRecords, WrongType,
    Vault vlt;
    GENERATE_SET(vrs);
    std::vector<VaultRecordRef> records;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord(vrr, {{"A", 1}, {"B", std::string("a")}});
    vrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("b")}});
    vrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bb")}});
    vrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bbb")}});
    vrs.AddRecord(vrr);

    vor = vrs.GetRecords("B", 1, records);
    
    TEST_ASSERT(records.size() == 0);

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "B", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongType, SavedType == typeid(std::string));
)

TEST_BODY(GetRecords, WrongValue,
    Vault vlt;
    GENERATE_SET(vrs);
    std::vector<VaultRecordRef> records;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord(vrr, {{"A", 1}, {"B", std::string("a")}});
    vrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("b")}});
    vrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bb")}});
    vrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bbb")}});
    vrs.AddRecord(vrr);

    vor = vrs.GetRecords("A", 99, records);
    
    TEST_ASSERT(records.size() == 0);

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongValue, SavedType == typeid(int));
)

TEST_BODY(RequestEqual, Invalid,
    VaultRecordSet parentVrs;
    VaultRecordSet vrs;
    VaultOperationResult vor;

    vor = parentVrs.RequestEqual("A", 0, vrs);

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::ParentVaultNotValid, SavedType == typeid(void));
)

TEST_BODY(RequestEqual, CorrectRequest,
    Vault vlt;
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord(vrr, {{"A", 1}, {"B", std::string("a")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("b")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bbb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 3}, {"B", std::string("c")}});
    parentVrs.AddRecord(vrr);

    vor = parentVrs.RequestEqual("A", 1, vrs);

    TEST_ASSERT(vrs.Size() == 1);

    COMPARE_VAULT(vrs, {{{"A", 1}, {"B", std::string("a")}}});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));


    vor = parentVrs.RequestEqual("A", 2, vrs);

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
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord(vrr, {{"A", 1}, {"B", std::string("a")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("b")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bbb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 3}, {"B", std::string("c")}});
    parentVrs.AddRecord(vrr);

    vor = parentVrs.RequestEqual("A", 1, vrs, 2);

    TEST_ASSERT(vrs.Size() == 1);

    COMPARE_VAULT(vrs, {{{"A", 1}, {"B", std::string("a")}}});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));


    vor = parentVrs.RequestEqual("A", 2, vrs, 2);

    TEST_ASSERT(vrs.Size() == 2);

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));
)

TEST_BODY(RequestEqual, CorrectRequestWithPredicat,
    Vault vlt;
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord(vrr, {{"A", 1}, {"B", std::string("a")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("b")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bbb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 3}, {"B", std::string("c")}});
    parentVrs.AddRecord(vrr);

    vor = parentVrs.RequestEqual("A", 2, vrs, -1, [](const VaultRecordRef& ref) -> bool
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
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord(vrr, {{"A", 1}, {"B", std::string("a")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("b")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bbb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 3}, {"B", std::string("c")}});
    parentVrs.AddRecord(vrr);

    vor = parentVrs.RequestEqual("Z", 1, vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "Z", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongKey, SavedType == typeid(void));
)

TEST_BODY(RequestEqual, WrongType,
    Vault vlt;
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord(vrr, {{"A", 1}, {"B", std::string("a")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("b")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bbb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 3}, {"B", std::string("c")}});
    parentVrs.AddRecord(vrr);

    vor = parentVrs.RequestEqual("B", 1, vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "B", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongType, SavedType == typeid(std::string));
)

TEST_BODY(RequestEqual, ValueNotInVault,
    Vault vlt;
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord(vrr, {{"A", 1}, {"B", std::string("a")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("b")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bbb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 3}, {"B", std::string("c")}});
    parentVrs.AddRecord(vrr);

    vor = parentVrs.RequestEqual("A", 4, vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));
)

TEST_BODY(RequestGreater, Invalid,
    VaultRecordSet parentVrs;
    VaultRecordSet vrs;
    VaultOperationResult vor;

    vor = parentVrs.RequestGreater("A", 0, vrs);

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::ParentVaultNotValid, SavedType == typeid(void));
)

TEST_BODY(RequestGreater, CorrectRequest,
    Vault vlt;
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord(vrr, {{"A", 1}, {"B", std::string("a")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("b")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bbb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 3}, {"B", std::string("c")}});
    parentVrs.AddRecord(vrr);

    vor = parentVrs.RequestGreater("A", 1, vrs);

    TEST_ASSERT(vrs.Size() == 4);

    COMPARE_VAULT(vrs, {
        {{"A", 2}, {"B", std::string("b")}},
        {{"A", 2}, {"B", std::string("bb")}},
        {{"A", 2}, {"B", std::string("bbb")}},
        {{"A", 3}, {"B", std::string("c")}},
    });

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));


    vor = parentVrs.RequestGreater("A", 2, vrs);

    TEST_ASSERT(vrs.Size() == 1);

    COMPARE_VAULT(vrs, {{{"A", 3}, {"B", std::string("c")}}});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));
)

TEST_BODY(RequestGreater, CorrectRequestNotAllRecords,
    Vault vlt;
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord(vrr, {{"A", 1}, {"B", std::string("a")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("b")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bbb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 3}, {"B", std::string("c")}});
    parentVrs.AddRecord(vrr);

    vor = parentVrs.RequestGreater("A", 1, vrs, 2);

    TEST_ASSERT(vrs.Size() == 2);

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));


    vor = parentVrs.RequestGreater("A", 2, vrs, 4);

    TEST_ASSERT(vrs.Size() == 1);

    COMPARE_VAULT(vrs, {{{"A", 3}, {"B", std::string("c")}}});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));
)

TEST_BODY(RequestGreater, CorrectRequestWithPredicat,
    Vault vlt;
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord(vrr, {{"A", 1}, {"B", std::string("a")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("b")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bbb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 3}, {"B", std::string("c")}});
    parentVrs.AddRecord(vrr);

    vor = parentVrs.RequestGreater("A", 0, vrs, -1, [](const VaultRecordRef& ref) -> bool
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
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;

    vlt.AddKey("A", 0);

    // ...1...3...5...
    vlt.CreateRecord(vrr, {{"A", 1}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 3}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 5}});
    parentVrs.AddRecord(vrr);

    //  x
    // ...1...3...5...
    parentVrs.RequestGreater("A", 0, vrs);
    COMPARE_VAULT(vrs, {{{"A", 1}}, {{"A", 3}}, {{"A", 5}}});

    //    x
    // ...1...3...5...
    parentVrs.RequestGreater("A", 1, vrs);
    COMPARE_VAULT(vrs, {{{"A", 3}}, {{"A", 5}}});

    //      x
    // ...1...3...5...
    parentVrs.RequestGreater("A", 2, vrs);
    COMPARE_VAULT(vrs, {{{"A", 3}}, {{"A", 5}}});

    //        x
    // ...1...3...5...
    parentVrs.RequestGreater("A", 3, vrs);
    COMPARE_VAULT(vrs, {{{"A", 5}}});

    //          x
    // ...1...3...5...
    parentVrs.RequestGreater("A", 4, vrs);
    COMPARE_VAULT(vrs, {{{"A", 5}}});

    //            x
    // ...1...3...5...
    parentVrs.RequestGreater("A", 5, vrs);
    COMPARE_VAULT(vrs, {});

    //              x
    // ...1...3...5...
    parentVrs.RequestGreater("A", 6, vrs);
    COMPARE_VAULT(vrs, {});
)

TEST_BODY(RequestGreater, WrongKey,
    Vault vlt;
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord(vrr, {{"A", 1}, {"B", std::string("a")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("b")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bbb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 3}, {"B", std::string("c")}});
    parentVrs.AddRecord(vrr);

    vor = parentVrs.RequestGreater("Z", 1, vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "Z", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongKey, SavedType == typeid(void));
)

TEST_BODY(RequestGreater, WrongType,
    Vault vlt;
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord(vrr, {{"A", 1}, {"B", std::string("a")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("b")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bbb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 3}, {"B", std::string("c")}});
    parentVrs.AddRecord(vrr);

    vor = parentVrs.RequestGreater("B", 1, vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "B", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongType, SavedType == typeid(std::string));
)

TEST_BODY(RequestGreater, ValueNotInVault,
    Vault vlt;
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord(vrr, {{"A", 1}, {"B", std::string("a")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("b")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bbb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 3}, {"B", std::string("c")}});
    parentVrs.AddRecord(vrr);

    vor = parentVrs.RequestGreater("A", 4, vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));
)

TEST_BODY(RequestGreaterOrEqual, Invalid,
    VaultRecordSet parentVrs;
    VaultRecordSet vrs;
    VaultOperationResult vor;

    vor = parentVrs.RequestGreaterOrEqual("A", 0, vrs);

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::ParentVaultNotValid, SavedType == typeid(void));
)

TEST_BODY(RequestGreaterOrEqual, CorrectRequest,
    Vault vlt;
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord(vrr, {{"A", 1}, {"B", std::string("a")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("b")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bbb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 3}, {"B", std::string("c")}});
    parentVrs.AddRecord(vrr);

    vor = parentVrs.RequestGreaterOrEqual("A", 1, vrs);

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


    vor = parentVrs.RequestGreaterOrEqual("A", 3, vrs);

    TEST_ASSERT(vrs.Size() == 1);

    COMPARE_VAULT(vrs, {{{"A", 3}, {"B", std::string("c")}}});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));
)

TEST_BODY(RequestGreaterOrEqual, CorrectRequestNotAllRecords,
    Vault vlt;
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord(vrr, {{"A", 1}, {"B", std::string("a")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("b")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bbb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 3}, {"B", std::string("c")}});
    parentVrs.AddRecord(vrr);

    vor = parentVrs.RequestGreaterOrEqual("A", 2, vrs, 2);

    TEST_ASSERT(vrs.Size() == 2);

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));


    vor = parentVrs.RequestGreaterOrEqual("A", 3, vrs, 4);

    TEST_ASSERT(vrs.Size() == 1);

    COMPARE_VAULT(vrs, {{{"A", 3}, {"B", std::string("c")}}});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));
)

TEST_BODY(RequestGreaterOrEqual, CorrectRequestWithPredicat,
    Vault vlt;
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord(vrr, {{"A", 1}, {"B", std::string("a")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("b")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bbb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 3}, {"B", std::string("c")}});
    parentVrs.AddRecord(vrr);

    vor = parentVrs.RequestGreaterOrEqual("A", 2, vrs, -1, [](const VaultRecordRef& ref) -> bool
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
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;

    vlt.AddKey("A", 0);

    // ...1...3...5...
    vlt.CreateRecord(vrr, {{"A", 1}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 3}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 5}});
    parentVrs.AddRecord(vrr);

    //  x
    // ...1...3...5...
    parentVrs.RequestGreaterOrEqual("A", 0, vrs);
    COMPARE_VAULT(vrs, {{{"A", 1}}, {{"A", 3}}, {{"A", 5}}});

    //    x
    // ...1...3...5...
    parentVrs.RequestGreaterOrEqual("A", 1, vrs);
    COMPARE_VAULT(vrs, {{{"A", 1}}, {{"A", 3}}, {{"A", 5}}});

    //      x
    // ...1...3...5...
    parentVrs.RequestGreaterOrEqual("A", 2, vrs);
    COMPARE_VAULT(vrs, {{{"A", 3}}, {{"A", 5}}});

    //        x
    // ...1...3...5...
    parentVrs.RequestGreaterOrEqual("A", 3, vrs);
    COMPARE_VAULT(vrs, {{{"A", 3}}, {{"A", 5}}});

    //          x
    // ...1...3...5...
    parentVrs.RequestGreaterOrEqual("A", 4, vrs);
    COMPARE_VAULT(vrs, {{{"A", 5}}});

    //            x
    // ...1...3...5...
    parentVrs.RequestGreaterOrEqual("A", 5, vrs);
    COMPARE_VAULT(vrs, {{{"A", 5}}});

    //              x
    // ...1...3...5...
    parentVrs.RequestGreaterOrEqual("A", 6, vrs);
    COMPARE_VAULT(vrs, {});
)

TEST_BODY(RequestGreaterOrEqual, WrongKey,
    Vault vlt;
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord(vrr, {{"A", 1}, {"B", std::string("a")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("b")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bbb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 3}, {"B", std::string("c")}});
    parentVrs.AddRecord(vrr);

    vor = parentVrs.RequestGreaterOrEqual("Z", 1, vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "Z", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongKey, SavedType == typeid(void));
)

TEST_BODY(RequestGreaterOrEqual, WrongType,
    Vault vlt;
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord(vrr, {{"A", 1}, {"B", std::string("a")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("b")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bbb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 3}, {"B", std::string("c")}});
    parentVrs.AddRecord(vrr);

    vor = parentVrs.RequestGreaterOrEqual("B", 1, vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "B", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongType, SavedType == typeid(std::string));
)

TEST_BODY(RequestGreaterOrEqual, ValueNotInVault,
    Vault vlt;
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord(vrr, {{"A", 1}, {"B", std::string("a")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("b")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bbb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 3}, {"B", std::string("c")}});
    parentVrs.AddRecord(vrr);

    vor = parentVrs.RequestGreaterOrEqual("A", 4, vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));
)

TEST_BODY(RequestLess, Invalid,
    VaultRecordSet parentVrs;
    VaultRecordSet vrs;
    VaultOperationResult vor;

    vor = parentVrs.RequestLess("A", 0, vrs);

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::ParentVaultNotValid, SavedType == typeid(void));
)

TEST_BODY(RequestLess, CorrectRequest,
    Vault vlt;
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord(vrr, {{"A", 1}, {"B", std::string("a")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("b")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bbb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 3}, {"B", std::string("c")}});
    parentVrs.AddRecord(vrr);

    vor = parentVrs.RequestLess("A", 3, vrs);

    TEST_ASSERT(vrs.Size() == 4);

    COMPARE_VAULT(vrs, {
        {{"A", 1}, {"B", std::string("a")}},
        {{"A", 2}, {"B", std::string("b")}},
        {{"A", 2}, {"B", std::string("bb")}},
        {{"A", 2}, {"B", std::string("bbb")}},
    });

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));

    vor = parentVrs.RequestLess("A", 2, vrs);

    TEST_ASSERT(vrs.Size() == 1);

    COMPARE_VAULT(vrs, {{{"A", 1}, {"B", std::string("a")}}});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));
)

TEST_BODY(RequestLess, CorrectRequestNotAllRecords,
    Vault vlt;
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord(vrr, {{"A", 1}, {"B", std::string("a")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("b")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bbb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 3}, {"B", std::string("c")}});
    parentVrs.AddRecord(vrr);

    vor = parentVrs.RequestLess("A", 3, vrs, 2);

    TEST_ASSERT(vrs.Size() == 2);

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));


    vor = parentVrs.RequestLess("A", 2, vrs, 4);

    TEST_ASSERT(vrs.Size() == 1);

    COMPARE_VAULT(vrs, {{{"A", 1}, {"B", std::string("a")}}});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));
)

TEST_BODY(RequestLess, CorrectRequestWithPredicat,
    Vault vlt;
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord(vrr, {{"A", 1}, {"B", std::string("a")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("b")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bbb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 3}, {"B", std::string("c")}});
    parentVrs.AddRecord(vrr);

    vor = parentVrs.RequestLess("A", 3, vrs, -1, [](const VaultRecordRef& ref) -> bool
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
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;

    vlt.AddKey("A", 0);

    // ...1...3...5...
    vlt.CreateRecord(vrr, {{"A", 1}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 3}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 5}});
    parentVrs.AddRecord(vrr);

    //  x
    // ...1...3...5...
    parentVrs.RequestLess("A", 0, vrs);
    COMPARE_VAULT(vrs, {});

    //    x
    // ...1...3...5...
    parentVrs.RequestLess("A", 1, vrs);
    COMPARE_VAULT(vrs, {});

    //      x
    // ...1...3...5...
    parentVrs.RequestLess("A", 2, vrs);
    COMPARE_VAULT(vrs, {{{"A", 1}}});

    //        x
    // ...1...3...5...
    parentVrs.RequestLess("A", 3, vrs);
    COMPARE_VAULT(vrs, {{{"A", 1}}});

    //          x
    // ...1...3...5...
    parentVrs.RequestLess("A", 4, vrs);
    COMPARE_VAULT(vrs, {{{"A", 1}}, {{"A", 3}}});

    //            x
    // ...1...3...5...
    parentVrs.RequestLess("A", 5, vrs);
    COMPARE_VAULT(vrs, {{{"A", 1}}, {{"A", 3}}});

    //              x
    // ...1...3...5...
    parentVrs.RequestLess("A", 6, vrs);
    COMPARE_VAULT(vrs, {{{"A", 1}}, {{"A", 3}}, {{"A", 5}}});
)

TEST_BODY(RequestLess, WrongKey,
    Vault vlt;
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord(vrr, {{"A", 1}, {"B", std::string("a")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("b")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bbb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 3}, {"B", std::string("c")}});
    parentVrs.AddRecord(vrr);

    vor = parentVrs.RequestLess("Z", 1, vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "Z", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongKey, SavedType == typeid(void));
)

TEST_BODY(RequestLess, WrongType,
    Vault vlt;
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord(vrr, {{"A", 1}, {"B", std::string("a")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("b")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bbb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 3}, {"B", std::string("c")}});
    parentVrs.AddRecord(vrr);

    vor = parentVrs.RequestLess("B", 1, vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "B", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongType, SavedType == typeid(std::string));
)

TEST_BODY(RequestLess, ValueNotInVault,
    Vault vlt;
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord(vrr, {{"A", 1}, {"B", std::string("a")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("b")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bbb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 3}, {"B", std::string("c")}});
    parentVrs.AddRecord(vrr);

    vor = parentVrs.RequestLess("A", 1, vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));
)

TEST_BODY(RequestLessOrEqual, Invalid,
    VaultRecordSet parentVrs;
    VaultRecordSet vrs;
    VaultOperationResult vor;

    vor = parentVrs.RequestLessOrEqual("A", 0, vrs);

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::ParentVaultNotValid, SavedType == typeid(void));
)

TEST_BODY(RequestLessOrEqual, CorrectRequest,
    Vault vlt;
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord(vrr, {{"A", 1}, {"B", std::string("a")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("b")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bbb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 3}, {"B", std::string("c")}});
    parentVrs.AddRecord(vrr);

    vor = parentVrs.RequestLessOrEqual("A", 2, vrs);

    TEST_ASSERT(vrs.Size() == 4);

    COMPARE_VAULT(vrs, {
        {{"A", 1}, {"B", std::string("a")}},
        {{"A", 2}, {"B", std::string("b")}},
        {{"A", 2}, {"B", std::string("bb")}},
        {{"A", 2}, {"B", std::string("bbb")}},
    });

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));

    vor = parentVrs.RequestLessOrEqual("A", 1, vrs);

    TEST_ASSERT(vrs.Size() == 1);

    COMPARE_VAULT(vrs, {{{"A", 1}, {"B", std::string("a")}}});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));
)

TEST_BODY(RequestLessOrEqual, CorrectRequestNotAllRecords,
    Vault vlt;
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord(vrr, {{"A", 1}, {"B", std::string("a")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("b")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bbb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 3}, {"B", std::string("c")}});
    parentVrs.AddRecord(vrr);

    vor = parentVrs.RequestLessOrEqual("A", 3, vrs, 2);

    TEST_ASSERT(vrs.Size() == 2);

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));


    vor = parentVrs.RequestLessOrEqual("A", 1, vrs, 4);

    TEST_ASSERT(vrs.Size() == 1);

    COMPARE_VAULT(vrs, {{{"A", 1}, {"B", std::string("a")}}});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));
)

TEST_BODY(RequestLessOrEqual, CorrectRequestWithPredicat,
    Vault vlt;
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord(vrr, {{"A", 1}, {"B", std::string("a")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("b")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bbb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 3}, {"B", std::string("c")}});
    parentVrs.AddRecord(vrr);

    vor = parentVrs.RequestLessOrEqual("A", 2, vrs, -1, [](const VaultRecordRef& ref) -> bool
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
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;

    vlt.AddKey("A", 0);

    // ...1...3...5...
    vlt.CreateRecord(vrr, {{"A", 1}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 3}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 5}});
    parentVrs.AddRecord(vrr);

    //  x
    // ...1...3...5...
    parentVrs.RequestLessOrEqual("A", 0, vrs);
    COMPARE_VAULT(vrs, {});

    //    x
    // ...1...3...5...
    parentVrs.RequestLessOrEqual("A", 1, vrs);
    COMPARE_VAULT(vrs, {{{"A", 1}}});

    //      x
    // ...1...3...5...
    parentVrs.RequestLessOrEqual("A", 2, vrs);
    COMPARE_VAULT(vrs, {{{"A", 1}}});

    //        x
    // ...1...3...5...
    parentVrs.RequestLessOrEqual("A", 3, vrs);
    COMPARE_VAULT(vrs, {{{"A", 1}}, {{"A", 3}}});

    //          x
    // ...1...3...5...
    parentVrs.RequestLessOrEqual("A", 4, vrs);
    COMPARE_VAULT(vrs, {{{"A", 1}}, {{"A", 3}}});

    //            x
    // ...1...3...5...
    parentVrs.RequestLessOrEqual("A", 5, vrs);
    COMPARE_VAULT(vrs, {{{"A", 1}}, {{"A", 3}}, {{"A", 5}}});

    //              x
    // ...1...3...5...
    parentVrs.RequestLessOrEqual("A", 6, vrs);
    COMPARE_VAULT(vrs, {{{"A", 1}}, {{"A", 3}}, {{"A", 5}}});
)

TEST_BODY(RequestLessOrEqual, WrongKey,
    Vault vlt;
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord(vrr, {{"A", 1}, {"B", std::string("a")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("b")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bbb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 3}, {"B", std::string("c")}});
    parentVrs.AddRecord(vrr);

    vor = parentVrs.RequestLessOrEqual("Z", 1, vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "Z", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongKey, SavedType == typeid(void));
)

TEST_BODY(RequestLessOrEqual, WrongType,
    Vault vlt;
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord(vrr, {{"A", 1}, {"B", std::string("a")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("b")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bbb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 3}, {"B", std::string("c")}});
    parentVrs.AddRecord(vrr);

    vor = parentVrs.RequestLessOrEqual("B", 1, vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "B", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongType, SavedType == typeid(std::string));
)

TEST_BODY(RequestLessOrEqual, ValueNotInVault,
    Vault vlt;
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord(vrr, {{"A", 1}, {"B", std::string("a")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("b")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bbb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 3}, {"B", std::string("c")}});
    parentVrs.AddRecord(vrr);

    vor = parentVrs.RequestLessOrEqual("A", 0, vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));
)

TEST_BODY(RequestInterval, Invalid,
    VaultRecordSet parentVrs;
    VaultRecordSet vrs;
    VaultOperationResult vor;

    vor = parentVrs.RequestInterval("A", 0, 1, vrs);

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::ParentVaultNotValid, SavedType == typeid(void));
)

TEST_BODY(RequestInterval, CorrectRequest,
    Vault vlt;
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord(vrr, {{"A", 1}, {"B", std::string("a")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("b")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 3}, {"B", std::string("c")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 4}, {"B", std::string("d")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 4}, {"B", std::string("dd")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 5}, {"B", std::string("ee")}});
    parentVrs.AddRecord(vrr);

    vor = parentVrs.RequestInterval("A", 2, 4, vrs);

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
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord(vrr, {{"A", 1}, {"B", std::string("a")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("b")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 3}, {"B", std::string("c")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 4}, {"B", std::string("d")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 4}, {"B", std::string("dd")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 5}, {"B", std::string("ee")}});
    parentVrs.AddRecord(vrr);

    vor = parentVrs.RequestInterval("A", 2, 4, vrs, false);

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
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord(vrr, {{"A", 1}, {"B", std::string("a")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("b")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 3}, {"B", std::string("c")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 4}, {"B", std::string("d")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 4}, {"B", std::string("dd")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 5}, {"B", std::string("ee")}});
    parentVrs.AddRecord(vrr);

    vor = parentVrs.RequestInterval("A", 2, 4, vrs, true, false);

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
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord(vrr, {{"A", 1}, {"B", std::string("a")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("b")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 3}, {"B", std::string("c")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 4}, {"B", std::string("d")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 4}, {"B", std::string("dd")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 5}, {"B", std::string("ee")}});
    parentVrs.AddRecord(vrr);

    vor = parentVrs.RequestInterval("A", 2, 4, vrs, false, false);

    TEST_ASSERT(vrs.Size() == 1);

    COMPARE_VAULT(vrs, {
        {{"A", 3}, {"B", std::string("c")}},
    });

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));
)

TEST_BODY(RequestInterval, CorrectRequestNotAllRecords,
    Vault vlt;
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord(vrr, {{"A", 1}, {"B", std::string("a")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("b")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 3}, {"B", std::string("c")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 4}, {"B", std::string("d")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 4}, {"B", std::string("dd")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 5}, {"B", std::string("ee")}});
    parentVrs.AddRecord(vrr);

    vor = parentVrs.RequestInterval("A", 2, 4, vrs, true, true, 3);

    TEST_ASSERT(vrs.Size() == 3);

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));


    vor = parentVrs.RequestInterval("A", 2, 4, vrs, true, true, 8);

    TEST_ASSERT(vrs.Size() == 5);

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));
)

TEST_BODY(RequestInterval, CorrectRequestWithPredicat,
    Vault vlt;
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord(vrr, {{"A", 1}, {"B", std::string("a")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("b")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 3}, {"B", std::string("c")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 4}, {"B", std::string("d")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 4}, {"B", std::string("dd")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 5}, {"B", std::string("ee")}});
    parentVrs.AddRecord(vrr);

    vor = parentVrs.RequestInterval("A", 2, 4, vrs, true, true, -1, [](const VaultRecordRef& ref) -> bool
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
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;

    vlt.AddKey("A", 0);

    // ....1....5....9....
    vlt.CreateRecord(vrr, {{"A", 1}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 5}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 9}});
    parentVrs.AddRecord(vrr);

    //  xx
    // ....1....5....9....
    parentVrs.RequestInterval("A", -1, 0, vrs);
    COMPARE_VAULT(vrs, {});

    parentVrs.RequestInterval("A", -1, 0, vrs, false);
    COMPARE_VAULT(vrs, {});

    parentVrs.RequestInterval("A", -1, 0, vrs, true, false);
    COMPARE_VAULT(vrs, {});

    parentVrs.RequestInterval("A", -1, 0, vrs, false, false);
    COMPARE_VAULT(vrs, {});

    //  x  x
    // ....1....5....9....
    parentVrs.RequestInterval("A", -1, 1, vrs);
    COMPARE_VAULT(vrs, {{{"A", 1}}});

    parentVrs.RequestInterval("A", -1, 1, vrs, false);
    COMPARE_VAULT(vrs, {{{"A", 1}}});

    parentVrs.RequestInterval("A", -1, 1, vrs, true, false);
    COMPARE_VAULT(vrs, {});

    parentVrs.RequestInterval("A", -1, 1, vrs, false, false);
    COMPARE_VAULT(vrs, {});

    //  x    x
    // ....1....5....9....
    parentVrs.RequestInterval("A", -1, 2, vrs);
    COMPARE_VAULT(vrs, {{{"A", 1}}});

    parentVrs.RequestInterval("A", -1, 2, vrs, false);
    COMPARE_VAULT(vrs, {{{"A", 1}}});

    parentVrs.RequestInterval("A", -1, 2, vrs, true, false);
    COMPARE_VAULT(vrs, {{{"A", 1}}});

    parentVrs.RequestInterval("A", -1, 2, vrs, false, false);
    COMPARE_VAULT(vrs, {{{"A", 1}}});

    //     x x
    // ....1....5....9....
    parentVrs.RequestInterval("A", 1, 2, vrs);
    COMPARE_VAULT(vrs, {{{"A", 1}}});

    parentVrs.RequestInterval("A", 1, 2, vrs, false);
    COMPARE_VAULT(vrs, {});

    parentVrs.RequestInterval("A", 1, 2, vrs, true, false);
    COMPARE_VAULT(vrs, {{{"A", 1}}});

    parentVrs.RequestInterval("A", 1, 2, vrs, false, false);
    COMPARE_VAULT(vrs, {});

    //       xx
    // ....1....5....9....
    parentVrs.RequestInterval("A", 2, 3, vrs);
    COMPARE_VAULT(vrs, {});

    parentVrs.RequestInterval("A", 2, 3, vrs, false);
    COMPARE_VAULT(vrs, {});

    parentVrs.RequestInterval("A", 2, 3, vrs, true, false);
    COMPARE_VAULT(vrs, {});

    parentVrs.RequestInterval("A", 2, 3, vrs, false, false);
    COMPARE_VAULT(vrs, {});

    //       x  x
    // ....1....5....9....
    parentVrs.RequestInterval("A", 2, 5, vrs);
    COMPARE_VAULT(vrs, {{{"A", 5}}});

    parentVrs.RequestInterval("A", 2, 5, vrs, false);
    COMPARE_VAULT(vrs, {{{"A", 5}}});

    parentVrs.RequestInterval("A", 2, 5, vrs, true, false);
    COMPARE_VAULT(vrs, {});

    parentVrs.RequestInterval("A", 2, 5, vrs, false, false);
    COMPARE_VAULT(vrs, {});

    //       x    x
    // ....1....5....9....
    parentVrs.RequestInterval("A", 2, 6, vrs);
    COMPARE_VAULT(vrs, {{{"A", 5}}});

    parentVrs.RequestInterval("A", 2, 6, vrs, false);
    COMPARE_VAULT(vrs, {{{"A", 5}}});

    parentVrs.RequestInterval("A", 2, 6, vrs, true, false);
    COMPARE_VAULT(vrs, {{{"A", 5}}});

    parentVrs.RequestInterval("A", 2, 6, vrs, false, false);
    COMPARE_VAULT(vrs, {{{"A", 5}}});

    //          x  x
    // ....1....5....9....
    parentVrs.RequestInterval("A", 5, 6, vrs);
    COMPARE_VAULT(vrs, {{{"A", 5}}});

    parentVrs.RequestInterval("A", 5, 6, vrs, false);
    COMPARE_VAULT(vrs, {});

    parentVrs.RequestInterval("A", 5, 6, vrs, true, false);
    COMPARE_VAULT(vrs, {{{"A", 5}}});

    parentVrs.RequestInterval("A", 5, 6, vrs, false, false);
    COMPARE_VAULT(vrs, {});

    //            xx
    // ....1....5....9....
    parentVrs.RequestInterval("A", 6, 7, vrs);
    COMPARE_VAULT(vrs, {});

    parentVrs.RequestInterval("A", 6, 7, vrs, false);
    COMPARE_VAULT(vrs, {});

    parentVrs.RequestInterval("A", 6, 7, vrs, true, false);
    COMPARE_VAULT(vrs, {});

    parentVrs.RequestInterval("A", 6, 7, vrs, false, false);
    COMPARE_VAULT(vrs, {});

    //            x  x
    // ....1....5....9....
    parentVrs.RequestInterval("A", 6, 9, vrs);
    COMPARE_VAULT(vrs, {{{"A", 9}}});

    parentVrs.RequestInterval("A", 6, 9, vrs, false);
    COMPARE_VAULT(vrs, {{{"A", 9}}});

    parentVrs.RequestInterval("A", 6, 9, vrs, true, false);
    COMPARE_VAULT(vrs, {});

    parentVrs.RequestInterval("A", 6, 9, vrs, false, false);
    COMPARE_VAULT(vrs, {});

    //            x    x
    // ....1....5....9....
    parentVrs.RequestInterval("A", 6, 10, vrs);
    COMPARE_VAULT(vrs, {{{"A", 9}}});

    parentVrs.RequestInterval("A", 6, 10, vrs, false);
    COMPARE_VAULT(vrs, {{{"A", 9}}});

    parentVrs.RequestInterval("A", 6, 10, vrs, true, false);
    COMPARE_VAULT(vrs, {{{"A", 9}}});

    parentVrs.RequestInterval("A", 6, 10, vrs, false, false);
    COMPARE_VAULT(vrs, {{{"A", 9}}});

    //               x x
    // ....1....5....9....
    parentVrs.RequestInterval("A", 9, 10, vrs);
    COMPARE_VAULT(vrs, {{{"A", 9}}});

    parentVrs.RequestInterval("A", 9, 10, vrs, false);
    COMPARE_VAULT(vrs, {});

    parentVrs.RequestInterval("A", 9, 10, vrs, true, false);
    COMPARE_VAULT(vrs, {{{"A", 9}}});

    parentVrs.RequestInterval("A", 9, 10, vrs, false, false);
    COMPARE_VAULT(vrs, {});

    //                 xx
    // ....1....5....9....
    parentVrs.RequestInterval("A", 10, 11, vrs);
    COMPARE_VAULT(vrs, {});

    parentVrs.RequestInterval("A", 10, 11, vrs, false);
    COMPARE_VAULT(vrs, {});

    parentVrs.RequestInterval("A", 10, 11, vrs, true, false);
    COMPARE_VAULT(vrs, {});

    parentVrs.RequestInterval("A", 10, 11, vrs, false, false);
    COMPARE_VAULT(vrs, {});
)

TEST_BODY(RequestInterval, SameBoundaries,
    Vault vlt;
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord(vrr, {{"A", 1}, {"B", std::string("a")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("b")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 3}, {"B", std::string("c")}});
    parentVrs.AddRecord(vrr);

    vor = parentVrs.RequestInterval("A", 2, 2, vrs);

    TEST_ASSERT(vrs.Size() == 2);

    COMPARE_VAULT(vrs, {
        {{"A", 2}, {"B", std::string("b")}},
        {{"A", 2}, {"B", std::string("bb")}},
    });

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));


    vor = parentVrs.RequestInterval("A", 2, 2, vrs, false);

    TEST_ASSERT(vrs.Size() == 0);
    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));


    vor = parentVrs.RequestInterval("A", 2, 2, vrs, true, false);

    TEST_ASSERT(vrs.Size() == 0);
    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));


    vor = parentVrs.RequestInterval("A", 2, 2, vrs, false, false);

    TEST_ASSERT(vrs.Size() == 0);
    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));
)

TEST_BODY(RequestInterval, IncorrectBoundaries,
    Vault vlt;
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord(vrr, {{"A", 1}, {"B", std::string("a")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("b")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("bb")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 3}, {"B", std::string("c")}});
    parentVrs.AddRecord(vrr);

    vor = parentVrs.RequestInterval("A", 2, -2, vrs);

    TEST_ASSERT(vrs.Size() == 0);
    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));
)

TEST_BODY(RequestInterval, WrongKey,
    Vault vlt;
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord(vrr, {{"A", 1}, {"B", std::string("a")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("b")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 3}, {"B", std::string("c")}});
    parentVrs.AddRecord(vrr);

    vor = parentVrs.RequestInterval("Z", 1, 1, vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "Z", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongKey, SavedType == typeid(void));
)

TEST_BODY(RequestInterval, WrongType,
    Vault vlt;
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord(vrr, {{"A", 1}, {"B", std::string("a")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 2}, {"B", std::string("b")}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 3}, {"B", std::string("c")}});
    parentVrs.AddRecord(vrr);

    vor = parentVrs.RequestInterval("B", 1, 1, vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "B", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongType, SavedType == typeid(std::string));
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

    GetIsParentVaultValid::Invalid();
    GetIsParentVaultValid::Valid();
    GetIsParentVaultValid::Invalidate();

    GetParentVaultUniqueId::Invalid();
    GetParentVaultUniqueId::Valid();
    GetParentVaultUniqueId::Invalidate();

    IsKeyExist::Invalid();
    IsKeyExist::ExistingKey();
    IsKeyExist::NonExistingKey();

    GetKeyValue::Invalid();
    GetKeyValue::ExistingKey();
    GetKeyValue::NonExistingKey();
    GetKeyValue::WrongType();
    GetKeyValue::UniqueKey();

    GetKeyType::Invalid();
    GetKeyType::ExistingKey();
    GetKeyType::NonExistingKey();

    AddRecord::Invalid();
    AddRecord::Correct();
    AddRecord::InvalidRef();
    AddRecord::OtherVaultParent();

    GetRecord::Invalid();
    GetRecord::CorrectGetRecord();
    GetRecord::WrongKey();
    GetRecord::WrongType();
    GetRecord::WrongValue();

    GetRecords::Invalid();
    GetRecords::CorrectGetRecords();
    GetRecords::CorrectGetNotAllRecords();
    GetRecords::WrongKey();
    GetRecords::WrongType();
    GetRecords::WrongValue();

    RequestEqual::Invalid();
    RequestEqual::CorrectRequest();
    RequestEqual::CorrectRequestNotAllRecords();
    RequestEqual::CorrectRequestWithPredicat();
    RequestEqual::WrongKey();
    RequestEqual::WrongType();
    RequestEqual::ValueNotInVault();

    RequestGreater::Invalid();
    RequestGreater::CorrectRequest();
    RequestGreater::CorrectRequestNotAllRecords();
    RequestGreater::CorrectRequestWithPredicat();
    RequestGreater::BoundaryValues();
    RequestGreater::WrongKey();
    RequestGreater::WrongType();
    RequestGreater::ValueNotInVault();

    RequestGreaterOrEqual::Invalid();
    RequestGreaterOrEqual::CorrectRequest();
    RequestGreaterOrEqual::CorrectRequestNotAllRecords();
    RequestGreaterOrEqual::CorrectRequestWithPredicat();
    RequestGreaterOrEqual::BoundaryValues();
    RequestGreaterOrEqual::WrongKey();
    RequestGreaterOrEqual::WrongType();
    RequestGreaterOrEqual::ValueNotInVault();

    RequestLess::Invalid();
    RequestLess::CorrectRequest();
    RequestLess::CorrectRequestNotAllRecords();
    RequestLess::CorrectRequestWithPredicat();
    RequestLess::BoundaryValues();
    RequestLess::WrongKey();
    RequestLess::WrongType();
    RequestLess::ValueNotInVault();

    RequestLessOrEqual::Invalid();
    RequestLessOrEqual::CorrectRequest();
    RequestLessOrEqual::CorrectRequestNotAllRecords();
    RequestLessOrEqual::CorrectRequestWithPredicat();
    RequestLessOrEqual::BoundaryValues();
    RequestLessOrEqual::WrongKey();
    RequestLessOrEqual::WrongType();
    RequestLessOrEqual::ValueNotInVault();

    RequestInterval::Invalid();
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
}