#include "VaultRecordSetUnitTests.h"

#define GENERATE_SET(setName) \
VaultRecordSet setName; \
vlt.AddKey("A", 0); \
vlt.CreateRecord({{}}); \
vlt.RequestEqual("A", 0, setName); \
vlt.EraseRecord("A", 0); \
vlt.RemoveKey("A");

namespace VaultRecordSetTestNs
{
std::vector<std::pair<std::string, std::function<void()>>> Tests;

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

TEST_BODY(AssignmentOperator, AssignWithoutParentToWithParent,
    Vault vlt;
    VaultRecordSet vrs1, vrs2;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({ {"A", 0}, {"B", std::string("a")} });
    vlt.CreateRecord({ {"A", 0}, {"B", std::string("b")} });
    vlt.CreateRecord({ {"A", 0}, {"B", std::string("c")} });

    vlt.RequestEqual("A", 0, vrs2);

    vrs2 = vrs1;

    TEST_ASSERT(vrs2.Size() == 0);
    TEST_ASSERT(vrs2.GetKeys().size() == 0);
    TEST_ASSERT(vrs2.GetUniqueKeys().size() == 0);
    TEST_ASSERT(vrs2.GetIsParentVaultValid() == false);

    COMPARE_VAULT(vrs2, {});
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

TEST_BODY(AssignmentOperator, AssignEmptyToFilled,
    Vault vlt;
    VaultRecordSet vrs1, vrs2;
    VaultRecordRef vrr;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord(vrr, { {"A", 0}, {"B", std::string("a")} });
    vlt.Request(Equal("A", 0), vrs1);
    vrs1.RemoveRecord(vrr);

    vlt.CreateRecord({ {"A", 0}, {"B", std::string("b")} });
    vlt.CreateRecord({ {"A", 0}, {"B", std::string("c")} });

    vlt.RequestEqual("A", 0, vrs2);

    vrs2 = vrs1;

    TEST_ASSERT(vrs2.Size() == 0);
    TEST_ASSERT(vrs2.GetKeys().size() == 2);
    TEST_ASSERT(vrs2.GetUniqueKeys().size() == 1);
    TEST_ASSERT(vrs2.GetIsParentVaultValid() == true);

    COMPARE_VAULT(vrs2, {});
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
    VaultRecordSet* vrs1 = new VaultRecordSet;
    VaultRecordRef vrr;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");
    
    vlt.CreateRecord({ {"A", 0}, {"B", std::string("a")} });
    vlt.CreateRecord({ {"A", 0}, {"B", std::string("b")} });
    vlt.CreateRecord(vrr, { {"A", 0}, {"B", std::string("c")} });

    vlt.RequestEqual("A", 0, *vrs1);

    VaultRecordSet vrs2(std::move(*vrs1));

    TEST_ASSERT(vrs2.Size() == 3);
    TEST_ASSERT(vrs2.GetKeys().size() == 2);
    TEST_ASSERT(vrs2.GetUniqueKeys().size() == 1);
    TEST_ASSERT(vrs2.GetIsParentVaultValid() == true);

    COMPARE_VAULT(vrs2, {
        {{"A", 0}, {"B",  std::string("a")}},
        {{"A", 0}, {"B",  std::string("b")}},
        {{"A", 0}, {"B",  std::string("c")}},
    });

    TEST_ASSERT(vrs1->Size() == 0);
    TEST_ASSERT(vrs1->GetKeys().size() == 0);
    TEST_ASSERT(vrs1->GetUniqueKeys().size() == 0);
    TEST_ASSERT(vrs1->GetIsParentVaultValid() == false);
    TEST_ASSERT(vrs1->GetParentVaultUniqueId() == "null");
    COMPARE_VAULT(*vrs1, {});

    delete vrs1;
    vrr.SetData("A", 12);
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

TEST_BODY(MoveAssignmentOperator, AssignWithoutParentToWithParent,
    Vault vlt;
    VaultRecordSet vrs1, vrs2;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({ {"A", 0}, {"B", std::string("a")} });
    vlt.CreateRecord({ {"A", 0}, {"B", std::string("b")} });
    vlt.CreateRecord({ {"A", 0}, {"B", std::string("c")} });

    vlt.RequestEqual("A", 0, vrs2);

    vrs2 = std::move(vrs1);

    TEST_ASSERT(vrs2.Size() == 0);
    TEST_ASSERT(vrs2.GetKeys().size() == 0);
    TEST_ASSERT(vrs2.GetUniqueKeys().size() == 0);
    TEST_ASSERT(vrs2.GetIsParentVaultValid() == false);

    COMPARE_VAULT(vrs2, {});
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

TEST_BODY(MoveAssignmentOperator, AssignEmptyToFilled,
    Vault vlt;
    VaultRecordSet vrs1, vrs2;
    VaultRecordRef vrr;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord(vrr, { {"A", 0}, {"B", std::string("a")} });
    vlt.Request(Equal("A", 0), vrs1);
    vrs1.RemoveRecord(vrr);

    vlt.CreateRecord({ {"A", 0}, {"B", std::string("b")} });
    vlt.CreateRecord({ {"A", 0}, {"B", std::string("c")} });

    vlt.RequestEqual("A", 0, vrs2);

    vrs2 = std::move(vrs1);

    TEST_ASSERT(vrs2.Size() == 0);
    TEST_ASSERT(vrs2.GetKeys().size() == 2);
    TEST_ASSERT(vrs2.GetUniqueKeys().size() == 1);
    TEST_ASSERT(vrs2.GetIsParentVaultValid() == true);

    COMPARE_VAULT(vrs2, {});
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

TEST_BODY(AddRecord, SecondTime,
    Vault vlt;
    GENERATE_SET(vrs);
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.CreateRecord(vrr, {});

    vor = vrs.AddRecord(vrr);

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "", 
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(void));
    
    vor = vrs.AddRecord(vrr);

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "", 
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::RecordAlredyInSet, SavedType == typeid(void));
    
    TEST_ASSERT(vrs.Size() == 1);

    COMPARE_VAULT(vrs, {
        {{"A", 0}}
    });
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

TEST_BODY(RequestEqual, Self,
    Vault vlt;
    GENERATE_SET(vrs);
    VaultOperationResult vor;

    vlt.AddKey("A", 0);

    vor = vrs.RequestEqual("A", 4, vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::SameVaultRecordSet, SavedType == typeid(void));
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

TEST_BODY(RequestGreater, Self,
    Vault vlt;
    GENERATE_SET(vrs);
    VaultOperationResult vor;

    vlt.AddKey("A", 0);

    vor = vrs.RequestGreater("A", 4, vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::SameVaultRecordSet, SavedType == typeid(void));
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

TEST_BODY(RequestGreaterOrEqual, Self,
    Vault vlt;
    GENERATE_SET(vrs);
    VaultOperationResult vor;

    vlt.AddKey("A", 0);

    vor = vrs.RequestGreaterOrEqual("A", 4, vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::SameVaultRecordSet, SavedType == typeid(void));
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

TEST_BODY(RequestLess, Self,
    Vault vlt;
    GENERATE_SET(vrs);
    VaultOperationResult vor;

    vlt.AddKey("A", 0);

    vor = vrs.RequestLess("A", 4, vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::SameVaultRecordSet, SavedType == typeid(void));
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

TEST_BODY(RequestLessOrEqual, Self,
    Vault vlt;
    GENERATE_SET(vrs);
    VaultOperationResult vor;

    vlt.AddKey("A", 0);

    vor = vrs.RequestLessOrEqual("A", 4, vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::SameVaultRecordSet, SavedType == typeid(void));
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

TEST_BODY(RequestInterval, Self,
    Vault vlt;
    GENERATE_SET(vrs);
    VaultOperationResult vor;

    vlt.AddKey("A", 0);

    vor = vrs.RequestInterval("A", 2, 4, vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::SameVaultRecordSet, SavedType == typeid(void));
)

TEST_BODY(Request, CorrectRequest,
    Vault vlt;
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;
    std::vector<std::vector<std::pair<std::string, TypeWrapper>>> records;

    vlt.AddKey("A", 0);

    for (int i = 0; i < 10; ++i) 
    {
        vlt.CreateRecord(vrr, {{"A", i}});
        parentVrs.AddRecord(vrr);
    }

    // Equal
    COMPARE_CORRECT_REQUEST_SET(Equal("A", 2), {{{"A", 2}}})

    // Greater
    COMPARE_CORRECT_REQUEST_SET(Greater("A", 7), {{{"A", 8}}, {{"A", 9}}});

    // Greater or equal
    COMPARE_CORRECT_REQUEST_SET(GreaterOrEqual("A", 7), {{{"A", 7}}, {{"A", 8}}, {{"A", 9}}});

    // Less
    COMPARE_CORRECT_REQUEST_SET(Less("A", 2), {{{"A", 0}}, {{"A", 1}}});

    // Less or equal
    COMPARE_CORRECT_REQUEST_SET(LessOrEqual("A", 2), {{{"A", 0}}, {{"A", 1}}, {{"A", 2}}});
)

TEST_BODY(Request, CorrectRequestWithPredicat,
    Vault vlt;
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddKey("B", 0);

    for (int i = 0; i < 10; ++i) 
    {
        vlt.CreateRecord(vrr, {{"A", i}, {"B", 100 - i}});
        parentVrs.AddRecord(vrr);
    }

    for (int i = 0; i < 4; ++i) 
    {
        vlt.CreateRecord(vrr, {{"A", 5}, {"B", i}});
        parentVrs.AddRecord(vrr);
    }

    // Equal
    vor = parentVrs.Request(Equal("A", 5, [](const VaultRecordRef& ref) -> bool
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

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "",
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(void));


    // Greater
    vor = parentVrs.Request(Greater("A", 7, [](const VaultRecordRef& ref) -> bool
    {
        int b = 0;
        ref.GetData("B", b);
        if (b % 2 == 0) return true;
        else return false;
    }), vrs);

    TEST_ASSERT(vrs.Size() == 1);

    COMPARE_VAULT(vrs, {{{"A", 8}, {"B", 92}}});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "",
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(void));


    // Greater or equal
    vor = parentVrs.Request(GreaterOrEqual("A", 7, [](const VaultRecordRef& ref) -> bool
    {
        int b = 0;
        ref.GetData("B", b);
        if (b % 2 == 0) return true;
        else return false;
    }), vrs);

    TEST_ASSERT(vrs.Size() == 1);

    COMPARE_VAULT(vrs, {{{"A", 8}, {"B", 92}}});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "",
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(void));


    // Less
    vor = parentVrs.Request(Less("A", 2, [](const VaultRecordRef& ref) -> bool
    {
        int b = 0;
        ref.GetData("B", b);
        if (b % 2 == 0) return true;
        else return false;
    }), vrs);

    TEST_ASSERT(vrs.Size() == 1);

    COMPARE_VAULT(vrs, {{{"A", 0}, {"B", 100}}});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "",
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(void));


    // Less or equal
    vor = parentVrs.Request(LessOrEqual("A", 2, [](const VaultRecordRef& ref) -> bool
    {
        int b = 0;
        ref.GetData("B", b);
        if (b % 2 == 0) return true;
        else return false;
    }), vrs);

    TEST_ASSERT(vrs.Size() == 2);

    COMPARE_VAULT(vrs, {{{"A", 0}, {"B", 100}}, {{"A", 2}, {"B", 98}}});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "",
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(void));
)

TEST_BODY(Request, BoundaryValues,
    Vault vlt;
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;
    std::vector<std::vector<std::pair<std::string, TypeWrapper>>> records;

    vlt.AddKey("A", 0);

    // ...1...3...
    vlt.CreateRecord(vrr, {{"A", 1}});
    parentVrs.AddRecord(vrr);
    vlt.CreateRecord(vrr, {{"A", 3}});
    parentVrs.AddRecord(vrr);

    // Greater
    //  x
    // ...1...3...
    COMPARE_CORRECT_REQUEST_SET(Greater("A", 0), {{{"A", 1}}, {{"A", 3}}});

    //    x
    // ...1...3...
    COMPARE_CORRECT_REQUEST_SET(Greater("A", 1), {{{"A", 3}}});

    //      x
    // ...1...3...
    COMPARE_CORRECT_REQUEST_SET(Greater("A", 2), {{{"A", 3}}});

    //        x
    // ...1...3...
    COMPARE_CORRECT_REQUEST_SET(Greater("A", 3), {});

    //          x
    // ...1...3...
    COMPARE_CORRECT_REQUEST_SET(Greater("A", 4), {});


    // Greater or equal
    //  x
    // ...1...3...
    COMPARE_CORRECT_REQUEST_SET(GreaterOrEqual("A", 0), {{{"A", 1}}, {{"A", 3}}});

    //    x
    // ...1...3...
    COMPARE_CORRECT_REQUEST_SET(GreaterOrEqual("A", 1), {{{"A", 1}}, {{"A", 3}}});

    //      x
    // ...1...3...
    COMPARE_CORRECT_REQUEST_SET(GreaterOrEqual("A", 2), {{{"A", 3}}});

    //        x
    // ...1...3...
    COMPARE_CORRECT_REQUEST_SET(GreaterOrEqual("A", 3), {{{"A", 3}}});

    //          x
    // ...1...3...
    COMPARE_CORRECT_REQUEST_SET(GreaterOrEqual("A", 4), {});


    // Less
    //  x
    // ...1...3...
    COMPARE_CORRECT_REQUEST_SET(Less("A", 0), {});

    //    x
    // ...1...3...
    COMPARE_CORRECT_REQUEST_SET(Less("A", 1), {});

    //      x
    // ...1...3...
    COMPARE_CORRECT_REQUEST_SET(Less("A", 2), {{{"A", 1}}});

    //        x
    // ...1...3...
    COMPARE_CORRECT_REQUEST_SET(Less("A", 3), {{{"A", 1}}});

    //          x
    // ...1...3...
    COMPARE_CORRECT_REQUEST_SET(Less("A", 4), {{{"A", 1}}, {{"A", 3}}});


    // Less or equal
    //  x
    // ...1...3...
    COMPARE_CORRECT_REQUEST_SET(LessOrEqual("A", 0), {});

    //    x
    // ...1...3...
    COMPARE_CORRECT_REQUEST_SET(LessOrEqual("A", 1), {{{"A", 1}}});

    //      x
    // ...1...3...
    COMPARE_CORRECT_REQUEST_SET(LessOrEqual("A", 2), {{{"A", 1}}});

    //        x
    // ...1...3...
    COMPARE_CORRECT_REQUEST_SET(LessOrEqual("A", 3), {{{"A", 1}}, {{"A", 3}}});

    //          x
    // ...1...3...
    COMPARE_CORRECT_REQUEST_SET(LessOrEqual("A", 4), {{{"A", 1}}, {{"A", 3}}});
)

TEST_BODY(Request, WrongKey,
    Vault vlt;
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);

    for (int i = 0; i < 10; ++i) 
    {
        vlt.CreateRecord(vrr, {{"A", i}});
        parentVrs.AddRecord(vrr);
    }

    // Equal
    vor = parentVrs.Request(Equal("Z", 2), vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "Z", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongKey, SavedType == typeid(void));


    // Greater
    vor = parentVrs.Request(Greater("Z", 7), vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "Z", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongKey, SavedType == typeid(void));


    // Greater or equal
    vor = parentVrs.Request(GreaterOrEqual("Z", 7), vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "Z", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongKey, SavedType == typeid(void));


    // Less
    vor = parentVrs.Request(Less("Z", 2), vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "Z", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongKey, SavedType == typeid(void));


    // Less or equal
    vor = parentVrs.Request(LessOrEqual("Z", 2), vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "Z", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongKey, SavedType == typeid(void));
)

TEST_BODY(Request, WrongType,
    Vault vlt;
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;
    std::string s;

    vlt.AddKey("A", 0);

    for (int i = 0; i < 10; ++i) vlt.CreateRecord({{"A", i}});

    // Equal
    vor = parentVrs.Request(Equal("A", s), vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(std::string), ResultCode == VaultOperationResultCode::WrongType, SavedType == typeid(int));


    // Greater
    vor = parentVrs.Request(Greater("A", s), vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(std::string), ResultCode == VaultOperationResultCode::WrongType, SavedType == typeid(int));


    // Greater or equal
    vor = parentVrs.Request(GreaterOrEqual("A", s), vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(std::string), ResultCode == VaultOperationResultCode::WrongType, SavedType == typeid(int));


    // Less
    vor = parentVrs.Request(Less("A", s), vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(std::string), ResultCode == VaultOperationResultCode::WrongType, SavedType == typeid(int));


    // Less or equal
    vor = parentVrs.Request(LessOrEqual("A", s), vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(std::string), ResultCode == VaultOperationResultCode::WrongType, SavedType == typeid(int));
)

TEST_BODY(Request, ComplexRequestAnd,
    Vault vlt;
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;
    std::vector<std::vector<std::pair<std::string, TypeWrapper>>> records;

    vlt.AddKey("A", 0);
    vlt.AddKey("B", 0);

    for (int i = 0; i < 10; ++i) 
    {
        vlt.CreateRecord(vrr, {{"A", i}, {"B", i}});
        parentVrs.AddRecord(vrr);
    }

    // Equal
    COMPARE_CORRECT_REQUEST_SET(Equal("A", 2) && Equal("B", 2), {{{"A", 2}, {"B", 2}}});

    COMPARE_CORRECT_REQUEST_SET(Equal("A", 2) && Equal("B", 3), {});

    COMPARE_CORRECT_REQUEST_SET(Equal("A", 2) && Greater("B", 1), {{{"A", 2}, {"B", 2}}});

    COMPARE_CORRECT_REQUEST_SET(Equal("A", 2) && GreaterOrEqual("B", 2), {{{"A", 2}, {"B", 2}}});

    COMPARE_CORRECT_REQUEST_SET(Equal("A", 2) && Less("B", 5), {{{"A", 2}, {"B", 2}}});

    COMPARE_CORRECT_REQUEST_SET(Equal("A", 2) && LessOrEqual("B", 2), {{{"A", 2}, {"B", 2}}});


    // Greater
    COMPARE_CORRECT_REQUEST_SET(Greater("A", 2) && Equal("B", 9), {{{"A", 9}, {"B", 9}}});

    COMPARE_CORRECT_REQUEST_SET(Greater("A", 7) && Greater("B", 1), {{{"A", 8}, {"B", 8}}, {{"A", 9}, {"B", 9}}});

    COMPARE_CORRECT_REQUEST_SET(Greater("A", 8) && GreaterOrEqual("B", 8), {{{"A", 9}, {"B", 9}}});

    COMPARE_CORRECT_REQUEST_SET(Greater("A", 2) && Less("B", 4), {{{"A", 3}, {"B", 3}}});

    COMPARE_CORRECT_REQUEST_SET(Greater("A", 2) && LessOrEqual("B", 3), {{{"A", 3}, {"B", 3}}});


    // Greater or equal
    COMPARE_CORRECT_REQUEST_SET(GreaterOrEqual("A", 2) && Equal("B", 5), {{{"A", 5}, {"B", 5}}});

    COMPARE_CORRECT_REQUEST_SET(GreaterOrEqual("A", 8) && Greater("B", 1), {{{"A", 8}, {"B", 8}}, {{"A", 9}, {"B", 9}}});

    COMPARE_CORRECT_REQUEST_SET(GreaterOrEqual("A", 8) && GreaterOrEqual("B", 9), {{{"A", 9}, {"B", 9}}});

    COMPARE_CORRECT_REQUEST_SET(GreaterOrEqual("A", 2) && Less("B", 4), {{{"A", 2}, {"B", 2}}, {{"A", 3}, {"B", 3}}});

    COMPARE_CORRECT_REQUEST_SET(GreaterOrEqual("A", 2) && LessOrEqual("B", 3), {{{"A", 2}, {"B", 2}}, {{"A", 3}, {"B", 3}}});


    // Less
    COMPARE_CORRECT_REQUEST_SET(Less("A", 7) && Equal("B", 5), {{{"A", 5}, {"B", 5}}});

    COMPARE_CORRECT_REQUEST_SET(Less("A", 3) && Greater("B", 1), {{{"A", 2}, {"B", 2}}});

    COMPARE_CORRECT_REQUEST_SET(Less("A", 8) && GreaterOrEqual("B", 7), {{{"A", 7}, {"B", 7}}});

    COMPARE_CORRECT_REQUEST_SET(Less("A", 2) && Less("B", 4), {{{"A", 0}, {"B", 0}}, {{"A", 1}, {"B", 1}}});

    COMPARE_CORRECT_REQUEST_SET(Less("A", 2) && LessOrEqual("B", 1), {{{"A", 0}, {"B", 0}}, {{"A", 1}, {"B", 1}}});


    // Less or equal
    COMPARE_CORRECT_REQUEST_SET(LessOrEqual("A", 7) && Equal("B", 5), {{{"A", 5}, {"B", 5}}});

    COMPARE_CORRECT_REQUEST_SET(LessOrEqual("A", 3) && Greater("B", 2), {{{"A", 3}, {"B", 3}}});

    COMPARE_CORRECT_REQUEST_SET(LessOrEqual("A", 8) && GreaterOrEqual("B", 8), {{{"A", 8}, {"B", 8}}});

    COMPARE_CORRECT_REQUEST_SET(LessOrEqual("A", 2) && Less("B", 4), {{{"A", 0}, {"B", 0}}, {{"A", 1}, {"B", 1}}, {{"A", 2}, {"B", 2}}});

    COMPARE_CORRECT_REQUEST_SET(LessOrEqual("A", 2) && LessOrEqual("B", 1), {{{"A", 0}, {"B", 0}}, {{"A", 1}, {"B", 1}}});
)

TEST_BODY(Request, ComplexRequestOr,
    Vault vlt;
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;
    std::vector<std::vector<std::pair<std::string, TypeWrapper>>> records;

    vlt.AddKey("A", 0);
    vlt.AddKey("B", 0);

    for (int i = 0; i < 10; ++i) 
    {
        vlt.CreateRecord(vrr, {{"A", i}, {"B", i}});
        parentVrs.AddRecord(vrr);
    }

    // Equal
    COMPARE_CORRECT_REQUEST_SET(Equal("A", 2) || Equal("B", 3), {{{"A", 2}, {"B", 2}}, {{"A", 3}, {"B", 3}}});

    COMPARE_CORRECT_REQUEST_SET(Equal("A", 2) || Greater("B", 8), {{{"A", 2}, {"B", 2}}, {{"A", 9}, {"B", 9}}});

    COMPARE_CORRECT_REQUEST_SET(Equal("A", 2) || GreaterOrEqual("B", 9), {{{"A", 2}, {"B", 2}}, {{"A", 9}, {"B", 9}}});

    COMPARE_CORRECT_REQUEST_SET(Equal("A", 2) || Less("B", 2), {{{"A", 0}, {"B", 0}}, {{"A", 1}, {"B", 1}}, {{"A", 2}, {"B", 2}}});

    COMPARE_CORRECT_REQUEST_SET(Equal("A", 2) || LessOrEqual("B", 1), {{{"A", 0}, {"B", 0}}, {{"A", 1}, {"B", 1}}, {{"A", 2}, {"B", 2}}});


    // Greater
    COMPARE_CORRECT_REQUEST_SET(Greater("A", 8) || Equal("B", 0), {{{"A", 0}, {"B", 0}}, {{"A", 9}, {"B", 9}}});

    COMPARE_CORRECT_REQUEST_SET(Greater("A", 8) || Greater("B", 10), {{{"A", 9}, {"B", 9}}});

    COMPARE_CORRECT_REQUEST_SET(Greater("A", 8) || GreaterOrEqual("B", 9), {{{"A", 9}, {"B", 9}}});

    COMPARE_CORRECT_REQUEST_SET(Greater("A", 8) || Less("B", 1), {{{"A", 0}, {"B", 0}}, {{"A", 9}, {"B", 9}}});

    COMPARE_CORRECT_REQUEST_SET(Greater("A", 8) || LessOrEqual("B", 0), {{{"A", 0}, {"B", 0}}, {{"A", 9}, {"B", 9}}});


    // Greater or equal
    COMPARE_CORRECT_REQUEST_SET(GreaterOrEqual("A", 9) || Equal("B", 5), {{{"A", 5}, {"B", 5}}, {{"A", 9}, {"B", 9}}});

    COMPARE_CORRECT_REQUEST_SET(GreaterOrEqual("A", 8) || Greater("B", 8), {{{"A", 8}, {"B", 8}}, {{"A", 9}, {"B", 9}}});

    COMPARE_CORRECT_REQUEST_SET(GreaterOrEqual("A", 8) || GreaterOrEqual("B", 9), {{{"A", 8}, {"B", 8}}, {{"A", 9}, {"B", 9}}});

    COMPARE_CORRECT_REQUEST_SET(GreaterOrEqual("A", 9) || Less("B", 1), {{{"A", 0}, {"B", 0}}, {{"A", 9}, {"B", 9}}});

    COMPARE_CORRECT_REQUEST_SET(GreaterOrEqual("A", 9) || LessOrEqual("B", 0), {{{"A", 0}, {"B", 0}}, {{"A", 9}, {"B", 9}}});


    // Less
    COMPARE_CORRECT_REQUEST_SET(Less("A", 1) || Equal("B", 5), {{{"A", 0}, {"B", 0}}, {{"A", 5}, {"B", 5}}});

    COMPARE_CORRECT_REQUEST_SET(Less("A", 1) || Greater("B", 8), {{{"A", 0}, {"B", 0}}, {{"A", 9}, {"B", 9}}});

    COMPARE_CORRECT_REQUEST_SET(Less("A", 1) || GreaterOrEqual("B", 9), {{{"A", 0}, {"B", 0}}, {{"A", 9}, {"B", 9}}});

    COMPARE_CORRECT_REQUEST_SET(Less("A", 2) || Less("B", 1), {{{"A", 0}, {"B", 0}}, {{"A", 1}, {"B", 1}}});

    COMPARE_CORRECT_REQUEST_SET(Less("A", 1) || LessOrEqual("B", 1), {{{"A", 0}, {"B", 0}}, {{"A", 1}, {"B", 1}}});


    // Less or equal
    COMPARE_CORRECT_REQUEST_SET(LessOrEqual("A", 1) || Equal("B", 5), {{{"A", 0}, {"B", 0}}, {{"A", 1}, {"B", 1}}, {{"A", 5}, {"B", 5}}});

    COMPARE_CORRECT_REQUEST_SET(LessOrEqual("A", 1) || Greater("B", 8), {{{"A", 0}, {"B", 0}}, {{"A", 1}, {"B", 1}}, {{"A", 9}, {"B", 9}}});

    COMPARE_CORRECT_REQUEST_SET(LessOrEqual("A", 1) || GreaterOrEqual("B", 9), {{{"A", 0}, {"B", 0}}, {{"A", 1}, {"B", 1}}, {{"A", 9}, {"B", 9}}});

    COMPARE_CORRECT_REQUEST_SET(LessOrEqual("A", 1) || Less("B", 3), {{{"A", 0}, {"B", 0}}, {{"A", 1}, {"B", 1}}, {{"A", 2}, {"B", 2}}});

    COMPARE_CORRECT_REQUEST_SET(LessOrEqual("A", 2) || LessOrEqual("B", 1), {{{"A", 0}, {"B", 0}}, {{"A", 1}, {"B", 1}}, {{"A", 2}, {"B", 2}}});
)

TEST_BODY(Request, ComplexRequest,
    Vault vlt;
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddKey("B", 0);
    vlt.AddKey<std::string>("C", "");
    vlt.AddKey<bool>("D", false);

    for (int i = 0; i < 10; ++i)
    {
        vlt.CreateRecord(vrr, {{"A", i}, {"B", i}, {"C", std::to_string(i)}});
        parentVrs.AddRecord(vrr);
        if (i == 5 || i == 9) vrr.SetData("D", true);
    }
    
    parentVrs.Request(Greater("A", 2) && Less("B", 8) && (Equal("C", std::string("3")) || Equal("C", std::string("4"))) || Equal("D", true), vrs);
    
    CompareVault(vrs, {
        {{"A", 3}, {"B", 3}, {"C", std::string("3")}, {"D", false}},
        {{"A", 4}, {"B", 4}, {"C", std::string("4")}, {"D", false}},
        {{"A", 5}, {"B", 5}, {"C", std::string("5")}, {"D", true}},
        {{"A", 9}, {"B", 9}, {"C", std::string("9")}, {"D", true}},
    });
)

TEST_BODY(Request, ComplexRequestWrongKey,
    Vault vlt;
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddKey("B", 0);

    for (int i = 0; i < 10; ++i) 
    {
        vlt.CreateRecord(vrr, {{"A", i}, {"B", i}});
        parentVrs.AddRecord(vrr);
    }

    vor = parentVrs.Request(Equal("A", 0) && Equal("Z", 0), vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "Z", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongKey, SavedType == typeid(void));


    vor = parentVrs.Request(Equal("Z", 0) && Equal("Y", 0), vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "Z", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongKey, SavedType == typeid(void));
)

TEST_BODY(Request, ComplexRequestWrongType,
    Vault vlt;
    GENERATE_SET(parentVrs);
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;
    std::string s;

    vlt.AddKey("A", 0);
    vlt.AddKey("B", 0);

    for (int i = 0; i < 10; ++i) 
    {
        vlt.CreateRecord(vrr, {{"A", i}, {"B", i}});
        parentVrs.AddRecord(vrr);
    }

    vor = parentVrs.Request(Equal("A", 0) && Equal("B", s), vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "B", 
        RequestedType == typeid(std::string), ResultCode == VaultOperationResultCode::WrongType, SavedType == typeid(int));


    vor = parentVrs.Request(Equal("A", s) && Equal("B", 0), vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(std::string), ResultCode == VaultOperationResultCode::WrongType, SavedType == typeid(int));
)

TEST_BODY(Request, Self,
    Vault vlt;
    GENERATE_SET(vrs);
    VaultOperationResult vor;

    vlt.AddKey("A", 0);

    vor = vrs.Request(Equal("A", 2), vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "",
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::SameVaultRecordSet, SavedType == typeid(void));


    vor = vrs.Request(Greater("A", 2), vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "",
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::SameVaultRecordSet, SavedType == typeid(void));


    vor = vrs.Request(GreaterOrEqual("A", 2), vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "",
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::SameVaultRecordSet, SavedType == typeid(void));


    vor = vrs.Request(Less("A", 2), vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "",
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::SameVaultRecordSet, SavedType == typeid(void));


    vor = vrs.Request(LessOrEqual("A", 2), vrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "",
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::SameVaultRecordSet, SavedType == typeid(void));
 )

 TEST_BODY(Request, InvalidParent,
    VaultRecordSet vrs, tmpVrs;
    VaultOperationResult vor;

    vor = vrs.Request(Equal("A", 2), tmpVrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "",
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::ParentVaultNotValid, SavedType == typeid(void));


    vor = vrs.Request(Greater("A", 2), tmpVrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "",
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::ParentVaultNotValid, SavedType == typeid(void));


    vor = vrs.Request(GreaterOrEqual("A", 2), tmpVrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "",
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::ParentVaultNotValid, SavedType == typeid(void));


    vor = vrs.Request(Less("A", 2), tmpVrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "",
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::ParentVaultNotValid, SavedType == typeid(void));


    vor = vrs.Request(LessOrEqual("A", 2), tmpVrs);

    TEST_ASSERT(vrs.Size() == 0);

    COMPARE_VAULT(vrs, {});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "",
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::ParentVaultNotValid, SavedType == typeid(void));
 )

TEST_BODY(CheckRecord, InvalidSet,
    VaultRecordSet vrs;
    VaultRecordRef vrr;

    TEST_ASSERT(vrs.CheckRecord(vrr) == false);
)

TEST_BODY(CheckRecord, InvalidRef,
    Vault vlt;
    GENERATE_SET(vrs);
    VaultRecordRef vrr;

    TEST_ASSERT(vrs.CheckRecord(vrr) == false);
)

TEST_BODY(CheckRecord, WrongRef,
    Vault vlt;
    GENERATE_SET(vrs1);
    VaultRecordSet vrs2;
    VaultRecordRef vrr;

    vlt.AddKey("A", 0);
    vlt.CreateRecord({});
    vlt.RequestEqual("A", 0, vrs1);
    vrs1.GetRecord("A", 0, vrr);

    TEST_ASSERT(vrs2.CheckRecord(vrr) == false);
)

TEST_BODY(CheckRecord, Correct,
    Vault vlt;
    VaultRecordSet vrs;
    VaultRecordRef vrr;

    vlt.AddKey("A", 0);
    vlt.CreateRecord({});
    vlt.RequestEqual("A", 0, vrs);
    vrs.GetRecord("A", 0, vrr);

    TEST_ASSERT(vrs.CheckRecord(vrr) == true);
)

// TEST_SUITE(Reset,
        
//     TEST_CASE(InvalidSet)

//     TEST_CASE(EmptySet)

//     TEST_CASE(FilledSet)
// )

TEST_BODY(Reset, InvalidSet,
    VaultRecordSet vrs;
    vrs.Reset();

    TEST_ASSERT(vrs.GetIsParentVaultValid() == false);
    TEST_ASSERT(vrs.GetParentVaultUniqueId() == "null");
)

TEST_BODY(Reset, EmptySet,
    Vault vlt;
    GENERATE_SET(vrs);
    vrs.Reset();

    TEST_ASSERT(vrs.GetIsParentVaultValid() == false);
    TEST_ASSERT(vrs.GetParentVaultUniqueId() == "null");
    TEST_ASSERT(vrs.GetKeys() == std::vector<std::string>());
    TEST_ASSERT(vrs.GetUniqueKeys() == std::vector<std::string>());
    COMPARE_VAULT(vrs, {});
)

TEST_BODY(Reset, FilledSet,
    Vault vlt;
    VaultRecordSet vrs;

    vlt.AddKey("A", 0);
    vlt.CreateRecord({});
    vlt.RequestEqual("A", 0, vrs);

    vrs.Reset();

    TEST_ASSERT(vrs.GetIsParentVaultValid() == false);
    TEST_ASSERT(vrs.GetParentVaultUniqueId() == "null");
    TEST_ASSERT(vrs.GetKeys() == std::vector<std::string>());
    TEST_ASSERT(vrs.GetUniqueKeys() == std::vector<std::string>());
    COMPARE_VAULT(vrs, {});
)

TEST_BODY(Clear, InvalidSet,
    VaultRecordSet vrs;
    vrs.Clear();

    TEST_ASSERT(vrs.GetIsParentVaultValid() == false);
    TEST_ASSERT(vrs.GetParentVaultUniqueId() == "null");
)

TEST_BODY(Clear, EmptySet,
    Vault vlt;
    GENERATE_SET(vrs);
    vrs.Clear();

    TEST_ASSERT(vrs.GetIsParentVaultValid() == true);
    TEST_ASSERT(vrs.GetKeys() == std::vector<std::string>());
    TEST_ASSERT(vrs.GetUniqueKeys() == std::vector<std::string>());
    COMPARE_VAULT(vrs, {});
)

TEST_BODY(Clear, FilledSet,
    Vault vlt;
    VaultRecordSet vrs;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");
    vlt.CreateRecord({{"A", 1}, {"B", std::string("str")}});
    vlt.RequestEqual("A", 1, vrs);

    COMPARE_VAULT(vrs, {
        {{"A", 1}, {"B", std::string("str")}}
    });
    vrs.Clear();

    TEST_ASSERT(vrs.GetIsParentVaultValid() == true);
    TEST_ASSERT(vrs.GetKeys() == std::vector<std::string>({"A", "B"}));
    TEST_ASSERT(vrs.GetUniqueKeys() == std::vector<std::string>({"B"}));
    COMPARE_VAULT(vrs, {});
)

TEST_BODY(RemoveRecordByRef, InvalidSet,
    VaultRecordSet vrs;
    VaultRecordRef vrf;

    TEST_ASSERT(vrs.RemoveRecord(vrf) == false);
)

TEST_BODY(RemoveRecordByRef, InvalidRef,
    Vault vlt;
    GENERATE_SET(vrs);
    VaultRecordRef vrf;

    TEST_ASSERT(vrs.RemoveRecord(vrf) == false);
)

TEST_BODY(RemoveRecordByRef, Default,
    Vault vlt;
    VaultRecordSet vrs;
    VaultRecordRef vrf;

    vlt.AddKey("A", 0);
    vlt.CreateRecord({});
    vlt.CreateRecord({});

    vlt.RequestEqual("A", 0, vrs);
    vrs.GetRecord("A", 0, vrf);

    TEST_ASSERT(vrs.RemoveRecord(vrf) == true);
    COMPARE_VAULT(vrs, {
        {{"A", 0}}
    });
)

TEST_BODY(RemoveRecordByRef, SecondDeletion,
    Vault vlt;
    VaultRecordSet vrs;
    VaultRecordRef vrf;

    vlt.AddKey("A", 0);
    vlt.CreateRecord({});
    vlt.CreateRecord({});

    vlt.RequestEqual("A", 0, vrs);
    vrs.GetRecord("A", 0, vrf);

    TEST_ASSERT(vrs.RemoveRecord(vrf) == true);
    TEST_ASSERT(vrs.RemoveRecord(vrf) == false);

    COMPARE_VAULT(vrs, {
        {{"A", 0}}
    });
)

TEST_BODY(RemoveRecordByKeyAndValue, InvalidSet,
    VaultRecordSet vrs;
    VaultOperationResult vor;

    vor = vrs.RemoveRecord("A", 0);

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::ParentVaultNotValid, SavedType == typeid(void));
)

TEST_BODY(RemoveRecordByKeyAndValue, Default,
    Vault vlt;
    VaultRecordSet vrs;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.CreateRecord({});
    vlt.CreateRecord({});

    vlt.RequestEqual("A", 0, vrs);

    vor = vrs.RemoveRecord("A", 0);

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));

    COMPARE_VAULT(vrs, {
        {{"A", 0}}
    });
)

TEST_BODY(RemoveRecordByKeyAndValue, SecondDeletion,
    Vault vlt;
    VaultRecordSet vrs;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.CreateRecord({});
    vlt.CreateRecord({});

    vlt.RequestEqual("A", 0, vrs);

    vor = vrs.RemoveRecord("A", 0);

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));

    vrs.RemoveRecord("A", 0);

    COMPARE_VAULT(vrs, {});
)

TEST_BODY(RemoveRecordByKeyAndValue, WrongKey,
    Vault vlt;
    VaultRecordSet vrs;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.CreateRecord({});
    vlt.CreateRecord({});

    vlt.RequestEqual("A", 0, vrs);

    vor = vrs.RemoveRecord("B", 0);

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "B", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongKey, SavedType == typeid(void));

    COMPARE_VAULT(vrs, {
        {{"A", 0}},
        {{"A", 0}}
    });
)

TEST_BODY(RemoveRecordByKeyAndValue, WrongType,
    Vault vlt;
    VaultRecordSet vrs;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.CreateRecord({});
    vlt.CreateRecord({});

    vlt.RequestEqual("A", 0, vrs);

    vor = vrs.RemoveRecord("A", std::string());

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(std::string), ResultCode == VaultOperationResultCode::WrongType, SavedType == typeid(int));

    COMPARE_VAULT(vrs, {
        {{"A", 0}},
        {{"A", 0}}
    });
)

TEST_BODY(RemoveRecordByKeyAndValue, WrongValue,
    Vault vlt;
    VaultRecordSet vrs;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.CreateRecord({});
    vlt.CreateRecord({});

    vlt.RequestEqual("A", 0, vrs);

    vor = vrs.RemoveRecord("A", 1);

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongValue, SavedType == typeid(int));

    COMPARE_VAULT(vrs, {
        {{"A", 0}},
        {{"A", 0}}
    });
)

TEST_BODY(RemoveRecords, InvalidSet,
    VaultRecordSet vrs;
    VaultOperationResult vor;

    vor = vrs.RemoveRecords("A", 0);

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::ParentVaultNotValid, SavedType == typeid(void));
)

TEST_BODY(RemoveRecords, Default,
    Vault vlt;
    VaultRecordSet vrs;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.CreateRecord({});
    vlt.CreateRecord({});

    vlt.RequestEqual("A", 0, vrs);

    vor = vrs.RemoveRecords("A", 0);

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));

    COMPARE_VAULT(vrs, {});
)

TEST_BODY(RemoveRecords, DeleteNotAll,
    Vault vlt;
    VaultRecordSet vrs;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.CreateRecord({});
    vlt.CreateRecord({});

    vlt.RequestEqual("A", 0, vrs);

    vor = vrs.RemoveRecords("A", 0, 1);

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));

    COMPARE_VAULT(vrs, {
        {{"A", 0}}
    });
)

TEST_BODY(RemoveRecords, WrongKey,
    Vault vlt;
    VaultRecordSet vrs;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.CreateRecord({});
    vlt.CreateRecord({});

    vlt.RequestEqual("A", 0, vrs);

    vor = vrs.RemoveRecords("B", 0);

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "B", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongKey, SavedType == typeid(void));

    COMPARE_VAULT(vrs, {
        {{"A", 0}},
        {{"A", 0}}
    });
)

TEST_BODY(RemoveRecords, WrongType,
    Vault vlt;
    VaultRecordSet vrs;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.CreateRecord({});
    vlt.CreateRecord({});

    vlt.RequestEqual("A", 0, vrs);

    vor = vrs.RemoveRecords("A", std::string());

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(std::string), ResultCode == VaultOperationResultCode::WrongType, SavedType == typeid(int));

    COMPARE_VAULT(vrs, {
        {{"A", 0}},
        {{"A", 0}}
    });
)

TEST_BODY(RemoveRecords, WrongValue,
    Vault vlt;
    VaultRecordSet vrs;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.CreateRecord({});
    vlt.CreateRecord({});

    vlt.RequestEqual("A", 0, vrs);

    vor = vrs.RemoveRecords("A", 1);

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongValue, SavedType == typeid(int));

    COMPARE_VAULT(vrs, {
        {{"A", 0}},
        {{"A", 0}}
    });
)

TEST_BODY(Size, Invalid,
    VaultRecordSet vrs;

    TEST_ASSERT(vrs.Size() == 0);
)

TEST_BODY(Size, Empty,
    Vault vlt;
    GENERATE_SET(vrs);

    TEST_ASSERT(vrs.Size() == 0);
)

TEST_BODY(Size, Default,
    Vault vlt;
    GENERATE_SET(vrs);
    VaultRecordRef vrf;

    vlt.AddKey("A", 0);
    vlt.CreateRecord(vrf, {});
    vrs.AddRecord(vrf);

    TEST_ASSERT(vrs.Size() == 1);
)

TEST_BODY(GetKeys, Invalid,
    VaultRecordSet vrs;
    TEST_ASSERT(vrs.GetKeys() == std::vector<std::string>{});
)

TEST_BODY(GetKeys, Keys,
    Vault vlt;
    GENERATE_SET(vrs);

    TEST_ASSERT(vrs.GetKeys() == std::vector<std::string>{});

    vlt.AddKey("A", 0);
    TEST_ASSERT(vrs.GetKeys() == std::vector<std::string>{"A"});

    vlt.AddKey("B", 0);
    TEST_ASSERT(vrs.GetKeys() == std::vector<std::string>({"A", "B"}));

    vlt.AddUniqueKey<std::string>("C");
    TEST_ASSERT(vrs.GetKeys() == std::vector<std::string>({"A", "B", "C"}));
)

TEST_BODY(GetUniqueKeys, Invalid,
    VaultRecordSet vrs;
    TEST_ASSERT(vrs.GetUniqueKeys() == std::vector<std::string>{});
)

TEST_BODY(GetUniqueKeys, UniqueKeys,
    Vault vlt;
    GENERATE_SET(vrs);

    TEST_ASSERT(vrs.GetUniqueKeys() == std::vector<std::string>{});

    vlt.AddKey("A", 0);
    TEST_ASSERT(vrs.GetUniqueKeys() == std::vector<std::string>{});

    vlt.AddUniqueKey<int>("B");
    TEST_ASSERT(vrs.GetUniqueKeys() == std::vector<std::string>{"B"});

    vlt.AddUniqueKey<std::string>("C");
    TEST_ASSERT(vrs.GetUniqueKeys() == std::vector<std::string>({"B", "C"}));
)

TEST_BODY(GetSortedRecords, Invalid,
    VaultRecordSet vrs;
    std::vector<VaultRecordRef> refs = vrs.GetSortedRecords("A");
    TEST_ASSERT(refs.size() == 0);
)

TEST_BODY(GetSortedRecords, GetRecords,
    Vault vlt;
    GENERATE_SET(vrs);
    VaultRecordRef vrf;
    std::vector<VaultRecordRef> refs;

    vlt.AddKey("A", 0);

    refs = vrs.GetSortedRecords("A");

    TEST_ASSERT(refs.size() == 0);


    for (int i = 0; i < 1000; ++i) 
    {
        vlt.CreateRecord(vrf, {{"A", i}});
        vrs.AddRecord(vrf);
    }

    refs = vrs.GetSortedRecords("A");

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
    GENERATE_SET(vrs);
    VaultRecordRef vrf;
    std::vector<VaultRecordRef> refs;

    vlt.AddKey("A", 0);

    for (int i = 0; i < 1000; ++i) 
    {
        vlt.CreateRecord(vrf, {{"A", i}});
        vrs.AddRecord(vrf);
    }

    refs = vrs.GetSortedRecords("A", true);

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
    GENERATE_SET(vrs);
    VaultRecordRef vrf;
    std::vector<VaultRecordRef> refs;

    vlt.AddKey("A", 0);

    for (int i = 0; i < 1000; ++i) 
    {
        vlt.CreateRecord(vrf, {{"A", i}});
        vrs.AddRecord(vrf);
    }

    refs = vrs.GetSortedRecords("A", false, 100);

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
    GENERATE_SET(vrs);
    VaultRecordRef vrf;
    std::vector<VaultRecordRef> refs;

    vlt.AddKey("A", 0);

    for (int i = 0; i < 1000; ++i) 
    {
        vlt.CreateRecord(vrf, {{"A", i}});
        vrs.AddRecord(vrf);
    }

    refs = vrs.GetSortedRecords("Z");

    TEST_ASSERT(refs.size() == 0);
)

TEST_BODY(SortBy, Invalid,
    VaultRecordSet vrs;
    std::size_t counter = 0;
    vrs.SortBy("A", [&](const VaultRecordRef& vrf) -> bool { ++counter; return true;});
    TEST_ASSERT(counter == 0);
)

TEST_BODY(SortBy, Sort,
    Vault vlt;
    GENERATE_SET(vrs);
    VaultRecordRef vrf;

    vlt.AddKey("A", 0);

    for (int i = 0; i < 1000; ++i) 
    {
        vlt.CreateRecord(vrf, {{"A", i}});
        vrs.AddRecord(vrf);
    }

    int counter = 0;
    vrs.SortBy("A", [&](const VaultRecordRef& ref) -> bool
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
    GENERATE_SET(vrs);
    VaultRecordRef vrf;

    vlt.AddKey("A", 0);

    for (int i = 0; i < 1000; ++i) 
    {
        vlt.CreateRecord(vrf, {{"A", i}});
        vrs.AddRecord(vrf);
    }

    int counter = 999;
    vrs.SortBy("A", [&](const VaultRecordRef& ref) -> bool
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
    GENERATE_SET(vrs);
    VaultRecordRef vrf;

    vlt.AddKey("A", 0);

    for (int i = 0; i < 1000; ++i) 
    {
        vlt.CreateRecord(vrf, {{"A", i}});
        vrs.AddRecord(vrf);
    }

    int counter = 0;
    vrs.SortBy("A", [&](const VaultRecordRef& ref) -> bool
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
    GENERATE_SET(vrs);
    VaultRecordRef vrf;

    vlt.AddKey("A", 0);

    for (int i = 0; i < 1000; ++i) 
    {
        vlt.CreateRecord(vrf, {{"A", i}});
        vrs.AddRecord(vrf);
    }

    int counter = 0;
    vrs.SortBy("Z", [&](const VaultRecordRef& ref) -> bool
    {
        int i = 0;
        ref.GetData("A", i);
        TEST_ASSERT(counter == i);
        ++counter;
        return true;
    });

    TEST_ASSERT(counter == 0);
)

TEST_BODY(ToJson, Invalid,
    VaultRecordSet vrs;
    TEST_ASSERT(vrs.ToJson() == "{}");
)

TEST_BODY(ToJson, Default,
    Vault vlt;
    GENERATE_SET(vrs);
    VaultRecordRef vrf;

    vlt.AddUniqueKey<std::size_t>("A");
    vlt.AddKey<std::string>("B", "none");

    vlt.CreateRecord(vrf, { {"A", std::size_t(1)}, {"B", std::string("a")} });
    vrs.AddRecord(vrf);
    vlt.CreateRecord(vrf, { {"A", std::size_t(2)}, {"B", std::string("b")} });
    vrs.AddRecord(vrf);
    vlt.CreateRecord(vrf, { {"A", std::size_t(3)}, {"B", std::string("c")} });
    vrs.AddRecord(vrf);

    std::string res;
    res = vrs.ToJson();

    TEST_ASSERT(res == R"({"Record0":{"A":3,"B":"c"},"Record1":{"A":2,"B":"b"},"Record2":{"A":1,"B":"a"}})");
)

TEST_BODY(ToJson, Format,
    Vault vlt;
    GENERATE_SET(vrs);
    VaultRecordRef vrf;

    vlt.AddUniqueKey<std::size_t>("A");
    vlt.AddKey<std::string>("B", "none");

    vlt.CreateRecord(vrf, { {"A", std::size_t(1)}, {"B", std::string("a")} });
    vrs.AddRecord(vrf);
    vlt.CreateRecord(vrf, { {"A", std::size_t(2)}, {"B", std::string("b")} });
    vrs.AddRecord(vrf);
    vlt.CreateRecord(vrf, { {"A", std::size_t(3)}, {"B", std::string("c")} });
    vrs.AddRecord(vrf);

    std::string res;
    res = vrs.ToJson(true);

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
    GENERATE_SET(vrs);
    VaultRecordRef vrf;

    vlt.AddUniqueKey<std::size_t>("A");
    vlt.AddKey<std::string>("B", "none");

    vlt.CreateRecord(vrf, { {"A", std::size_t(1)}, {"B", std::string("a")} });
    vrs.AddRecord(vrf);
    vlt.CreateRecord(vrf, { {"A", std::size_t(2)}, {"B", std::string("b")} });
    vrs.AddRecord(vrf);
    vlt.CreateRecord(vrf, { {"A", std::size_t(3)}, {"B", std::string("c")} });
    vrs.AddRecord(vrf);

    std::string res;
    res = vrs.ToJson(true, 1);

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
    GENERATE_SET(vrs);
    VaultRecordRef vrf;

    vlt.AddUniqueKey<std::size_t>("A");
    vlt.AddKey<std::string>("B", "none");

    vlt.CreateRecord(vrf, { {"A", std::size_t(1)}, {"B", std::string("a")} });
    vrs.AddRecord(vrf);
    vlt.CreateRecord(vrf, { {"A", std::size_t(2)}, {"B", std::string("b")} });
    vrs.AddRecord(vrf);
    vlt.CreateRecord(vrf, { {"A", std::size_t(3)}, {"B", std::string("c")} });
    vrs.AddRecord(vrf);


    std::string res;
    res = vrs.ToJson(true, 2, true, "Rec");

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
    GENERATE_SET(vrs);
    VaultRecordRef vrf;

    vlt.AddUniqueKey<std::size_t>("A");
    vlt.AddKey<std::string>("B", "none");

    vlt.CreateRecord(vrf, { {"A", std::size_t(1)}, {"B", std::string("a")} });
    vrs.AddRecord(vrf);
    vlt.CreateRecord(vrf, { {"A", std::size_t(2)}, {"B", std::string("b")} });
    vrs.AddRecord(vrf);
    vlt.CreateRecord(vrf, { {"A", std::size_t(3)}, {"B", std::string("c")} });
    vrs.AddRecord(vrf);

    std::string res;
    res = vrs.ToJson(false, 2, true, "Record", true);

    TEST_ASSERT(res == R"([{"A":3,"B":"c"},{"A":2,"B":"b"},{"A":1,"B":"a"}])");
)

TEST_BODY(ToJson, ArrayFormat,
    Vault vlt;
    GENERATE_SET(vrs);
    VaultRecordRef vrf;

    vlt.AddUniqueKey<std::size_t>("A");
    vlt.AddKey<std::string>("B", "none");

    vlt.CreateRecord(vrf, { {"A", std::size_t(1)}, {"B", std::string("a")} });
    vrs.AddRecord(vrf);
    vlt.CreateRecord(vrf, { {"A", std::size_t(2)}, {"B", std::string("b")} });
    vrs.AddRecord(vrf);
    vlt.CreateRecord(vrf, { {"A", std::size_t(3)}, {"B", std::string("c")} });
    vrs.AddRecord(vrf);

    std::string res;
    res = vrs.ToJson(true, 2, true, "Record", true);

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
    GENERATE_SET(vrs);
    VaultRecordRef vrf;

    vlt.AddUniqueKey<std::size_t>("A");
    vlt.AddKey<std::string>("B", "none");

    vlt.CreateRecord(vrf, { {"A", std::size_t(1)}, {"B", std::string("a")} });
    vrs.AddRecord(vrf);
    vlt.CreateRecord(vrf, { {"A", std::size_t(2)}, {"B", std::string("b")} });
    vrs.AddRecord(vrf);
    vlt.CreateRecord(vrf, { {"A", std::size_t(3)}, {"B", std::string("c")} });
    vrs.AddRecord(vrf);

    std::string res;
    res = vrs.ToJson(true, 1, true, "Record", true);

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
    GENERATE_SET(vrs);
    VaultRecordRef vrf;

    vlt.AddUniqueKey<std::size_t>("A");
    vlt.AddKey<std::string>("B", "none");

    vlt.CreateRecord(vrf, { {"A", std::size_t(1)}, {"B", std::string("a")} });
    vrs.AddRecord(vrf);
    vlt.CreateRecord(vrf, { {"A", std::size_t(2)}, {"B", std::string("b")} });
    vrs.AddRecord(vrf);
    vlt.CreateRecord(vrf, { {"A", std::size_t(3)}, {"B", std::string("c")} });
    vrs.AddRecord(vrf);

    std::string res;
    res = vrs.ToJson(true, 1, false);
)

TEST_BODY(ToStrings, Invalid,
    VaultRecordSet vrs;
    TEST_ASSERT(vrs.ToStrings() == decltype(vrs.ToStrings())());
)

TEST_BODY(ToStrings, EmptySet,
    Vault vlt;
    GENERATE_SET(vrs);

    TEST_ASSERT(vrs.ToStrings() == decltype(vrs.ToStrings())());
)

TEST_BODY(ToStrings, KeysWithoutRecords,
    Vault vlt;
    GENERATE_SET(vrs);

    vlt.AddUniqueKey<int>("A");
    vlt.AddKey<std::string>("B", "none");

    TEST_ASSERT(vrs.ToStrings() == decltype(vrs.ToStrings())());
)

TEST_BODY(ToStrings, FilledVault,
    Vault vlt;
    GENERATE_SET(vrs);
    VaultRecordRef vrf;

    vlt.AddUniqueKey<int>("A");
    vlt.AddKey<std::string>("B", "none");

    vlt.CreateRecord(vrf, { {"A", 1}, {"B", std::string("a")} });
    vrs.AddRecord(vrf);
    vlt.CreateRecord(vrf, { {"A", 2}, {"B", std::string("b")} });
    vrs.AddRecord(vrf);
    vlt.CreateRecord(vrf, { {"A", 3}, {"B", std::string("c")} });
    vrs.AddRecord(vrf);

    // Compare vault use ToStrings
    COMPARE_VAULT(vrs, {
        {{"A", 1}, {"B", std::string("a")}},
        {{"A", 2}, {"B", std::string("b")}},
        {{"A", 3}, {"B", std::string("c")}},
    });
)

TEST_BODY(Print, Invalid, 
    VaultRecordSet vrs;
    
    TEST_COUT(vrs.Print(), R"{{{(The parent Vault is not valid!
){{{");
)

TEST_BODY(Print, Empty,
    Vault vlt;
    GENERATE_SET(vrs);

    TEST_COUT(vrs.Print(), R"{{{(VaultRecordSet does not contain keys!
 (0 records)
){{{");
)

TEST_BODY(Print, KeysWithoutRecords,
    Vault vlt;
    GENERATE_SET(vrs);

    vlt.AddUniqueKey<int>("A");
    vlt.AddKey<std::string>("B", "none");

    TEST_COUT(vrs.Print(), R"{{{(+---+---+
| A | B |
+---+---+
+---+---+
 (0 records)
){{{");
)

TEST_BODY(Print, FilledVaultWithDefaultArgs,
    Vault vlt;
    GENERATE_SET(vrs);
    VaultRecordRef vrf;

    vlt.AddUniqueKey<int>("A");
    vlt.AddKey<std::string>("B", "none");

    vlt.CreateRecord(vrf, { {"A", 1}, {"B", std::string("a")} });
    vrs.AddRecord(vrf);
    vlt.CreateRecord(vrf, { {"A", 2}, {"B", std::string("b")} });
    vrs.AddRecord(vrf);
    vlt.CreateRecord(vrf, { {"A", 3}, {"B", std::string("c")} });
    vrs.AddRecord(vrf);

    TEST_COUT(vrs.Print(), R"{{{(+---+---+
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
    GENERATE_SET(vrs);
    VaultRecordRef vrf;

    vlt.AddUniqueKey<int>("A");
    vlt.AddKey<std::string>("B", "none");

    vlt.CreateRecord(vrf, { {"A", 1}, {"B", std::string("a")} });
    vrs.AddRecord(vrf);
    vlt.CreateRecord(vrf, { {"A", 2}, {"B", std::string("b")} });
    vrs.AddRecord(vrf);
    vlt.CreateRecord(vrf, { {"A", 3}, {"B", std::string("c")} });
    vrs.AddRecord(vrf);

    TEST_COUT(vrs.Print(false, 2), R"{{{(+---+---+
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
    GENERATE_SET(vrs);
    VaultRecordRef vrf;

    vlt.AddUniqueKey<int>("A");
    vlt.AddKey<std::string>("B", "none");

    vlt.CreateRecord(vrf, { {"A", 1}, {"B", std::string("c")} });
    vrs.AddRecord(vrf);
    vlt.CreateRecord(vrf, { {"A", 2}, {"B", std::string("b")} });
    vrs.AddRecord(vrf);
    vlt.CreateRecord(vrf, { {"A", 3}, {"B", std::string("a")} });
    vrs.AddRecord(vrf);

    TEST_COUT(vrs.Print(false, 2, "B"), R"{{{(+---+---+
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
    GENERATE_SET(vrs);
    VaultRecordRef vrf;

    vlt.AddUniqueKey<int>("A");
    vlt.AddKey<std::string>("B", "none");

    vlt.CreateRecord(vrf, { {"A", 1}, {"B", std::string("a")} });
    vrs.AddRecord(vrf);
    vlt.CreateRecord(vrf, { {"A", 2}, {"B", std::string("b")} });
    vrs.AddRecord(vrf);
    vlt.CreateRecord(vrf, { {"A", 3}, {"B", std::string("c")} });
    vrs.AddRecord(vrf);

    TEST_COUT(vrs.Print(false, 2, "B", true), R"{{{(+---+---+
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
    GENERATE_SET(vrs);
    VaultRecordRef vrf;

    vlt.AddUniqueKey<int>("A");
    vlt.AddKey<std::string>("B", "none");
    vlt.AddKey("C", false);

    vlt.CreateRecord(vrf, { {"A", 1}, {"B", std::string("a")}, {"C", true} });
    vrs.AddRecord(vrf);
    vlt.CreateRecord(vrf, { {"A", 2}, {"B", std::string("b")}, {"C", false} });
    vrs.AddRecord(vrf);
    vlt.CreateRecord(vrf, { {"A", 3}, {"B", std::string("c")}, {"C", true} });
    vrs.AddRecord(vrf);

    TEST_COUT(vrs.Print(false, 2, "B", true, {"C", "B"}), R"{{{(+-------+---+
| C     | B |
+-------+---+
| true  | c |
| false | b |
$~~~~~~~$~~~$
 (3 records)
){{{");
)

TEST_BODY(Print, PrintId,
    Vault vlt;
    GENERATE_SET(vrs);
    VaultRecordRef vrf;

    vlt.AddUniqueKey<int>("A");
    vlt.AddKey<std::string>("B", "none");

    vlt.CreateRecord(vrf, { {"A", 1}, {"B", std::string("a")} });
    vrs.AddRecord(vrf);
    vlt.CreateRecord(vrf, { {"A", 2}, {"B", std::string("b")} });
    vrs.AddRecord(vrf);
    vlt.CreateRecord(vrf, { {"A", 3}, {"B", std::string("c")} });
    vrs.AddRecord(vrf);

    SUPPRESS_COUT(vrs.Print(true));
)

TEST_BODY(Join, JoinInvalidToInvalid,
    VaultRecordSet vrs1, vrs2;
    VaultOperationResult vor;

    vor = vrs1.Join(vrs2);

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "", 
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::ParentVaultNotValid, SavedType == typeid(void));    
)

TEST_BODY(Join, JoinValidToInvalid,
    Vault vlt;
    VaultRecordSet vrs1;
    GENERATE_SET(vrs2);
    VaultOperationResult vor;

    vor = vrs1.Join(vrs2);

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "", 
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::ParentVaultNotValid, SavedType == typeid(void));    
)

TEST_BODY(Join, JoinInvalidToValid,
    Vault vlt;
    VaultRecordSet vrs1;
    GENERATE_SET(vrs2);
    VaultOperationResult vor;

    vor = vrs2.Join(vrs1);

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "", 
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::OtherParentVaultNotValid, SavedType == typeid(void));    
)

TEST_BODY(Join, JoinFilledToEmpty,
    Vault vlt;
    GENERATE_SET(vrs1);
    GENERATE_SET(vrs2); 
    VaultRecordRef vrf;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);

    vlt.CreateRecord(vrf, {{}});
    vrs1.AddRecord(vrf);
    vlt.CreateRecord(vrf, {{}});
    vrs1.AddRecord(vrf);

    vor = vrs2.Join(vrs1);

    COMPARE_VAULT(vrs2, {
        {{"A", 0}},
        {{"A", 0}}
    });

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "", 
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(void));
)

TEST_BODY(Join, JoinEmptyToFilled,
    Vault vlt;
    GENERATE_SET(vrs1);
    GENERATE_SET(vrs2); 
    VaultRecordRef vrf;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);

    vlt.CreateRecord(vrf, {{}});
    vrs1.AddRecord(vrf);
    vlt.CreateRecord(vrf, {{}});
    vrs1.AddRecord(vrf);

    vor = vrs1.Join(vrs2);

    COMPARE_VAULT(vrs1, {
        {{"A", 0}},
        {{"A", 0}}
    });

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "", 
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(void));
)

TEST_BODY(Join, JoinFilled,
    Vault vlt;
    GENERATE_SET(vrs1);
    GENERATE_SET(vrs2); 
    VaultRecordRef vrf;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);

    vlt.CreateRecord({{"A", -1}});
    vlt.CreateRecord(vrf, {{"A", 1}});
    vrs1.AddRecord(vrf);
    vlt.CreateRecord(vrf, {{"A", 2}});
    vrs2.AddRecord(vrf);
    vlt.CreateRecord(vrf, {{"A", 3}});
    vrs1.AddRecord(vrf);
    vrs2.AddRecord(vrf);

    vor = vrs1.Join(vrs2);

    COMPARE_VAULT(vrs1, {
        {{"A", 1}},
        {{"A", 2}},
        {{"A", 3}}
    });

    TEST_ASSERT(vrs1.Size() == 3);

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "", 
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(void));
)

TEST_BODY(Join, WrongParentVault,
    Vault vlt1, vlt2;
    VaultRecordSet vrs1, vrs2;

    VaultRecordRef vrf;
    VaultOperationResult vor;

    vlt1.AddKey("A", 0);
    vlt2.AddKey("A", 0);

    vlt1.CreateRecord(vrf, {{"A", 1}});
    vlt1.RequestEqual("A", 1, vrs1);
    vlt2.CreateRecord(vrf, {{"A", 1}});
    vlt2.RequestEqual("A", 1, vrs2);

    vor = vrs1.Join(vrs2);

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "", 
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::ParentVaultNotMatch, SavedType == typeid(void));
)

TEST_BODY(Join, Self,
    Vault vlt;
    GENERATE_SET(vrs);
    VaultOperationResult vor;

    vor = vrs.Join(vrs);

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "", 
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::SameVaultRecordSet, SavedType == typeid(void));
)

TEST_BODY(Exclude, ExcludeInvalidFromInvalid, 
    VaultRecordSet vrs1, vrs2;
    VaultOperationResult vor;

    vor = vrs1.Exclude(vrs2);
    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "", 
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::ParentVaultNotValid, SavedType == typeid(void));
)

TEST_BODY(Exclude, ExcludeValidFromInvalid, 
    Vault vlt;
    VaultRecordSet vrs1;
    GENERATE_SET(vrs2);
    VaultOperationResult vor;

    vor = vrs1.Exclude(vrs2);
    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "", 
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::ParentVaultNotValid, SavedType == typeid(void));
)

TEST_BODY(Exclude, ExcludeInvalidFromValid, 
    Vault vlt;
    GENERATE_SET (vrs1);
    VaultRecordSet vrs2;
    VaultOperationResult vor;

    vor = vrs1.Exclude(vrs2);
    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "", 
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::OtherParentVaultNotValid, SavedType == typeid(void));
)

TEST_BODY(Exclude, ExcludeFilledFromEmpty, 
    Vault vlt;
    GENERATE_SET(vrs1);
    GENERATE_SET(vrs2); 
    VaultRecordRef vrf;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);

    vlt.CreateRecord(vrf, {{}});
    vrs1.AddRecord(vrf);
    vlt.CreateRecord(vrf, {{}});
    vrs1.AddRecord(vrf);

    vor = vrs2.Exclude(vrs1);

    TEST_ASSERT(vrs2.Size() == 0);

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "", 
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(void));
)

TEST_BODY(Exclude, ExcludeEmptyFromFilled, 
    Vault vlt;
    GENERATE_SET(vrs1);
    GENERATE_SET(vrs2); 
    VaultRecordRef vrf;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);

    vlt.CreateRecord(vrf, {{}});
    vrs1.AddRecord(vrf);
    vlt.CreateRecord(vrf, {{}});
    vrs1.AddRecord(vrf);

    vor = vrs1.Exclude(vrs2);

    TEST_ASSERT(vrs1.Size() == 2);

    COMPARE_VAULT(vrs1, {
        {{"A", 0}},
        {{"A", 0}}
    })

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "", 
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(void));
)

TEST_BODY(Exclude, ExcludeFilled, 
    Vault vlt;
    GENERATE_SET(vrs1);
    GENERATE_SET(vrs2); 
    VaultRecordRef vrf;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);

    vlt.CreateRecord(vrf, {{"A", 1}});
    vrs1.AddRecord(vrf);
    vlt.CreateRecord(vrf, {{"A", 2}});
    vrs1.AddRecord(vrf);
    vrs2.AddRecord(vrf);

    vor = vrs1.Exclude(vrs2);

    COMPARE_VAULT(vrs1, {
        {{"A", 1}}
    });

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "", 
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(void));
)

TEST_BODY(Exclude, Self,
    Vault vlt;
    GENERATE_SET(vrs);
    VaultOperationResult vor;

    vor = vrs.Exclude(vrs);

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "", 
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::SameVaultRecordSet, SavedType == typeid(void));
)

TEST_BODY(Exclude, ExcludeDifferentParrent, 
    Vault vlt1, vlt2;
    VaultRecordSet vrs1, vrs2;
    VaultRecordRef vrf;
    VaultOperationResult vor;

    vlt1.AddKey("A", 0);
    vlt2.AddKey("A", 0);

    vlt1.CreateRecord({{"A", 1}});
    vlt1.RequestEqual("A", 1, vrs1);

    vlt2.CreateRecord({{"A", 1}});
    vlt2.RequestEqual("A", 1, vrs2);

    vor = vrs1.Exclude(vrs2);

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "", 
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::ParentVaultNotMatch, SavedType == typeid(void));
    
    TEST_ASSERT(vrs1.Size());
)

TEST_BODY(Intersect, IntersectInvalidWithInvalid, 
    VaultRecordSet vrs1, vrs2;
    VaultOperationResult vor;

    vor = vrs1.Intersect(vrs2);

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "", 
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::ParentVaultNotValid, SavedType == typeid(void));
)

TEST_BODY(Intersect, IntersectInvalidWithValid,
    Vault vlt;
    VaultRecordSet vrs1;
    GENERATE_SET(vrs2);
    VaultOperationResult vor;

    vor = vrs1.Intersect(vrs2);

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "", 
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::ParentVaultNotValid, SavedType == typeid(void));
)

TEST_BODY(Intersect, IntersectValidWithInvalid,
    Vault vlt;
    GENERATE_SET(vrs1);
    VaultRecordSet vrs2;
    VaultOperationResult vor;

    vor = vrs1.Intersect(vrs2);

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "", 
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::OtherParentVaultNotValid, SavedType == typeid(void));
)

TEST_BODY(Intersect, IntersectFilledFromEmpty,
    Vault vlt;
    GENERATE_SET(vrs1);
    GENERATE_SET(vrs2); 
    VaultRecordRef vrf;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);

    vlt.CreateRecord(vrf, {{}});
    vrs1.AddRecord(vrf);
    vlt.CreateRecord(vrf, {{}});
    vrs1.AddRecord(vrf);

    vor = vrs2.Exclude(vrs1);

    TEST_ASSERT(vrs2.Size() == 0);

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "", 
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(void));
)

TEST_BODY(Intersect, IntersectEmptyFromFilled,
    Vault vlt;
    GENERATE_SET(vrs1);
    GENERATE_SET(vrs2); 
    VaultRecordRef vrf;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);

    vlt.CreateRecord(vrf, {{}});
    vrs1.AddRecord(vrf);
    vlt.CreateRecord(vrf, {{}});
    vrs1.AddRecord(vrf);

    vor = vrs1.Exclude(vrs2);

    TEST_ASSERT(vrs1.Size() == 2);

    COMPARE_VAULT(vrs1, {
        {{"A", 0}},
        {{"A", 0}}
    })

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "", 
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(void));
)

TEST_BODY(Intersect, IntersectFilled, 
    Vault vlt;
    GENERATE_SET(vrs1);
    GENERATE_SET(vrs2); 
    VaultRecordRef vrf;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);

    vlt.CreateRecord(vrf, {{"A", 1}});
    vrs1.AddRecord(vrf);
    vlt.CreateRecord(vrf, {{"A", 2}});
    vrs1.AddRecord(vrf);
    vrs2.AddRecord(vrf);

    vor = vrs1.Intersect(vrs2);

    COMPARE_VAULT(vrs1, {
        {{"A", 2}}
    });

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "", 
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(void));
)

TEST_BODY(Intersect, Self,
    Vault vlt;
    GENERATE_SET(vrs);
    VaultOperationResult vor;

    vor = vrs.Intersect(vrs);

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "", 
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::SameVaultRecordSet, SavedType == typeid(void));
)

TEST_BODY(Intersect, ExcludeDifferentParrent, 
    Vault vlt1, vlt2;
    VaultRecordSet vrs1, vrs2;
    VaultRecordRef vrf;
    VaultOperationResult vor;

    vlt1.AddKey("A", 0);
    vlt2.AddKey("A", 0);

    vlt1.CreateRecord({{"A", 1}});
    vlt1.RequestEqual("A", 1, vrs1);

    vlt2.CreateRecord({{"A", 1}});
    vlt2.RequestEqual("A", 1, vrs2);

    vor = vrs1.Intersect(vrs2);

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "", 
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::ParentVaultNotMatch, SavedType == typeid(void));
    
    TEST_ASSERT(vrs1.Size());
)

TEST_BODY(SaveToFile, FilledVault,
    Vault vlt;
    GENERATE_SET(vrs);
    VaultRecordRef vrf;

    vlt.AddKey("A", 0);
    vlt.CreateRecord(vrf, {{"A", 0}});
    vrs.AddRecord(vrf);

    std::string fileName = GenTmpFileName(std::string("MVault_") + "Vault_" + __FUNCTION__ + "_");
    bool res = false;

    res = vrs.SaveToFile(fileName);

    TEST_ASSERT(res == true);

    COMPARE_FILE(fileName, true, "A\r\n"
                                 "0\r\n");
)

TEST_BODY(SaveToFile, ReverseNotAllKeys,
    Vault vlt;
    GENERATE_SET(vrs);
    VaultRecordRef vrf;

    vlt.AddKey("A", 0);
    vlt.AddKey("B", 0);
    vlt.AddUniqueKey<int>("C");

    vlt.CreateRecord(vrf, {{"A", 0}, {"B", 1}, {"C", 2}});
    vrs.AddRecord(vrf);

    std::string fileName = GenTmpFileName(std::string("MVault_") + "Vault_" + __FUNCTION__ + "_");
    bool res = false;

    res = vrs.SaveToFile(fileName, {"C", "A"});

    TEST_ASSERT(res == true);

    COMPARE_FILE(fileName, true, "C,A\r\n"
                                 "2,0\r\n");
)

TEST_BODY(SaveToFile, Separator,
    Vault vlt;
    GENERATE_SET(vrs);
    VaultRecordRef vrf;

    vlt.AddKey("A", 0);
    vlt.AddKey("B", 0);
    vlt.AddUniqueKey<int>("C");

    vlt.CreateRecord(vrf, {{"A", 0}, {"B", 1}, {"C", 2}});
    vrs.AddRecord(vrf);

    std::string fileName = GenTmpFileName(std::string("MVault_") + "Vault_" + __FUNCTION__ + "_");
    bool res = false;

    res = vrs.SaveToFile(fileName, {}, ";");

    TEST_ASSERT(res == true);

    COMPARE_FILE(fileName, true, "A;B;C\r\n"
                                 "0;1;2\r\n");
)

TEST_BODY(SaveToFile, NotSaveKeys,
    Vault vlt;
    GENERATE_SET(vrs);
    VaultRecordRef vrf;

    vlt.AddKey("A", 0);
    vlt.AddKey("B", 0);
    vlt.AddUniqueKey<int>("C");

    vlt.CreateRecord(vrf, {{"A", 0}, {"B", 1}, {"C", 2}});
    vrs.AddRecord(vrf);

    std::string fileName = GenTmpFileName(std::string("MVault_") + "Vault_" + __FUNCTION__ + "_");
    bool res = false;

    res = vrs.SaveToFile(fileName, {}, ",", false);

    TEST_ASSERT(res == true);

    COMPARE_FILE(fileName, true, "0,1,2\r\n");
)

TEST_BODY(SaveToFile, FailedToOpenFile,
    Vault vlt;
    GENERATE_SET(vrs);
    VaultRecordRef vrf;

    vlt.AddKey("A", 0);
    vlt.AddKey("B", 0);
    vlt.AddUniqueKey<int>("C");

    vlt.CreateRecord(vrf, {{"A", 0}, {"B", 1}, {"C", 2}});
    vrs.AddRecord(vrf);

    TEST_ASSERT(vrs.SaveToFile("") == false);
)

TEST_BODY(Destructor, Default,
    Vault vlt;
    VaultRecordSet* vrs = new VaultRecordSet;
    VaultRecordRef vrf;

    vlt.AddKey("A", 0);
    vlt.CreateRecord(vrf, {{"A", 1}});
    vrs->AddRecord(vrf);
    vrs->GetRecord("A", 1, vrf);

    VaultRecordSet vrs2(*vrs);
    delete vrs;
)

TEST_BODY(CompareOperator, InvalidWithInvalid,
    VaultRecordSet vrs1, vrs2;
    TEST_ASSERT((vrs1 == vrs2) == true);
)

TEST_BODY(CompareOperator, InvalidWithValid,
    Vault vlt;
    GENERATE_SET(vrs1);
    VaultRecordSet vrs2;
    TEST_ASSERT((vrs1 == vrs2) == false);
)

TEST_BODY(CompareOperator, ValidWithInvalid,
    Vault vlt;
    VaultRecordSet vrs1;
    GENERATE_SET(vrs2);
    TEST_ASSERT((vrs1 == vrs2) == false);
)

TEST_BODY(CompareOperator, Empty,
    Vault vlt;
    GENERATE_SET(vrs1);
    GENERATE_SET(vrs2);
    TEST_ASSERT((vrs1 == vrs2) == true);
)

TEST_BODY(CompareOperator, Filled,
    Vault vlt;
    GENERATE_SET(vrs1);
    GENERATE_SET(vrs2);
    VaultRecordRef vrf;

    vlt.AddKey("A", 0);

    vlt.CreateRecord(vrf, {});

    vrs1.AddRecord(vrf);
    vrs2.AddRecord(vrf);

    TEST_ASSERT((vrs1 == vrs2) == true);
)

TEST_BODY(CompareOperator, DifferentData,
    Vault vlt;
    GENERATE_SET(vrs1);
    GENERATE_SET(vrs2);
    VaultRecordRef vrf;

    vlt.AddKey("A", 0);

    vlt.CreateRecord(vrf, {});
    vrs1.AddRecord(vrf);
    vlt.CreateRecord(vrf, {});
    vrs2.AddRecord(vrf);

    TEST_ASSERT((vrs1 == vrs2) == false);
)

TEST_BODY(CompareOperator, DifferentParents,
    Vault vlt1, vlt2;
    VaultRecordSet vrs1, vrs2;
    VaultRecordRef vrf;
    VaultOperationResult vor;

    vlt1.AddKey("A", 0);
    vlt2.AddKey("A", 0);

    vlt1.CreateRecord(vrf, {});
    vlt1.RequestEqual("A", 0, vrs1);
    vlt2.CreateRecord(vrf, {});
    vlt1.RequestEqual("A", 0, vrs2);

    TEST_ASSERT((vrs1 == vrs2) == true);
)


TEST_BODY(NotEquationOperator, InvalidWithInvalid,
    VaultRecordSet vrs1, vrs2;
    TEST_ASSERT((vrs1 != vrs2) == false);
)

TEST_BODY(NotEquationOperator, InvalidWithValid,
    Vault vlt;
    GENERATE_SET(vrs1);
    VaultRecordSet vrs2;
    TEST_ASSERT((vrs1 != vrs2) == true);
)

TEST_BODY(NotEquationOperator, ValidWithInvalid,
    Vault vlt;
    VaultRecordSet vrs1;
    GENERATE_SET(vrs2);
    TEST_ASSERT((vrs1 != vrs2) == true);
)

TEST_BODY(NotEquationOperator, Empty,
    Vault vlt;
    GENERATE_SET(vrs1);
    GENERATE_SET(vrs2);
    TEST_ASSERT((vrs1 != vrs2) == false);
)

TEST_BODY(NotEquationOperator, Filled,
    Vault vlt;
    GENERATE_SET(vrs1);
    GENERATE_SET(vrs2);
    VaultRecordRef vrf;

    vlt.AddKey("A", 0);

    vlt.CreateRecord(vrf, {});

    vrs1.AddRecord(vrf);
    vrs2.AddRecord(vrf);

    TEST_ASSERT((vrs1 != vrs2) == false);
)

TEST_BODY(NotEquationOperator, DifferentData,
    Vault vlt;
    GENERATE_SET(vrs1);
    GENERATE_SET(vrs2);
    VaultRecordRef vrf;

    vlt.AddKey("A", 0);

    vlt.CreateRecord(vrf, {});
    vrs1.AddRecord(vrf);
    vlt.CreateRecord(vrf, {});
    vrs2.AddRecord(vrf);

    TEST_ASSERT((vrs1 != vrs2) == true);
)

TEST_BODY(NotEquationOperator, DifferentParents,
    Vault vlt1, vlt2;
    VaultRecordSet vrs1, vrs2;
    VaultRecordRef vrf;
    VaultOperationResult vor;

    vlt1.AddKey("A", 0);
    vlt2.AddKey("A", 0);

    vlt1.CreateRecord(vrf, {});
    vlt1.RequestEqual("A", 0, vrs1);
    vlt2.CreateRecord(vrf, {});
    vlt1.RequestEqual("A", 0, vrs2);

    TEST_ASSERT((vrs1 != vrs2) == false);
)


TEST_BODY(UnionSets, InvalidWithInvalid,
    VaultRecordSet vrs1, vrs2, vrs3;
    VaultOperationResult vor;

    vor = Union(vrs1, vrs2, vrs3);

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "", 
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::ParentVaultNotValid, SavedType == typeid(void));

    TEST_ASSERT(vrs3.Size() == 0);
)

TEST_BODY(UnionSets, InvalidWithValid,
    Vault vlt;
    VaultRecordSet vrs1, vrs3;
    GENERATE_SET(vrs2);
    VaultOperationResult vor;

    vor = Union(vrs1, vrs2, vrs3);
    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "",
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::ParentVaultNotValid, SavedType == typeid(void));

    TEST_ASSERT(vrs3.Size() == 0);
)

TEST_BODY(UnionSets, ValidWithInvalid,
    Vault vlt;
    GENERATE_SET(vrs1);
    VaultRecordSet vrs2, vrs3;
    VaultOperationResult vor;

    vor = Union(vrs1, vrs2, vrs3);
    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "",
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::OtherParentVaultNotValid, SavedType == typeid(void));

    TEST_ASSERT(vrs3.Size() == 0);
)

TEST_BODY(UnionSets, Empty,
    Vault vlt;
    GENERATE_SET(vrs1);
    GENERATE_SET(vrs2);
    VaultRecordSet vrs3;
    VaultOperationResult vor;

    vor = Union(vrs1, vrs2, vrs3);
    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "",
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(void));

    TEST_ASSERT(vrs3.Size() == 0);
)

TEST_BODY(UnionSets, Filled,
    Vault vlt;
    GENERATE_SET(vrs1);
    GENERATE_SET(vrs2);
    VaultRecordSet vrs3;
    VaultRecordRef vrf;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);

    vlt.CreateRecord({{"A", -1}});
    vlt.CreateRecord(vrf, {{"A", 0}});
    vrs1.AddRecord(vrf);
    vlt.CreateRecord(vrf, {{"A", 1}});
    vrs2.AddRecord(vrf);
    vlt.CreateRecord(vrf, {{"A", 2}});
    vrs1.AddRecord(vrf);
    vrs2.AddRecord(vrf);

    vor = Union(vrs1, vrs2, vrs3);

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "",
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(void));

    COMPARE_VAULT(vrs3, {
        {{"A", 0}},
        {{"A", 1}},
        {{"A", 2}}
    });
    TEST_ASSERT(vrs3.Size() == 3);
)

TEST_BODY(UnionSets, DifferentParents,
    Vault vlt1, vlt2;
    VaultRecordSet vrs1, vrs2, vrs3;
    VaultRecordRef vrf;
    VaultOperationResult vor;

    vlt1.AddKey("A", 0);
    vlt2.AddKey("A", 0);

    vlt1.CreateRecord(vrf, {});
    vlt1.RequestEqual("A", 0, vrs1);
    vlt2.CreateRecord(vrf, {});
    vlt2.RequestEqual("A", 0, vrs2);

    vor = Union(vrs1, vrs2, vrs3);
    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "",
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::ParentVaultNotMatch, SavedType == typeid(void));

    TEST_ASSERT(vrs3.Size() == 0);
)

TEST_BODY(UnionSets, Self,
    Vault vlt;
    GENERATE_SET(vrs1);
    GENERATE_SET(vrs2);
    VaultOperationResult vor;

    vor = Union(vrs1, vrs2, vrs1);
    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "",
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::SameVaultRecordSet, SavedType == typeid(void));

    TEST_ASSERT(vrs1.Size() == 0);

    vor = Union(vrs1, vrs1, vrs2);
    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "",
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::SameVaultRecordSet, SavedType == typeid(void));

    TEST_ASSERT(vrs1.Size() == 0);

    vor = Union(vrs1, vrs1, vrs1);
    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "",
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::SameVaultRecordSet, SavedType == typeid(void));

    TEST_ASSERT(vrs1.Size() == 0);
)

TEST_BODY(IntersectionSets, InvalidWithInvalid,
    VaultRecordSet vrs1, vrs2, vrs3;
    VaultOperationResult vor;

    vor = Intersection(vrs1, vrs2, vrs3);
    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "", 
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::ParentVaultNotValid, SavedType == typeid(void));

    TEST_ASSERT(vrs3.Size() == 0);
)

TEST_BODY(IntersectionSets, InvalidWithValid,
    Vault vlt;
    VaultRecordSet vrs1, vrs3;
    GENERATE_SET(vrs2);
    VaultOperationResult vor;

    vor = Intersection(vrs1, vrs2, vrs3);
    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "",
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::ParentVaultNotValid, SavedType == typeid(void));

    TEST_ASSERT(vrs3.Size() == 0);
)

TEST_BODY(IntersectionSets, ValidWithInvalid,
    Vault vlt;
    GENERATE_SET(vrs1);
    VaultRecordSet vrs2, vrs3;
    VaultOperationResult vor;

    vor = Intersection(vrs1, vrs2, vrs3);
    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "",
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::OtherParentVaultNotValid, SavedType == typeid(void));

    TEST_ASSERT(vrs3.Size() == 0);
)

TEST_BODY(IntersectionSets, Empty,
    Vault vlt;
    GENERATE_SET(vrs1);
    GENERATE_SET(vrs2);
    VaultRecordSet vrs3;
    VaultOperationResult vor;

    vor = Intersection(vrs1, vrs2, vrs3);
    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "",
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(void));

    TEST_ASSERT(vrs3.Size() == 0);
)

TEST_BODY(IntersectionSets, Filled,
    Vault vlt;
    GENERATE_SET(vrs1);
    GENERATE_SET(vrs2);
    VaultRecordSet vrs3;
    VaultRecordRef vrf;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);

    vlt.CreateRecord({{"A", -1}});
    vlt.CreateRecord(vrf, {{"A", 0}});
    vrs1.AddRecord(vrf);
    vlt.CreateRecord(vrf, {{"A", 1}});
    vrs2.AddRecord(vrf);
    vlt.CreateRecord(vrf, {{"A", 2}});
    vrs2.AddRecord(vrf);

    // Empty intersecton
    vor = Intersection(vrs1, vrs2, vrs3);
    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "",
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(void));

    COMPARE_VAULT(vrs3, {});
    TEST_ASSERT(vrs3.Size() == 0);

    
    vlt.CreateRecord(vrf, {{"A", 10}});
    vrs1.AddRecord(vrf);
    vrs2.AddRecord(vrf);

    // Left intersection
    vor = Intersection(vrs1, vrs2, vrs3);
    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "",
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(void));

    COMPARE_VAULT(vrs3, {{{"A", 10}}});
    TEST_ASSERT(vrs3.Size() == 1);

    // Right intersection
    vor = Intersection(vrs2, vrs1, vrs3);
    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "",
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(void));

    COMPARE_VAULT(vrs3, {{{"A", 10}}});
    TEST_ASSERT(vrs3.Size() == 1);
)

TEST_BODY(IntersectionSets, DifferentParents,
    Vault vlt1, vlt2;
    VaultRecordSet vrs1, vrs2, vrs3;
    VaultRecordRef vrf;
    VaultOperationResult vor;

    vlt1.AddKey("A", 0);
    vlt2.AddKey("A", 0);

    vlt1.CreateRecord(vrf, {});
    vlt1.RequestEqual("A", 0, vrs1);
    vlt2.CreateRecord(vrf, {});
    vlt2.RequestEqual("A", 0, vrs2);

    vor = Intersection(vrs1, vrs2, vrs3);

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "",
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::ParentVaultNotMatch, SavedType == typeid(void));

    TEST_ASSERT(vrs3.Size() == 0);
)

TEST_BODY(IntersectionSets, RewriteTarget,
    Vault vlt;
    GENERATE_SET(vrs1);
    GENERATE_SET(vrs2);
    GENERATE_SET(vrs3);
    VaultRecordRef vrf;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);

    vlt.CreateRecord(vrf, {{"A", -1}});
    vrs3.AddRecord(vrf);
    vlt.CreateRecord(vrf, {{"A", 0}});
    vrs1.AddRecord(vrf);
    vlt.CreateRecord(vrf, {{"A", 1}});
    vrs2.AddRecord(vrf);

    vlt.CreateRecord(vrf, {{"A", 10}});
    vrs1.AddRecord(vrf);
    vrs2.AddRecord(vrf);

    vor = Intersection(vrs1, vrs2, vrs3);
    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "",
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(void));

    COMPARE_VAULT(vrs3, {{{"A", 10}}});
    TEST_ASSERT(vrs3.Size() == 1);
)

TEST_BODY(IntersectionSets, Self,
    Vault vlt;
    GENERATE_SET(vrs1);
    GENERATE_SET(vrs2);
    VaultOperationResult vor;

    vor = Intersection(vrs1, vrs2, vrs1);
    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "",
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::SameVaultRecordSet, SavedType == typeid(void));

    TEST_ASSERT(vrs1.Size() == 0);

    vor = Intersection(vrs1, vrs1, vrs2);
    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "",
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::SameVaultRecordSet, SavedType == typeid(void));

    TEST_ASSERT(vrs1.Size() == 0);

    vor = Intersection(vrs1, vrs1, vrs1);
    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "",
        RequestedType == typeid(void), ResultCode == VaultOperationResultCode::SameVaultRecordSet, SavedType == typeid(void));

    TEST_ASSERT(vrs1.Size() == 0);
)
}

void VaultRecordSetUnitTests(const std::string& testName)
{
    DBG_LOG_ENTER();
    SetBackTraceFormat(BackTraceFormat::None);

    for (const auto& test : VaultRecordSetTestNs::Tests)
    {
        if (testName.empty() || testName == test.first)
            test.second();
    }
}