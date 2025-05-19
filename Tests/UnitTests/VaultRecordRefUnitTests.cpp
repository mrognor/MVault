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

TEST_BODY(GetRecordUniqueId, Invalid,
    VaultRecordRef vrr;

    TEST_ASSERT(vrr.GetRecordUniqueId() == "null");
)

TEST_BODY(GetRecordUniqueId, Valid,
    Vault vlt;
    VaultRecordRef vrr;

    vlt.AddKey("A", 0);
    vlt.CreateRecord({{"A", 0}});
    vlt.GetRecord("A", 0, vrr);

    TEST_ASSERT(vrr.GetRecordUniqueId() != "null");
)

TEST_BODY(SetDataByKeyAndValue, Invalid,
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vor = vrr.SetData("A", 1);
    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::DataRecordNotValid, SavedType == typeid(void));
)

TEST_BODY(SetDataByKeyAndValue, Deleted,
    Vault vlt;
    VaultRecordRef vrr1, vrr2;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.CreateRecord({{"A", 0}});
    vlt.GetRecord("A", 0, vrr1);
    vlt.GetRecord("A", 0, vrr2);

    vlt.EraseRecord(vrr2);

    vor = vrr1.SetData("A", 1);
    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::DataRecordNotValid, SavedType == typeid(void));
)

TEST_BODY(SetDataByKeyAndValue, Default,
    Vault vlt;
    VaultRecordSet vrs1, vrs2;
    VaultRecordRef vrr1, vrr2, vrr3, vrr4;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.CreateRecord({{"A", -10}});
    vlt.GetRecord("A", -10, vrr1);

    // Check set data
    vor = vrr1.SetData("A", 1);
    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));
    
    // Check get record
    vlt.GetRecord("A", 1, vrr2);
    TEST_ASSERT(vrr1 == vrr2);

    // Check hash table request
    vlt.RequestEqual("A", 1, vrs1);
    vrs1.GetRecord("A", 1, vrr3);
    TEST_ASSERT(vrr1 == vrr3);

    // Check map request
    vlt.RequestGreater("A", 0, vrs2);
    vrs2.GetRecord("A", 1, vrr4);
    TEST_ASSERT(vrr1 == vrr4);
)

TEST_BODY(SetDataByKeyAndValue, WrongKey,
    Vault vlt;
    VaultRecordSet vrs1, vrs2;
    VaultRecordRef vrr1, vrr2, vrr3, vrr4;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.CreateRecord({{"A", -10}});
    vlt.GetRecord("A", -10, vrr1);

    // Check set data
    vor = vrr1.SetData("Z", 0);
    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "Z", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongKey, SavedType == typeid(void));

    // Check get record
    vlt.GetRecord("A", -10, vrr2);
    TEST_ASSERT(vrr1 == vrr2);

    // Check hash table request
    vlt.RequestEqual("A", -10, vrs1);
    vrs1.GetRecord("A", -10, vrr3);
    TEST_ASSERT(vrr1 == vrr3);

    // Check map request
    vlt.RequestLess("A", 0, vrs2);
    vrs2.GetRecord("A", -10, vrr4);
    TEST_ASSERT(vrr1 == vrr4);
)

TEST_BODY(SetDataByKeyAndValue, WrongType,
    Vault vlt;
    VaultRecordSet vrs1, vrs2;
    VaultRecordRef vrr1, vrr2, vrr3, vrr4;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.CreateRecord({{"A", -10}});
    vlt.GetRecord("A", -10, vrr1);

    // Check set data
    vor = vrr1.SetData<float>("A", 0.0);
    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(float), ResultCode == VaultOperationResultCode::WrongType, SavedType == typeid(int));

    // Check get record
    vlt.GetRecord("A", -10, vrr2);
    TEST_ASSERT(vrr1 == vrr2);

    // Check hash table request
    vlt.RequestEqual("A", -10, vrs1);
    vrs1.GetRecord("A", -10, vrr3);
    TEST_ASSERT(vrr1 == vrr3);

    // Check map request
    vlt.RequestLess("A", 0, vrs2);
    vrs2.GetRecord("A", -10, vrr4);
    TEST_ASSERT(vrr1 == vrr4);
)

TEST_BODY(SetDataByKeyAndValue, DuplicateUniqueKeyValue,
    Vault vlt;
    VaultRecordSet vrs;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddUniqueKey<int>("A");
    vlt.CreateRecord({{"A", 0}});
    vlt.CreateRecord({{"A", 1}});
    vlt.GetRecord("A", 0, vrr);

    // Check set data
    vor = vrr.SetData("A", 1);

    COMPARE_VAULT(vlt, {{{"A", 0}}, {{"A", 1}}});
    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::UniqueKeyValueAlredyInSet, SavedType == typeid(int));
)

TEST_BODY(SetDataByVectorOfParams, Default,
    Vault vlt;
    VaultRecordSet vrs1, vrs2;
    VaultRecordRef vrr1, vrr2, vrr3, vrr4, vrr5, vrr6, vrr7;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddKey<int64_t>("B", 0);

    vlt.CreateRecord({{"A", -10}});
    vlt.GetRecord("A", -10, vrr1);

    // Check set data
    vor = vrr1.SetData({{"A", 1}, {"B", int64_t(-1)}});

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "B",
        RequestedType == typeid(int64_t), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int64_t));
    
    // Check get record
    vlt.GetRecord("A", 1, vrr2);
    TEST_ASSERT(vrr1 == vrr2);

    vlt.GetRecord("B", int64_t(-1), vrr3);
    TEST_ASSERT(vrr1 == vrr3);

    // Check hash table request
    vlt.RequestEqual("A", 1, vrs1);
    vrs1.GetRecord("A", 1, vrr4);
    TEST_ASSERT(vrr1 == vrr4);

    vlt.RequestEqual("B", int64_t(-1), vrs1);
    vrs1.GetRecord("B", int64_t(-1), vrr5);
    TEST_ASSERT(vrr1 == vrr5);

    // Check map request
    vlt.RequestGreater("A", 0, vrs2);
    vrs2.GetRecord("A", 1, vrr6);
    TEST_ASSERT(vrr1 == vrr6);

    vlt.RequestLess("B", int64_t(0), vrs2);
    vrs2.GetRecord("B", int64_t(-1), vrr7);
    TEST_ASSERT(vrr1 == vrr7);
)

TEST_BODY(SetDataByVectorOfParams, WrongFirstKey,
    Vault vlt;
    VaultRecordSet vrs1, vrs2;
    VaultRecordRef vrr1, vrr2, vrr3, vrr4, vrr5, vrr6, vrr7;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddKey<int64_t>("B", 0);

    vlt.CreateRecord({{"A", 10}, {"B", int64_t(-10)}});
    vlt.GetRecord("A", 10, vrr1);

    // Check set data
    vor = vrr1.SetData({{"Z", 1}, {"B", int64_t(-1)}});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "Z",
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongKey, SavedType == typeid(void));
    
    // Check get record
    vlt.GetRecord("A", 10, vrr2);
    TEST_ASSERT(vrr1 == vrr2);

    vlt.GetRecord("B", int64_t(-10), vrr3);
    TEST_ASSERT(vrr1 == vrr3);

    // Check hash table request
    vlt.RequestEqual("A", 10, vrs1);
    vrs1.GetRecord("A", 10, vrr4);
    TEST_ASSERT(vrr1 == vrr4);

    vlt.RequestEqual("B", int64_t(-10), vrs1);
    vrs1.GetRecord("B", int64_t(-10), vrr5);
    TEST_ASSERT(vrr1 == vrr5);

    // Check map request
    vlt.RequestGreater("A", 0, vrs2);
    vrs2.GetRecord("A", 10, vrr6);
    TEST_ASSERT(vrr1 == vrr6);

    vlt.RequestLess("B", int64_t(0), vrs2);
    vrs2.GetRecord("B", int64_t(-10), vrr7);
    TEST_ASSERT(vrr1 == vrr7);
)

TEST_BODY(SetDataByVectorOfParams, WrongSecondKey,
    Vault vlt;
    VaultRecordSet vrs1, vrs2;
    VaultRecordRef vrr1, vrr2, vrr3, vrr4, vrr5, vrr6, vrr7;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddKey<int64_t>("B", 0);

    vlt.CreateRecord({{"A", 10}, {"B", int64_t(-10)}});
    vlt.GetRecord("A", 10, vrr1);

    // Check set data
    vor = vrr1.SetData({{"A", 1}, {"Z", int64_t(-1)}});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "Z",
        RequestedType == typeid(int64_t), ResultCode == VaultOperationResultCode::WrongKey, SavedType == typeid(void));
    
    // Check get record
    vlt.GetRecord("A", 1, vrr2);
    TEST_ASSERT(vrr1 == vrr2);

    vlt.GetRecord("B", int64_t(-10), vrr3);
    TEST_ASSERT(vrr1 == vrr3);

    // Check hash table request
    vlt.RequestEqual("A", 1, vrs1);
    vrs1.GetRecord("A", 1, vrr4);
    TEST_ASSERT(vrr1 == vrr4);

    vlt.RequestEqual("B", int64_t(-10), vrs1);
    vrs1.GetRecord("B", int64_t(-10), vrr5);
    TEST_ASSERT(vrr1 == vrr5);

    // Check map request
    vlt.RequestGreater("A", 0, vrs2);
    vrs2.GetRecord("A", 1, vrr6);
    TEST_ASSERT(vrr1 == vrr6);

    vlt.RequestLess("B", int64_t(0), vrs2);
    vrs2.GetRecord("B", int64_t(-10), vrr7);
    TEST_ASSERT(vrr1 == vrr7);
)

TEST_BODY(SetDataByVectorOfParams, WrongBothKeys,
    Vault vlt;
    VaultRecordSet vrs1, vrs2;
    VaultRecordRef vrr1, vrr2, vrr3, vrr4, vrr5, vrr6, vrr7;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddKey<int64_t>("B", 0);

    vlt.CreateRecord({{"A", 10}, {"B", int64_t(-10)}});
    vlt.GetRecord("A", 10, vrr1);

    // Check set data
    vor = vrr1.SetData({{"Z", 1}, {"Y", int64_t(-1)}});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "Z",
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongKey, SavedType == typeid(void));
    
    // Check get record
    vlt.GetRecord("A", 10, vrr2);
    TEST_ASSERT(vrr1 == vrr2);

    vlt.GetRecord("B", int64_t(-10), vrr3);
    TEST_ASSERT(vrr1 == vrr3);

    // Check hash table request
    vlt.RequestEqual("A", 10, vrs1);
    vrs1.GetRecord("A", 10, vrr4);
    TEST_ASSERT(vrr1 == vrr4);

    vlt.RequestEqual("B", int64_t(-10), vrs1);
    vrs1.GetRecord("B", int64_t(-10), vrr5);
    TEST_ASSERT(vrr1 == vrr5);

    // Check map request
    vlt.RequestGreater("A", 0, vrs2);
    vrs2.GetRecord("A", 10, vrr6);
    TEST_ASSERT(vrr1 == vrr6);

    vlt.RequestLess("B", int64_t(0), vrs2);
    vrs2.GetRecord("B", int64_t(-10), vrr7);
    TEST_ASSERT(vrr1 == vrr7);
)

TEST_BODY(SetDataByVectorOfParams, WrongTypeOnFirstKey,
    Vault vlt;
    VaultRecordSet vrs1, vrs2;
    VaultRecordRef vrr1, vrr2, vrr3, vrr4, vrr5, vrr6, vrr7;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddKey<int64_t>("B", 0);

    vlt.CreateRecord({{"A", 10}, {"B", int64_t(-10)}});
    vlt.GetRecord("A", 10, vrr1);

    // Check set data
    vor = vrr1.SetData({{"A", float(0.0)}, {"B", int64_t(-1)}});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A",
        RequestedType == typeid(float), ResultCode == VaultOperationResultCode::WrongType, SavedType == typeid(int));
    
    // Check get record
    vlt.GetRecord("A", 10, vrr2);
    TEST_ASSERT(vrr1 == vrr2);

    vlt.GetRecord("B", int64_t(-10), vrr3);
    TEST_ASSERT(vrr1 == vrr3);

    // Check hash table request
    vlt.RequestEqual("A", 10, vrs1);
    vrs1.GetRecord("A", 10, vrr4);
    TEST_ASSERT(vrr1 == vrr4);

    vlt.RequestEqual("B", int64_t(-10), vrs1);
    vrs1.GetRecord("B", int64_t(-10), vrr5);
    TEST_ASSERT(vrr1 == vrr5);

    // Check map request
    vlt.RequestGreater("A", 0, vrs2);
    vrs2.GetRecord("A", 10, vrr6);
    TEST_ASSERT(vrr1 == vrr6);

    vlt.RequestLess("B", int64_t(0), vrs2);
    vrs2.GetRecord("B", int64_t(-10), vrr7);
    TEST_ASSERT(vrr1 == vrr7);
)

TEST_BODY(SetDataByVectorOfParams, WrongTypeOnSecondKey,
    Vault vlt;
    VaultRecordSet vrs1, vrs2;
    VaultRecordRef vrr1, vrr2, vrr3, vrr4, vrr5, vrr6, vrr7;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddKey<int64_t>("B", 0);

    vlt.CreateRecord({{"A", 10}, {"B", int64_t(-10)}});
    vlt.GetRecord("A", 10, vrr1);

    // Check set data
    vor = vrr1.SetData({{"A", 1}, {"B", -1}});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "B",
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongType, SavedType == typeid(int64_t));
    
    // Check get record
    vlt.GetRecord("A", 1, vrr2);
    TEST_ASSERT(vrr1 == vrr2);

    vlt.GetRecord("B", int64_t(-10), vrr3);
    TEST_ASSERT(vrr1 == vrr3);

    // Check hash table request
    vlt.RequestEqual("A", 1, vrs1);
    vrs1.GetRecord("A", 1, vrr4);
    TEST_ASSERT(vrr1 == vrr4);

    vlt.RequestEqual("B", int64_t(-10), vrs1);
    vrs1.GetRecord("B", int64_t(-10), vrr5);
    TEST_ASSERT(vrr1 == vrr5);

    // Check map request
    vlt.RequestGreater("A", 0, vrs2);
    vrs2.GetRecord("A", 1, vrr6);
    TEST_ASSERT(vrr1 == vrr6);

    vlt.RequestLess("B", int64_t(0), vrs2);
    vrs2.GetRecord("B", int64_t(-10), vrr7);
    TEST_ASSERT(vrr1 == vrr7);
)

TEST_BODY(SetDataByVectorOfParams, WrongTypeOnBothKeys,
    Vault vlt;
    VaultRecordSet vrs1, vrs2;
    VaultRecordRef vrr1, vrr2, vrr3, vrr4, vrr5, vrr6, vrr7;
    VaultOperationResult vor;

    vlt.AddKey("A", 0);
    vlt.AddKey<int64_t>("B", 0);

    vlt.CreateRecord({{"A", 10}, {"B", int64_t(-10)}});
    vlt.GetRecord("A", 10, vrr1);

    // Check set data
    vor = vrr1.SetData({{"A", float(1.0)}, {"B", -1}});

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A",
        RequestedType == typeid(float), ResultCode == VaultOperationResultCode::WrongType, SavedType == typeid(int));
    
    // Check get record
    vlt.GetRecord("A", 10, vrr2);
    TEST_ASSERT(vrr1 == vrr2);

    vlt.GetRecord("B", int64_t(-10), vrr3);
    TEST_ASSERT(vrr1 == vrr3);

    // Check hash table request
    vlt.RequestEqual("A", 10, vrs1);
    vrs1.GetRecord("A", 10, vrr4);
    TEST_ASSERT(vrr1 == vrr4);

    vlt.RequestEqual("B", int64_t(-10), vrs1);
    vrs1.GetRecord("B", int64_t(-10), vrr5);
    TEST_ASSERT(vrr1 == vrr5);

    // Check map request
    vlt.RequestGreater("A", 0, vrs2);
    vrs2.GetRecord("A", 10, vrr6);
    TEST_ASSERT(vrr1 == vrr6);

    vlt.RequestLess("B", int64_t(0), vrs2);
    vrs2.GetRecord("B", int64_t(-10), vrr7);
    TEST_ASSERT(vrr1 == vrr7);
)

TEST_BODY(GetData, Invalid,
    VaultRecordRef vrr;
    VaultOperationResult vor;

    int i = 0;
    vor = vrr.GetData("A", i);
    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::DataRecordNotValid, SavedType == typeid(void));
    
    TEST_ASSERT(i == 0);
)

TEST_BODY(GetData, Deleted,
    Vault vlt;
    VaultRecordRef vrr1, vrr2;
    VaultOperationResult vor;

    vlt.AddKey("A", 1);
    vlt.CreateRecord({{"A", 1}});
    vlt.GetRecord("A", 1, vrr1);
    vlt.GetRecord("A", 1, vrr2);

    vlt.EraseRecord(vrr2);

    int i = 0;
    vor = vrr1.GetData("A", i);
    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::DataRecordNotValid, SavedType == typeid(void));
    
    TEST_ASSERT(i == 0);
)

TEST_BODY(GetData, Default,
    Vault vlt;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", -1);
    vlt.AddUniqueKey<std::string>("B");
    vlt.AddKey("C", true);

    vlt.CreateRecord({{"A", 1}, {"B", std::string("b")}, {"C", false}});
    vlt.GetRecord("A", 1, vrr);

    int a = 0;
    std::string b;
    bool c = true;

    // Get A
    vor = vrr.GetData("A", a);

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A",
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));

    TEST_ASSERT(a == 1);

    // Get B
    vor = vrr.GetData("B", b);

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "B",
        RequestedType == typeid(std::string), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(std::string));

    TEST_ASSERT(b == "b");

    // Get C
    vor = vrr.GetData("C", c);

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "C",
        RequestedType == typeid(bool), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(bool));

    TEST_ASSERT(c == false);
)

TEST_BODY(GetData, WrongKey,
    Vault vlt;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", -1);
    vlt.AddUniqueKey<std::string>("B");
    vlt.AddKey("C", true);

    vlt.CreateRecord({{"A", 1}, {"B", std::string("b")}, {"C", false}});
    vlt.GetRecord("A", 1, vrr);

    int a = 0;

    vor = vrr.GetData("Z", a);

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "Z",
        RequestedType == typeid(int), ResultCode == VaultOperationResultCode::WrongKey, SavedType == typeid(void));

    TEST_ASSERT(a == 0);
)

TEST_BODY(GetData, WrongType,
    Vault vlt;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", -1);
    vlt.AddUniqueKey<std::string>("B");
    vlt.AddKey("C", true);

    vlt.CreateRecord({{"A", 1}, {"B", std::string("b")}, {"C", false}});
    vlt.GetRecord("A", 1, vrr);

    std::string s;

    vor = vrr.GetData("A", s);

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A",
        RequestedType == typeid(std::string), ResultCode == VaultOperationResultCode::WrongType, SavedType == typeid(int));

    TEST_ASSERT(s.empty());
)

TEST_BODY(GetDataAsString, Invalid,
    VaultRecordRef vrr;
    VaultOperationResult vor;

    std::string s;
    vor = vrr.GetDataAsString("A", s);
    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(std::string), ResultCode == VaultOperationResultCode::DataRecordNotValid, SavedType == typeid(void));
    
    TEST_ASSERT(s.empty());
)

TEST_BODY(GetDataAsString, Deleted,
    Vault vlt;
    VaultRecordRef vrr1, vrr2;
    VaultOperationResult vor;

    vlt.AddKey("A", 1);
    vlt.CreateRecord({{"A", 1}});
    vlt.GetRecord("A", 1, vrr1);
    vlt.GetRecord("A", 1, vrr2);

    vlt.EraseRecord(vrr2);

    std::string s;
    vor = vrr1.GetDataAsString("A", s);
    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "A", 
        RequestedType == typeid(std::string), ResultCode == VaultOperationResultCode::DataRecordNotValid, SavedType == typeid(void));
    
    TEST_ASSERT(s.empty());
)

TEST_BODY(GetDataAsString, Default,
    Vault vlt;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", -1);
    vlt.AddUniqueKey<std::string>("B");
    vlt.AddKey("C", true);

    vlt.CreateRecord({{"A", 1}, {"B", std::string("b")}, {"C", false}});
    vlt.GetRecord("A", 1, vrr);

    std::string s;

    // Get A
    vor = vrr.GetDataAsString("A", s);

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "A",
        RequestedType == typeid(std::string), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(int));

    TEST_ASSERT(s == "1");

    // Get B
    vor = vrr.GetDataAsString("B", s);

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "B",
        RequestedType == typeid(std::string), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(std::string));

    TEST_ASSERT(s == "b");

    // Get C
    vor = vrr.GetDataAsString("C", s);

    COMPARE_OPERATION(vor, IsOperationSuccess == true, Key == "C",
        RequestedType == typeid(std::string), ResultCode == VaultOperationResultCode::Success, SavedType == typeid(bool));

    TEST_ASSERT(s == "false");
)

TEST_BODY(GetDataAsString, WrongKey,
    Vault vlt;
    VaultRecordRef vrr;
    VaultOperationResult vor;

    vlt.AddKey("A", -1);
    vlt.AddUniqueKey<std::string>("B");
    vlt.AddKey("C", true);

    vlt.CreateRecord({{"A", 1}, {"B", std::string("b")}, {"C", false}});
    vlt.GetRecord("A", 1, vrr);

    std::string s;

    vor = vrr.GetDataAsString("Z", s);

    COMPARE_OPERATION(vor, IsOperationSuccess == false, Key == "Z",
        RequestedType == typeid(std::string), ResultCode == VaultOperationResultCode::WrongKey, SavedType == typeid(void));

    TEST_ASSERT(s.empty());
)

TEST_BODY(IsValid, Invalid,
    VaultRecordRef vrr;

    TEST_ASSERT(vrr.IsValid() == false);
)

TEST_BODY(IsValid, Valid,
    Vault vlt;
    VaultRecordRef vrr;

    vlt.AddKey("A", 0);
    vlt.CreateRecord({{"A", 0}});
    vlt.GetRecord("A", 0, vrr);
    
    TEST_ASSERT(vrr.IsValid() == true);

    vlt.EraseRecord(vrr);

    TEST_ASSERT(vrr.IsValid() == false);
)

TEST_BODY(IsKeyExist, Invalid,
    VaultRecordRef vrr;

    TEST_ASSERT(vrr.IsKeyExist("A") == false);
)

TEST_BODY(IsKeyExist, Existed,
    Vault vlt;
    VaultRecordRef vrr;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");
    vlt.CreateRecord({{"A", 0}});
    vlt.GetRecord("A", 0, vrr);

    TEST_ASSERT(vlt.IsKeyExist("A") == true);
    TEST_ASSERT(vlt.IsKeyExist("B") == true);
)

TEST_BODY(IsKeyExist, NotExisted,
    Vault vlt;
    VaultRecordRef vrr;

    vlt.AddKey("A", 0);
    vlt.CreateRecord({{"A", 0}});
    vlt.GetRecord("A", 0, vrr);

    TEST_ASSERT(vlt.IsKeyExist("Z") == false);
)

TEST_BODY(GetKeys, Invalid,
    VaultRecordRef vrr;

    std::vector<std::string> keys = vrr.GetKeys();
    TEST_ASSERT(keys.empty());
)

TEST_BODY(GetKeys, Valid,
    Vault vlt;
    VaultRecordRef vrr;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");
    vlt.CreateRecord({{"A", 0}});
    vlt.GetRecord("A", 0, vrr);

    std::vector<std::string> keys = vrr.GetKeys();

    TEST_ASSERT(keys.size() == 2);
    TEST_ASSERT(keys[0] == "A");
    TEST_ASSERT(keys[1] == "B");
)

TEST_BODY(PrintRecord, Invalid,
    VaultRecordRef vrr;

    TEST_COUT(vrr.PrintRecord(), "VaultRecordRef not valid!\n");
)

TEST_BODY(PrintRecord, Deleted,
    Vault vlt;
    VaultRecordRef vrr1, vrr2;
    VaultOperationResult vor;

    vlt.AddKey("A", 1);
    vlt.CreateRecord({{"A", 1}});
    vlt.GetRecord("A", 1, vrr1);
    vlt.GetRecord("A", 1, vrr2);

    vlt.EraseRecord(vrr2);

    TEST_COUT(vrr1.PrintRecord(), "VaultRecordRef not valid!\n");
)

TEST_BODY(PrintRecord, Valid,
    Vault vlt;
    VaultRecordRef vrr;

    vlt.AddKey("A", 0);
    vlt.CreateRecord({{"A", 0}});
    vlt.GetRecord("A", 0, vrr);

    SUPPRESS_COUT(vrr.PrintRecord());
)

TEST_BODY(Reset, Invalid,
    VaultRecordRef vrr;

    vrr.Reset();

    TEST_ASSERT(vrr.IsValid() == false);
    TEST_ASSERT(vrr.GetRecordUniqueId() == "null");
)

TEST_BODY(Reset, Valid,
    Vault vlt;
    VaultRecordSet vrs;
    VaultRecordRef vrr;

    vlt.AddKey("A", 0);
    vlt.CreateRecord({{"A", 0}});
    vlt.RequestEqual("A", 0, vrs);
    vlt.GetRecord("A", 0, vrr);

    vrr.Reset();

    TEST_ASSERT(vrr.IsValid() == false);
    TEST_ASSERT(vrr.GetRecordUniqueId() == "null");

    TEST_ASSERT(vlt.Size() == 1);
    TEST_ASSERT(vrs.Size() == 1);
)

TEST_BODY(ToJson, Invalid,
    VaultRecordRef vrr;

    TEST_ASSERT(vrr.ToJson() == "{\n}");
)

TEST_BODY(ToJson, Default,
    Vault vlt;
    VaultRecordRef vrr;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 0}, {"B", std::string("b")}});
    vlt.GetRecord("A", 0, vrr);

    TEST_ASSERT(vrr.ToJson() == R"({
  "A": 0,
  "B": "b"
})");
)

TEST_BODY(ToJson, NonFormat,
    Vault vlt;
    VaultRecordRef vrr;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 0}, {"B", std::string("b")}});
    vlt.GetRecord("A", 0, vrr);

    TEST_ASSERT(vrr.ToJson(false) == R"({"A":0,"B":"b"})");
)

TEST_BODY(ToJson, DiffTabSize,
    Vault vlt;
    VaultRecordRef vrr;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 0}, {"B", std::string("b")}});
    vlt.GetRecord("A", 0, vrr);

    TEST_ASSERT(vrr.ToJson(true, 4) == R"({
    "A": 0,
    "B": "b"
})");
)

TEST_BODY(ToString, Invalid,
    VaultRecordRef vrr;

    std::vector<std::pair<std::string, std::string>> strings = vrr.ToStrings();
    
    TEST_ASSERT(strings.empty());
)

TEST_BODY(ToString, Valid,
    Vault vlt;
    VaultRecordRef vrr;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 0}, {"B", std::string("b")}});
    vlt.GetRecord("A", 0, vrr);

    std::vector<std::pair<std::string, std::string>> strings = vrr.ToStrings();
    
    std::pair<std::string, std::string> res("A", "0");
    TEST_ASSERT(strings[0] == res);

    res = {"B", "b"};
    TEST_ASSERT(strings[1] == res);
)

TEST_BODY(Destructor, Default,
    Vault vlt;
    VaultRecordRef vrr;
    VaultRecordRef* vrrPtr = new VaultRecordRef;

    vlt.AddKey("A", 0);
    vlt.AddUniqueKey<std::string>("B");

    vlt.CreateRecord({{"A", 0}, {"B", std::string("b")}});
    vlt.GetRecord("A", 0, *vrrPtr);

    delete vrrPtr;

    TEST_ASSERT(vlt.Size() == 1);

    vlt.GetRecord("A", 0, vrr);

    TEST_ASSERT(vrr.IsValid());
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