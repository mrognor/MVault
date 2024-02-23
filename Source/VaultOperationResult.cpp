#include "VaultOperationResult.h"

namespace mvlt
{
    std::string VaultOperationResult::ResultCodeString() const
    {
        switch (ResultCode)
        {
        default:
        case VaultOperationResultCode::WrongType:
            return "The mismatch between the types of the requested key and the saved one.";
        
        case VaultOperationResultCode::WrongKey:
            return "The requested key was not found.";

        case VaultOperationResultCode::Idle:
            return "Idle state. The operation was not performed.";

        case VaultOperationResultCode::Success:
            return "The operation was completed successfully.";

        case VaultOperationResultCode::WrongValue:
            return "A record with this key value was not found";
        }
    }
}