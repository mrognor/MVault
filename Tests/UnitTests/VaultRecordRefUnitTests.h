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
}

void VaultRecordRefUnitTests(const std::string& testName);