#include "VaultParamInput.h"

namespace mvlt
{
    VaultOperationResult VaultParamInput::SetDataToRef(const std::string& key, VaultRecordRef& refToSetData) const noexcept
    {
        return SetDataToRefFunc(key, refToSetData);
    }

    void VaultParamInput::SetDataToRecord(const std::string& key, VaultRecord* recordToSetData) const noexcept
    {
        SetDataToRecordFunc(key, recordToSetData);
    }

    std::type_index VaultParamInput::GetDataType() const noexcept
    {
        return DataType;
    }
}