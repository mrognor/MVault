#pragma once

#include "../../Source/MVault.h"

#include "../MTests/TestLog.h"
#include "../MTests/TestSuite.h"
#include "../MTests/TestAssert.h"
#include "../MTests/CompareVault.h"
#include "../MTests/CompareRequest.h"
#include "../MTests/CompareVaultRecordRef.h"
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

    TEST_CASE(DuplicateParams)

    TEST_CASE(WrongKeyWithoutUniqueKey)

    TEST_CASE(WrongTypeWithoutUniqueKey)

    TEST_CASE(WrongKeyWithUniqueKey)

    TEST_CASE(WrongTypeWithUniqueKey)

    TEST_CASE(DuplicateUniqueKeyValue)
)

TEST_SUITE(GetRecord,

    TEST_CASE(CorrectGetRecord)

    TEST_CASE(WrongKey)

    TEST_CASE(WrongType)

    TEST_CASE(WrongValue)
)

TEST_SUITE(GetRecords,

    TEST_CASE(CorrectGetRecords)

    TEST_CASE(CorrectGetNotAllRecords)

    TEST_CASE(WrongKey)

    TEST_CASE(WrongType)

    TEST_CASE(WrongValue)
)

TEST_SUITE(RequestEqual,

    TEST_CASE(CorrectRequest)

    TEST_CASE(CorrectRequestNotAllRecords)

    TEST_CASE(CorrectRequestWithPredicat)

    TEST_CASE(WrongKey)

    TEST_CASE(WrongType)

    TEST_CASE(ValueNotInVault)
)

TEST_SUITE(RequestGreater,

    TEST_CASE(CorrectRequest)

    TEST_CASE(CorrectRequestNotAllRecords)

    TEST_CASE(CorrectRequestWithPredicat)

    TEST_CASE(BoundaryValues)

    TEST_CASE(WrongKey)

    TEST_CASE(WrongType)

    TEST_CASE(ValueNotInVault)
)

TEST_SUITE(RequestGreaterOrEqual,

    TEST_CASE(CorrectRequest)

    TEST_CASE(CorrectRequestNotAllRecords)

    TEST_CASE(CorrectRequestWithPredicat)

    TEST_CASE(BoundaryValues)

    TEST_CASE(WrongKey)

    TEST_CASE(WrongType)

    TEST_CASE(ValueNotInVault)
)

TEST_SUITE(RequestLess,

    TEST_CASE(CorrectRequest)

    TEST_CASE(CorrectRequestNotAllRecords)

    TEST_CASE(CorrectRequestWithPredicat)

    TEST_CASE(BoundaryValues)

    TEST_CASE(WrongKey)

    TEST_CASE(WrongType)

    TEST_CASE(ValueNotInVault)
)

TEST_SUITE(RequestLessOrEqual,

    TEST_CASE(CorrectRequest)

    TEST_CASE(CorrectRequestNotAllRecords)

    TEST_CASE(CorrectRequestWithPredicat)

    TEST_CASE(BoundaryValues)

    TEST_CASE(WrongKey)

    TEST_CASE(WrongType)

    TEST_CASE(ValueNotInVault)
)

TEST_SUITE(RequestInterval,

    TEST_CASE(CorrectRequest)

    TEST_CASE(CorrectRequestWithoutLeftBoundary)

    TEST_CASE(CorrectRequestWithoutRightBoundary)

    TEST_CASE(CorrectRequestWithoutBoundaries)

    TEST_CASE(CorrectRequestNotAllRecords)

    TEST_CASE(CorrectRequestWithPredicat)

    TEST_CASE(BoundaryValues)

    TEST_CASE(SameBoundaries)

    TEST_CASE(IncorrectBoundaries)

    TEST_CASE(WrongKey)

    TEST_CASE(WrongType)
)

TEST_SUITE(Request,

    TEST_CASE(CorrectRequest)

    TEST_CASE(CorrectRequestWithPredicat)

    TEST_CASE(BoundaryValues)

    TEST_CASE(WrongKey)

    TEST_CASE(WrongType)

    TEST_CASE(ComplexRequestAnd)

    TEST_CASE(ComplexRequestOr)

    TEST_CASE(ComplexRequest)

    TEST_CASE(ComplexRequestWrongKey)

    TEST_CASE(ComplexRequestWrongType)
)

TEST_SUITE(DropVault,
    TEST_CASE(Drop)

    TEST_CASE(DropEmpty)

    TEST_CASE(DropSecond)
)

TEST_SUITE(DropData,
    TEST_CASE(Drop)

    TEST_CASE(DropEmpty)

    TEST_CASE(DropSecond)
)

TEST_SUITE(EraseRecord,
    TEST_CASE(CorrectEraseByRef)

    TEST_CASE(IncorrectEraseByRef)

    TEST_CASE(CorrectEraseByKeyAndValue)

    TEST_CASE(WrongKeyEraseByKeyAndValue)

    TEST_CASE(WrongTypeEraseByKeyAndValue)

    TEST_CASE(WrongValueEraseByKeyAndValue)
)

TEST_SUITE(EraseRecords,
    TEST_CASE(Erase)

    TEST_CASE(WrongKeyErase)

    TEST_CASE(WrongTypeErase)

    TEST_CASE(WrongValueErase)
)

TEST_SUITE(Size,
    TEST_CASE(Default)
)

TEST_SUITE(GetSortedRecords,
    TEST_CASE(GetRecords)

    TEST_CASE(GetRecordsReverse)

    TEST_CASE(GetNotAllRecords)

    TEST_CASE(WrongKey)
)

TEST_SUITE(SortBy,
    TEST_CASE(Sort)

    TEST_CASE(SortReverse)

    TEST_CASE(SortNotAllRecords)

    TEST_CASE(WrongKey)
)

TEST_SUITE(ToJson,
    TEST_CASE(Default)

    TEST_CASE(Format)

    TEST_CASE(DiffTabSize)

    TEST_CASE(RecordTemplate)

    TEST_CASE(Array)

    TEST_CASE(ArrayFormat)

    TEST_CASE(ArrayDiffTabSize)
)

void VaultUnitTests();