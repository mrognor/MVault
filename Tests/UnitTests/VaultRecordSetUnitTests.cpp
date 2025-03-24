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
}