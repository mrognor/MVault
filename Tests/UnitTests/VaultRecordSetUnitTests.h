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

namespace VaultRecordSetTestNs
{
    TEST_SUITE(DefaultConstructor,

        TEST_CASE(Default)
    )

    TEST_SUITE(CopyConstructor,

        TEST_CASE(CopyWithoutParent)

        TEST_CASE(CopyEmptySet)

        TEST_CASE(CopyFilledSet)
    )

    TEST_SUITE(AssignmentOperator,

        TEST_CASE(AssignWithoutParents)

        TEST_CASE(AssignWithParentsToWithoutParent)

        TEST_CASE(AssignEmpty)

        TEST_CASE(AssignFilledSet)
    )

    TEST_SUITE(MoveConstructor,
    
        TEST_CASE(MoveWithoutParents)

        TEST_CASE(MoveEmpty)

        TEST_CASE(MoveFilled)
    )

    TEST_SUITE(MoveAssignmentOperator,

        TEST_CASE(AssignWithoutParents)

        TEST_CASE(AssignWithParentsToWithoutParent)

        TEST_CASE(AssignEmpty)

        TEST_CASE(AssignFilledSet)
    )

    TEST_SUITE(GetIsParentVaultValid,
        
        TEST_CASE(Invalid)

        TEST_CASE(Valid)

        TEST_CASE(Invalidate)
    )

    TEST_SUITE(GetParentVaultUniqueId,
        
        TEST_CASE(Invalid)

        TEST_CASE(Valid)

        TEST_CASE(Invalidate)
    )

    TEST_SUITE(IsKeyExist,

        TEST_CASE(ExistingKey)

        TEST_CASE(NonExistingKey)
    )

    TEST_SUITE(GetKeyValue,

        TEST_CASE(ExistingKey)

        TEST_CASE(NonExistingKey)

        TEST_CASE(WrongType)

        TEST_CASE(UniqueKey)
    )

    TEST_SUITE(GetKeyType,

        TEST_CASE(ExistingKey)

        TEST_CASE(NonExistingKey)
    )
}

void VaultRecordSetUnitTests();