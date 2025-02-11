#include "VaultOperationResult.h"

namespace mvlt
{
    void VaultOperationResult::SetOpResult(const VaultOperationResultCode& resultCode) noexcept
    {
        DBG_LOG_ENTER();

        ResultCode = resultCode;

        #ifndef NDEBUG
        if (TraceFormat != BackTraceFormat::None)
        {
            std::cout << "Internal error! " << ResultCodeString() << "\n";
            std::cout << "\tRequested key: \x1B[31m" << Key << "\033[0m. Requested type: \x1B[31m" << RequestedType.name() << "\033[0m. SavedType: \x1B[31m" << SavedType.name() << "\033[0m.\n";
            PrintBackTrace();
        }
        #endif
    }

    std::string VaultOperationResult::ResultCodeString() const noexcept
    {
        DBG_LOG_ENTER();

        switch (ResultCode)
        {
        case VaultOperationResultCode::OtherParentVaultNotValid:
            return "The parent Vault of another VaultRecordSet is not valid.";

        case VaultOperationResultCode::SameVaultRecordSet:
            return "The operation is performed on the same VaultRecordSet object.";

        case VaultOperationResultCode::TryToAddUniqueKeyInNonEmptyVaultWithoutLambda:
            return "You can not add unique key without lambda in non-empty vault";
             
        case VaultOperationResultCode::DuplicateKey:
            return "This key is alredy in vault";

        case VaultOperationResultCode::TryToUpdateUniqueKey:
            return "The key is unique and you can not update it default value.";

        case VaultOperationResultCode::RecordAlredyInSet:
            return "The record is already in VaultRecordSet.";

        case VaultOperationResultCode::ParentVaultNotMatch:
            return "The parent Vaults don't match.";

        case VaultOperationResultCode::ParentVaultNotValid:
            return "Parent Vault is not valid.";

        case VaultOperationResultCode::DataRecordNotValid:
            return "The record referenced by VaultRecordRef is not valid.";
   
        case VaultOperationResultCode::WrongKey:
            return "The requested key was not found.";

        case VaultOperationResultCode::WrongType:
            return "The mismatch between the types of the requested key and the saved one.";

        case VaultOperationResultCode::Idle:
            return "Idle state. The operation was not performed.";

        case VaultOperationResultCode::WrongValue:
            return "A record with this key value was not found.";

        case VaultOperationResultCode::UniqueKeyValueAlredyInSet:
            return "Unique key value alredy in set.";

        case VaultOperationResultCode::Success:
            return "The operation was completed successfully.";

        default:
            break;
        }

        return "";
    }

    bool operator== (const VaultOperationResult& resA, const VaultOperationResult& resB) noexcept
    {
        DBG_LOG_ENTER();

        return (resA.IsOperationSuccess == resB.IsOperationSuccess) && (resA.ResultCode == resB.ResultCode) &&
            (resA.Key == resB.Key) && (resA.RequestedType == resB.RequestedType) &&
            (resA.SavedType == resB.SavedType);
    }
}