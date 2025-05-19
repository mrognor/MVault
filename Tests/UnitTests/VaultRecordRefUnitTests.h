#pragma once

#include "../../Source/MVault.h"

#include "../MTests/TestLog.h"
#include "../MTests/TestCout.h"
#include "../MTests/TestSuite.h"
#include "../MTests/TestAssert.h"
#include "../MTests/CompareFile.h"
#include "../MTests/CompareVault.h"
#include "../MTests/CompareRequest.h"
#include "../MTests/CompareVaultRecordRef.h"
#include "../MTests/CompareVaultOperationResult.h"

using namespace mvlt;

namespace VaultRecordRefTestNs
{
    TEST_SUITE(DefaultConstructor,

        TEST_CASE(Default)
    )

    TEST_SUITE(CopyConstructor,
    
        TEST_CASE(FromEmpty)

        TEST_CASE(FromFilled)
    )

    TEST_SUITE(MoveConstructor,
    
        TEST_CASE(FromEmpty)

        TEST_CASE(FromFilled)
    )

    TEST_SUITE(AssignmentOperator,

        TEST_CASE(AssignWithoutParents)

        TEST_CASE(AssignWithParentsToWithoutParent)

        TEST_CASE(AssignWithoutParentToWithParent)

        TEST_CASE(Assign)
    )

    TEST_SUITE(MoveAssignmentOperator,

        TEST_CASE(AssignWithoutParents)

        TEST_CASE(AssignWithParentsToWithoutParent)

        TEST_CASE(AssignWithoutParentToWithParent)

        TEST_CASE(Assign)
    )

    TEST_SUITE(OperatorComparison,
    
        TEST_CASE(CompareWithoutParents)

        TEST_CASE(CompareWithParentToWithoutParent)

        TEST_CASE(Compare)
    )

    TEST_SUITE(OperatorNonComparison,
    
        TEST_CASE(CompareWithoutParents)

        TEST_CASE(CompareWithParentToWithoutParent)

        TEST_CASE(Compare)
    )

    TEST_SUITE(GetRecordUniqueId,
    
        TEST_CASE(Invalid)

        TEST_CASE(Valid)
    )

    TEST_SUITE(SetDataByKeyAndValue,

        TEST_CASE(Invalid)

        TEST_CASE(Deleted)

        TEST_CASE(Default)

        TEST_CASE(WrongKey)

        TEST_CASE(WrongType)

        TEST_CASE(DuplicateUniqueKeyValue)
    )

    TEST_SUITE(SetDataByVectorOfParams,
    
        TEST_CASE(Default)

        TEST_CASE(WrongFirstKey)

        TEST_CASE(WrongSecondKey)

        TEST_CASE(WrongBothKeys)

        TEST_CASE(WrongTypeOnFirstKey)

        TEST_CASE(WrongTypeOnSecondKey)

        TEST_CASE(WrongTypeOnBothKeys)
    )

    TEST_SUITE(GetData,

        TEST_CASE(Invalid)

        TEST_CASE(Deleted)

        TEST_CASE(Default)

        TEST_CASE(WrongKey)

        TEST_CASE(WrongType)
    )

    TEST_SUITE(GetDataAsString,

        TEST_CASE(Invalid)

        TEST_CASE(Deleted)

        TEST_CASE(Default)

        TEST_CASE(WrongKey)
    )

    TEST_SUITE(IsValid,
    
        TEST_CASE(Invalid)

        TEST_CASE(Valid)
    )

    TEST_SUITE(IsKeyExist,

        TEST_CASE(Invalid)

        TEST_CASE(Existed)

        TEST_CASE(NotExisted)
    )

    TEST_SUITE(GetKeys,
    
        TEST_CASE(Invalid)

        TEST_CASE(Valid)
    )

    TEST_SUITE(PrintRecord,
        
        TEST_CASE(Invalid)

        TEST_CASE(Deleted)

        TEST_CASE(Valid)
    )

    TEST_SUITE(Reset,
        
        TEST_CASE(Invalid)

        TEST_CASE(Valid)
    )

    TEST_SUITE(ToJson,
    
        TEST_CASE(Invalid)

        TEST_CASE(Default)

        TEST_CASE(NonFormat)

        TEST_CASE(DiffTabSize)
    )

    TEST_SUITE(ToString,
        
        TEST_CASE(Invalid)

        TEST_CASE(Valid)
    )
    
    TEST_SUITE(Destructor,
        
        TEST_CASE(Default)
    )
}

void VaultRecordRefUnitTests(const std::string& testName);