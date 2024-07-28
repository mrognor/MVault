#pragma once

#include "VaultRecordRef.h"
#include "VaultParamInput.hpp"

// This file contains an implementation of the VaultRecordRef template methods
namespace mvlt 
{
    template <class T>
    VaultOperationResult VaultRecordRef::SetData(const std::string& key, const T& data) noexcept
    {
        VaultOperationResult res;
        res.Key = key;
        res.RequestedType = typeid(T);

        VaultRecordRefMutex.lock();

        if (VaultRecordPtr == nullptr)
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::DataRecordNotValid;

            VaultRecordRefMutex.unlock();
            return res;
        }

        // VaultRecordPtr lock VaultRecordRefMutex to lock VaultRecord::Invalidate in Vault destructor.
        // It is necessary to prevent Vault::RecursiveReadWriteMtx from deletion 
        VaultRecordPtr->VaultRecordMutex.lock();

        // Check if Vault still accessable
        if (!VaultRecordPtr->GetIsValid())
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::DataRecordNotValid;

            VaultRecordPtr->VaultRecordMutex.unlock();
            VaultRecordRefMutex.unlock();
            return res;
        }

        Vlt->RecursiveReadWriteMtx.WriteLock();

        res = Vlt->SetDataToRecord(VaultRecordPtr, key, data);

        Vlt->RecursiveReadWriteMtx.WriteUnlock();
        VaultRecordPtr->VaultRecordMutex.unlock();
        VaultRecordRefMutex.unlock();

        return res;
    }

    template <class T>
    VaultOperationResult VaultRecordRef::GetData(const std::string& key, T& data) const noexcept
    {
        // Fill res info known at start
        VaultOperationResult res;
        res.Key = key;
        res.RequestedType = typeid(T);

        VaultRecordRefMutex.lock();        

        if (VaultRecordPtr == nullptr)
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::DataRecordNotValid;

            VaultRecordRefMutex.unlock();
            return res;
        }

        // VaultRecordPtr lock VaultRecordRefMutex to lock VaultRecord::Invalidate in Vault destructor.
        // It is necessary to prevent Vault::RecursiveReadWriteMtx from deletion 
        VaultRecordPtr->VaultRecordMutex.lock();

        // Check if Vault still accessable
        if (!VaultRecordPtr->GetIsValid())
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::DataRecordNotValid;

            VaultRecordPtr->VaultRecordMutex.unlock();
            VaultRecordRefMutex.unlock();
            return res;
        }

        Vlt->RecursiveReadWriteMtx.ReadLock();

        // If key not exist
        if(!Vlt->GetKeyType(key, res.SavedType))
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::WrongKey;

            Vlt->RecursiveReadWriteMtx.ReadUnlock();
            VaultRecordPtr->VaultRecordMutex.unlock();
            VaultRecordRefMutex.unlock();
            return res;
        }

        // Check types
        if (res.SavedType != res.RequestedType)
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::WrongType;

            Vlt->RecursiveReadWriteMtx.ReadUnlock();
            VaultRecordPtr->VaultRecordMutex.unlock();
            VaultRecordRefMutex.unlock();
            return res;
        }

        VaultRecordPtr->GetData(key, data);
        res.IsOperationSuccess = true;
        res.ResultCode = VaultOperationResultCode::Success;

        Vlt->RecursiveReadWriteMtx.ReadUnlock();
        VaultRecordPtr->VaultRecordMutex.unlock();
        VaultRecordRefMutex.unlock();

        return res;
    }
}