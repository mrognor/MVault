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
    bool VaultRecordRef::GetData(const std::string& key, T& data) const
    {
        bool res = false;
        
        Vlt->RecursiveReadWriteMtx.ReadLock();
        Mtx.lock();

        if (DataRecord != nullptr && DataRecord->GetIsValid()) res = DataRecord->GetData(key, data);
        
        Mtx.unlock();
        Vlt->RecursiveReadWriteMtx.ReadUnlock();

        return res;
    }
}