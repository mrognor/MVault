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

        TEST_CASE(Invalid)

        TEST_CASE(ExistingKey)

        TEST_CASE(NonExistingKey)
    )

    TEST_SUITE(GetKeyValue,

        TEST_CASE(Invalid)

        TEST_CASE(ExistingKey)

        TEST_CASE(NonExistingKey)

        TEST_CASE(WrongType)

        TEST_CASE(UniqueKey)
    )

    TEST_SUITE(GetKeyType,

        TEST_CASE(Invalid)

        TEST_CASE(ExistingKey)

        TEST_CASE(NonExistingKey)
    )

    TEST_SUITE(AddRecord,

        TEST_CASE(Invalid)

        TEST_CASE(Correct)

        TEST_CASE(InvalidRef)

        TEST_CASE(OtherVaultParent)

        TEST_CASE(SecondTime)
    )

    TEST_SUITE(GetRecord,
    
        TEST_CASE(Invalid)

        TEST_CASE(CorrectGetRecord)

        TEST_CASE(WrongKey)

        TEST_CASE(WrongType)

        TEST_CASE(WrongValue)
    )

    TEST_SUITE(GetRecords,
    
        TEST_CASE(Invalid)

        TEST_CASE(CorrectGetRecords)

        TEST_CASE(CorrectGetNotAllRecords)

        TEST_CASE(WrongKey)

        TEST_CASE(WrongType)

        TEST_CASE(WrongValue)
    )

    TEST_SUITE(RequestEqual,

        TEST_CASE(Invalid)

        TEST_CASE(CorrectRequest)

        TEST_CASE(CorrectRequestNotAllRecords)

        TEST_CASE(CorrectRequestWithPredicat)

        TEST_CASE(WrongKey)

        TEST_CASE(WrongType)

        TEST_CASE(ValueNotInVault)

        TEST_CASE(Self)
    )

    TEST_SUITE(RequestGreater,

        TEST_CASE(Invalid)

        TEST_CASE(CorrectRequest)

        TEST_CASE(CorrectRequestNotAllRecords)

        TEST_CASE(CorrectRequestWithPredicat)

        TEST_CASE(BoundaryValues)

        TEST_CASE(WrongKey)

        TEST_CASE(WrongType)

        TEST_CASE(ValueNotInVault)

        TEST_CASE(Self)
    )

    TEST_SUITE(RequestGreaterOrEqual,

        TEST_CASE(Invalid)

        TEST_CASE(CorrectRequest)

        TEST_CASE(CorrectRequestNotAllRecords)

        TEST_CASE(CorrectRequestWithPredicat)

        TEST_CASE(BoundaryValues)

        TEST_CASE(WrongKey)

        TEST_CASE(WrongType)

        TEST_CASE(ValueNotInVault)

        TEST_CASE(Self)
    )

    TEST_SUITE(RequestLess,

        TEST_CASE(Invalid)

        TEST_CASE(CorrectRequest)

        TEST_CASE(CorrectRequestNotAllRecords)

        TEST_CASE(CorrectRequestWithPredicat)

        TEST_CASE(BoundaryValues)

        TEST_CASE(WrongKey)

        TEST_CASE(WrongType)

        TEST_CASE(ValueNotInVault)

        TEST_CASE(Self)
    )

    TEST_SUITE(RequestLessOrEqual,

        TEST_CASE(Invalid)

        TEST_CASE(CorrectRequest)

        TEST_CASE(CorrectRequestNotAllRecords)

        TEST_CASE(CorrectRequestWithPredicat)

        TEST_CASE(BoundaryValues)

        TEST_CASE(WrongKey)

        TEST_CASE(WrongType)

        TEST_CASE(ValueNotInVault)

        TEST_CASE(Self)
    )

    TEST_SUITE(RequestInterval,

        TEST_CASE(Invalid)

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

        TEST_CASE(Self)
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

        TEST_CASE(Self)

        TEST_CASE(InvalidParent)
    )

    TEST_SUITE(CheckRecord,
        
        TEST_CASE(InvalidSet)

        TEST_CASE(InvalidRef)

        TEST_CASE(WrongRef)

        TEST_CASE(Correct)
    )

    TEST_SUITE(Reset,
        
        TEST_CASE(InvalidSet)

        TEST_CASE(EmptySet)

        TEST_CASE(FilledSet)
    )

    TEST_SUITE(Clear,
        
        TEST_CASE(InvalidSet)

        TEST_CASE(EmptySet)

        TEST_CASE(FilledSet)
    )

    TEST_SUITE(RemoveRecordByRef,
    
        TEST_CASE(InvalidSet)

        TEST_CASE(InvalidRef)

        TEST_CASE(Default)

        TEST_CASE(SecondDeletion)
    )

    TEST_SUITE(RemoveRecordByKeyAndValue,
    
        TEST_CASE(InvalidSet)

        TEST_CASE(Default)

        TEST_CASE(SecondDeletion)

        TEST_CASE(WrongKey)

        TEST_CASE(WrongType)

        TEST_CASE(WrongValue)
    )

    TEST_SUITE(RemoveRecords,
    
        TEST_CASE(InvalidSet)

        TEST_CASE(Default)

        TEST_CASE(DeleteNotAll)

        TEST_CASE(WrongKey)

        TEST_CASE(WrongType)

        TEST_CASE(WrongValue)
    )

    TEST_SUITE(Size,

        TEST_CASE(Invalid)

        TEST_CASE(Empty)

        TEST_CASE(Default)
    )

    TEST_SUITE(GetKeys,
    
        TEST_CASE(Invalid)

        TEST_CASE(Keys)
    )

    TEST_SUITE(GetUniqueKeys,

        TEST_CASE(Invalid)

        TEST_CASE(UniqueKeys)
    )

    TEST_SUITE(GetSortedRecords,

        TEST_CASE(Invalid)

        TEST_CASE(GetRecords)

        TEST_CASE(GetRecordsReverse)

        TEST_CASE(GetNotAllRecords)

        TEST_CASE(WrongKey)
    )

    TEST_SUITE(SortBy,

        TEST_CASE(Invalid)

        TEST_CASE(Sort)

        TEST_CASE(SortReverse)

        TEST_CASE(SortNotAllRecords)

        TEST_CASE(WrongKey)
    )

    TEST_SUITE(ToJson,

        TEST_CASE(Invalid)

        TEST_CASE(Default)

        TEST_CASE(Format)

        TEST_CASE(DiffTabSize)

        TEST_CASE(RecordTemplate)

        TEST_CASE(Array)

        TEST_CASE(ArrayFormat)

        TEST_CASE(ArrayDiffTabSize)

        TEST_CASE(RecordId)
    )

    TEST_SUITE(ToStrings,

        TEST_CASE(Invalid)

        TEST_CASE(EmptySet)

        TEST_CASE(KeysWithoutRecords)

        TEST_CASE(FilledVault)
    )

    TEST_SUITE(Print,

        TEST_CASE(Invalid)

        TEST_CASE(Empty)

        TEST_CASE(KeysWithoutRecords)

        TEST_CASE(FilledVaultWithDefaultArgs)

        TEST_CASE(NotAllRecords)

        TEST_CASE(PrimaryKey)

        TEST_CASE(Reverse)

        TEST_CASE(NotAllKeys)

        TEST_CASE(PrintId)
    )

    TEST_SUITE(Join,
        
        TEST_CASE(JoinInvalidToInvalid)

        TEST_CASE(JoinValidToInvalid)

        TEST_CASE(JoinInvalidToValid)

        TEST_CASE(JoinFilledToEmpty)

        TEST_CASE(JoinEmptyToFilled)

        TEST_CASE(JoinFilled)

        TEST_CASE(WrongParentVault)

        TEST_CASE(Self)
    )

    TEST_SUITE(Exclude,
    
        TEST_CASE(ExcludeInvalidFromInvalid)

        TEST_CASE(ExcludeValidFromInvalid)

        TEST_CASE(ExcludeInvalidFromValid)

        TEST_CASE(ExcludeFilledFromEmpty)

        TEST_CASE(ExcludeEmptyFromFilled)

        TEST_CASE(ExcludeFilled)

        TEST_CASE(ExcludeDifferentParrent)

        TEST_CASE(Self)
    )

    TEST_SUITE(Intersect,
    
        TEST_CASE(IntersectInvalidWithInvalid)

        TEST_CASE(IntersectInvalidWithValid)

        TEST_CASE(IntersectValidWithInvalid)

        TEST_CASE(IntersectFilledFromEmpty)

        TEST_CASE(IntersectEmptyFromFilled)

        TEST_CASE(IntersectFilled)

        TEST_CASE(ExcludeDifferentParrent)

        TEST_CASE(Self)
    )

    TEST_SUITE(SaveToFile,

        TEST_CASE(Empty)

        TEST_CASE(KeysWithoutRecords)

        TEST_CASE(FilledVault)

        TEST_CASE(ReverseNotAllKeys)

        TEST_CASE(Separator)

        TEST_CASE(NotSaveKeys)

        TEST_CASE(FailedToOpenFile)
    )

    TEST_SUITE(Destructor,
    
        TEST_CASE(Default)
    )

    TEST_SUITE(CompareOperator,
    
        TEST_CASE(InvalidWithInvalid)

        TEST_CASE(InvalidWithValid)

        TEST_CASE(ValidWithInvalid)

        TEST_CASE(Empty)

        TEST_CASE(Filled)

        TEST_CASE(DifferentData)

        TEST_CASE(DifferentParents)
    )

    TEST_SUITE(UnionSets, 
    
        TEST_CASE(InvalidWithInvalid)

        TEST_CASE(InvalidWithValid)

        TEST_CASE(ValidWithInvalid)

        TEST_CASE(Empty)

        TEST_CASE(Filled)

        TEST_CASE(DifferentParents)

        TEST_CASE(Self)
    )

    TEST_SUITE(IntersectionSets, 
    
        TEST_CASE(InvalidWithInvalid)

        TEST_CASE(InvalidWithValid)

        TEST_CASE(ValidWithInvalid)

        TEST_CASE(Empty)

        TEST_CASE(Filled)

        TEST_CASE(DifferentParents)

        TEST_CASE(RewriteTarget)

        TEST_CASE(Self)
    )
}

void VaultRecordSetUnitTests();