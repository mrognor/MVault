#pragma once

#include "VaultParamInput.h"

#include "VaultRecordRef.hpp"

namespace mvlt
{
    template <class T>
    VaultParamInput::VaultParamInput(const T& data) noexcept : DataType(typeid(T))
    {
        static_assert(!std::is_array<T>::value, "It is not possible to use a c array as a key value. \n\
            If you want to use a string as a key, you must specialize the function with a string. Like this: \n\
            AddKey<std::string>(\"Key\", \"Value\") or AddKey(\"Key\", std::string(\"Value\"))");

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