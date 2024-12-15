#pragma once

#include "VaultRecordRef.h"
#include "VaultParamInput.hpp"

// This file contains an implementation of the VaultRecordRef template methods
namespace mvlt 
{
    template <class T>
    VaultOperationResult VaultRecordRef::SetData(const std::string& key, const T& data) noexcept
    {
        static_assert(!std::is_array<T>::value, "It is not possible to use a c array as a key value. \n\
            If you want to use a string as a key, you must specialize the function with a string. Like this: \n\
            SetData<std::string>(\"Key\", \"Value\") or SetData(\"Key\", std::string(\"Value\"))");

        VaultOperationResult res;
        res.Key = key;
        res.RequestedType = typeid(T);

        if (VaultRecordPtr == nullptr)
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::DataRecordNotValid;
            return res;
        }

        WriteLock<RecursiveReadWriteMutex> Lock(Vlt->RecursiveReadWriteMtx);

        // Check if Vault still accessable
        if (!VaultRecordPtr->GetIsValid())
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::DataRecordNotValid;
            return res;
        }

        res = Vlt->SetDataToRecord(VaultRecordPtr, key, data);

        return res;
    }

    template <class T>
    VaultOperationResult VaultRecordRef::GetData(const std::string& key, T& data) const noexcept
    {
        static_assert(!std::is_array<T>::value, "It is not possible to use a c array as a key value. \n\
            If you want to use a string as a key, you must specialize the function with a string. Like this: \n\
            GetData<std::string>(\"Key\", \"Value\") or GetData(\"Key\", std::string(\"Value\"))");

        // Fill res info known at start
        VaultOperationResult res;
        res.Key = key;
        res.RequestedType = typeid(T);


        if (VaultRecordPtr == nullptr)
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::DataRecordNotValid;

            return res;
        }

        ReadLock<RecursiveReadWriteMutex> readLock(Vlt->RecursiveReadWriteMtx);

        // Check if Vault still accessable
        if (!VaultRecordPtr->GetIsValid())
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::DataRecordNotValid;
            return res;
        }

        // If key not exist
        if(!Vlt->GetKeyType(key, res.SavedType))
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::WrongKey;
            return res;
        }

        // Check types
        if (res.SavedType != res.RequestedType)
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::WrongType;
            return res;
        }

        VaultRecordPtr->GetData(key, data);
        res.IsOperationSuccess = true;
        res.ResultCode = VaultOperationResultCode::Success;
        return res;
    }
}