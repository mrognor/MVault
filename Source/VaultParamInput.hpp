#pragma once

#include "VaultParamInput.h"

#include "VaultRecordRef.hpp"

namespace mvlt
{
    template <class T>
    VaultParamInput::VaultParamInput(const T& data) : DataType(typeid(T))
    {
        SetDataToRefFunc = [=](const std::string& key, VaultRecordRef& refToSetData)
        {
            return refToSetData.SetData(key, data);
        };

        SetDataToRecordFunc = [=](const std::string& key, VaultRecord* recordToSetData)
        {
            recordToSetData->SetData(key, data);
        };
    }
}