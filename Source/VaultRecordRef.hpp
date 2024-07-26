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

        Mtx.lock();

        if (DataRecord == nullptr)
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::DataRecordNotValid;

            Mtx.unlock();
            return res;
        }

        // DataRecord lock Mtx to lock VaultRecord::Invalidate in Vault destructor.
        // It is necessary to prevent Vault::RecursiveReadWriteMtx from deletion 
        DataRecord->Mtx.lock();

        // Check if Vault still accessable
        if (!DataRecord->GetIsValid())
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::DataRecordNotValid;

            DataRecord->Mtx.unlock();
            Mtx.unlock();
            return res;
        }

        Vlt->RecursiveReadWriteMtx.WriteLock();

        res = Vlt->SetDataToRecord(DataRecord, key, data);

        Vlt->RecursiveReadWriteMtx.WriteUnlock();
        DataRecord->Mtx.unlock();
        Mtx.unlock();

        return res;
    }

    template <class T>
    VaultOperationResult VaultRecordRef::GetData(const std::string& key, T& data) const noexcept
    {
        // Fill res info known at start
        VaultOperationResult res;
        res.Key = key;
        res.RequestedType = typeid(T);

        Mtx.lock();        

        if (DataRecord == nullptr)
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::DataRecordNotValid;

            Mtx.unlock();
            return res;
        }

        // DataRecord lock Mtx to lock VaultRecord::Invalidate in Vault destructor.
        // It is necessary to prevent Vault::RecursiveReadWriteMtx from deletion 
        DataRecord->Mtx.lock();

        // Check if Vault still accessable
        if (!DataRecord->GetIsValid())
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::DataRecordNotValid;

            DataRecord->Mtx.unlock();
            Mtx.unlock();
            return res;
        }

        Vlt->RecursiveReadWriteMtx.ReadLock();

        // If key not exist
        if(!Vlt->GetKeyType(key, res.SavedType))
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::WrongKey;

            Vlt->RecursiveReadWriteMtx.ReadUnlock();
            DataRecord->Mtx.unlock();
            Mtx.unlock();
            return res;
        }

        // Check types
        if (res.SavedType != res.RequestedType)
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::WrongType;

            Vlt->RecursiveReadWriteMtx.ReadUnlock();
            DataRecord->Mtx.unlock();
            Mtx.unlock();
            return res;
        }

        DataRecord->GetData(key, data);
        res.IsOperationSuccess = true;
        res.ResultCode = VaultOperationResultCode::Success;

        Vlt->RecursiveReadWriteMtx.ReadUnlock();
        DataRecord->Mtx.unlock();
        Mtx.unlock();

        return res;
    }
}