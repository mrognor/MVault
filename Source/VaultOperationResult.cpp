#include "VaultOperationResult.h"

namespace mvlt
{
    std::string VaultOperationResult::ResultCodeString() const noexcept
    {
        switch (ResultCode)
        {
        default:
        case VaultOperationResultCode::TryToAddUniqueKeyInNonEmptyVaultWithoutLambda:
            return "You can not add unique key without lambda in non-empty vault";
             
        case VaultOperationResultCode::DuplicateKey:
            return "This key is alredy in vault";

        case VaultOperationResultCode::TryToUpdateUniqueKey:
            return "The key is unique and you can not update it default value.";

        case VaultOperationResultCode::DataRecordNotValid:
            return "The record referenced by VaultRecordRef is not valid.";

        case VaultOperationResultCode::WrongType:
            return "The mismatch between the types of the requested key and the saved one.";
        
        case VaultOperationResultCode::WrongKey:
            return "The requested key was not found.";

        case VaultOperationResultCode::Idle:
            return "Idle state. The operation was not performed.";

        case VaultOperationResultCode::UniqueKeyValueAlredyInSet:
            return "Unique key value alredy in set";

        case VaultOperationResultCode::Success:
            return "The operation was completed successfully.";

        case VaultOperationResultCode::WrongValue:
            return "A record with this key value was not found.";
        }
    }

    bool operator== (const VaultOperationResult& resA, const VaultOperationResult& resB) noexcept
    {
        return (resA.IsOperationSuccess == resB.IsOperationSuccess) && (resA.ResultCode == resB.ResultCode) &&
            (resA.Key == resB.Key) && (resA.RequestedType == resB.RequestedType) &&
            (resA.SavedType == resB.SavedType);
    }
}