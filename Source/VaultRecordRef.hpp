#pragma once

#include "VaultRecordRef.h"

// This file contains an implementation of the VaultRecordRef template methods
namespace mvlt 
{
    template <class T>
    VaultOperationResult VaultRecordRef::SetData(const std::string& key, const T& data)
    {
        VaultOperationResult res;

        Mtx.lock();

        if (!IsValid()) 
        {
            res.Key = key;
            res.RequestedType = typeid(T);
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::DataRecordNotValid;
            Mtx.unlock();
            return res;
        }

        Vlt->RecursiveReadWriteMtx.WriteLock();

        res = Vlt->SetDataToRecord(DataRecord, key, data);
        
        Vlt->RecursiveReadWriteMtx.WriteUnlock();
        Mtx.unlock();

        return res;
    }

    template <class T>
    VaultOperationResult VaultRecordRef::GetData(const std::string& key, T& data) const
    {
        // Fill res info known at start
        VaultOperationResult res;
        res.Key = key;
        res.RequestedType = typeid(T);

        Mtx.lock();        

        if (!IsValid()) 
        {
            res.Key = key;
            res.RequestedType = typeid(T);
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::DataRecordNotValid;
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
            Mtx.unlock();
            return res;
        }

        // Check types
        if (res.SavedType != res.RequestedType)
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::WrongType;

            Vlt->RecursiveReadWriteMtx.ReadUnlock();
            Mtx.unlock();
            return res;
        }

        DataRecord->GetData(key, data);
        res.IsOperationSuccess = true;
        res.ResultCode = VaultOperationResultCode::Success;

        Vlt->RecursiveReadWriteMtx.ReadUnlock();
        Mtx.unlock();

        return res;
    }
}