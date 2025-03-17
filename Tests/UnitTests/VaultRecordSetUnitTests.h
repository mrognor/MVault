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
}

void VaultRecordSetUnitTests();