#include "VaultParamInput.h"

namespace mvlt
{
    VaultOperationResult VaultParamInput::SetDataToRef(const std::string& key, VaultRecordRef& refToSetData) const noexcept
    {
        DBG_LOG_ENTER();

        return SetDataToRefFunc(key, refToSetData);
    }

    void VaultParamInput::SetDataToRecord(const std::string& key, VaultRecord* recordToSetData) const noexcept
    {
        DBG_LOG_ENTER();

        SetDataToRecordFunc(key, recordToSetData);
    }

    std::type_index VaultParamInput::GetDataType() const noexcept
    {
        DBG_LOG_ENTER();

        return DataType;
    }
}