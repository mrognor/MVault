#include "VaultParamInput.h"

namespace mvlt
{
    VaultOperationResult VaultParamInput::SetDataToRef(const std::string& key, VaultRecordRef& refToSetData) const
    {
        return SetDataToRefFunc(key, refToSetData);
    }

    void VaultParamInput::SetDataToRecord(const std::string& key, VaultRecord* recordToSetData) const
    {
        SetDataToRecordFunc(key, recordToSetData);
    }

    std::type_index VaultParamInput::GetDataType() const
    {
        return DataType;
    }
}