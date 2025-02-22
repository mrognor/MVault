#pragma once

#include "../../Source/MVault.h"

#include "../MTests/TestLog.h"
#include "../MTests/TestSuite.h"
#include "../MTests/TestAssert.h"
#include "../MTests/CompareVault.h"
#include "../MTests/CompareVaultOperationResult.h"

using namespace mvlt;

TEST_SUITE(DefaultConstructor,

    TEST_CASE(Default)
)

TEST_SUITE(CopyConstructor,

    TEST_CASE(CopyEmptyVault)

    TEST_CASE(CopyFilledVault)
)

TEST_SUITE(AssignmentOperator,

    TEST_CASE(AssignEmptyVault)

    TEST_CASE(AssignFilledToEmpty)

    // This test required to check vault clearing on assigning
    TEST_CASE(AssignEmptyToFilled)
)

TEST_SUITE(MoveConstructor,

    TEST_CASE(MoveEmptyConstructor)

    TEST_CASE(MoveFilledConstructor)
)

TEST_SUITE(MoveAssignmentOperator,

    TEST_CASE(AssignEmptyVault)

    TEST_CASE(AssignFilledToEmpty)

    /// This test required to check vault clearing on assigning
    TEST_CASE(AssignEmptyToFilled)
)

TEST_SUITE(AddKey,

    TEST_CASE(CorrectAddKey)

    TEST_CASE(IncorrectAddKey)

    TEST_CASE(CorrectAddToNonEmpty)

    TEST_CASE(IncorrectAddToNonEmpty)
)

TEST_SUITE(AddUniqueKey,

    TEST_CASE(CorrectAddKeyToEmptyVault)

    TEST_CASE(IncorrectAddKeyToEmptyVault);

    TEST_CASE(CorrectAddKeyToNonEmptyVault);

    TEST_CASE(IncorrectAddKeyToNonEmptyVault);

    TEST_CASE(CorrectAddKeyToNonEmptyVaultWithDuplicate);
)

TEST_SUITE(UpdateKey,

    TEST_CASE(CorrectUpdateNonUniqueKey)

    TEST_CASE(WrongKeyUpdate)

    TEST_CASE(WrongType)

    TEST_CASE(UniqueKey)
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

TEST_SUITE(GetKeys,

    TEST_CASE(Keys)
)

TEST_SUITE(GetUniqueKeys,

    TEST_CASE(UniqueKeys)
)

TEST_SUITE(RemoveKey,

    TEST_CASE(CorrectRemoveFromEmptyVault)

    TEST_CASE(CorrectRemoveFromNonEmptyVault)

    TEST_CASE(IncorrectRemoveFromEmptyVault)

    TEST_CASE(IncorrectRemoveFromNonEmptyVault)
)

TEST_SUITE(CreateKey,

    TEST_CASE(CorrectParamsWithoutUniqueKey)

    TEST_CASE(CorrectParamsWithUniqueKey)

    TEST_CASE(WrongKeyWithoutUniqueKey)

    TEST_CASE(WrongTypeWithoutUniqueKey)

    TEST_CASE(WrongKeyWithUniqueKey)

    TEST_CASE(WrongTypeWithUniqueKey)

    TEST_CASE(DuplicateUniqueKeyValue)
)

void VaultUnitTests();