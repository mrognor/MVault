#pragma once

#include "VaultRecordRef.h"

// This file contains an implementation of the VaultRecordRef template methods
namespace mvlt 
{
    template <class T>
    VaultOperationResult VaultRecordRef::SetData(const std::string& key, const T& data)
    {
        VaultOperationResult res;

        Vlt->RecursiveReadWriteMtx.WriteLock();
        Mtx.lock();

        res = Vlt->SetDataToRecord(DataRecord, key, data);

        Mtx.unlock();
        Vlt->RecursiveReadWriteMtx.WriteUnlock();

        return res;
    }

    template <class T>
    VaultOperationResult VaultRecordRef::GetData(const std::string& keyName, T& data) const
    {
        // Fill res info known at start
        VaultOperationResult res;
        res.Key = keyName;
        res.RequestedType = typeid(T);
        
        Vlt->RecursiveReadWriteMtx.ReadLock();
        Mtx.lock();

        if (DataRecord == nullptr || !DataRecord->GetIsValid()) 
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::DataRecordNotValid;
            Mtx.unlock();
            Vlt->RecursiveReadWriteMtx.ReadUnlock();
            return res;
        }

        // If key not exist
        if(!Vlt->GetKeyType(keyName, res.SavedType))
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::WrongKey;
            Mtx.unlock();
            Vlt->RecursiveReadWriteMtx.ReadUnlock();
            return res;
        }

        // Check types
        if (res.SavedType != res.RequestedType)
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::WrongType;
            Mtx.unlock();
            Vlt->RecursiveReadWriteMtx.ReadUnlock();
            return res;
        }

        DataRecord->GetData(keyName, data);
        res.IsOperationSuccess = true;
        res.ResultCode = VaultOperationResultCode::Success;
        Mtx.unlock();
        Vlt->RecursiveReadWriteMtx.ReadUnlock();

        return res;
    }
}