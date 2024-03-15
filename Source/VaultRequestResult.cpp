#include "VaultRequestResult.h"

namespace mvlt
{
    void VaultRequestResult::CopyKeysFromVault(Vault* vlt)
    {
        ParentVault = vlt;

        // Copy keys from vlt to this
        for (auto& keyCopierIt : vlt->VaultKeyCopiers)
            keyCopierIt.second(*this);

    }

    void VaultRequestResult::AddRecord(VaultRecord* vaultRecord)
    {

        // Add pointer to record from this to vaultRequestResult
        RecordsSet.emplace(vaultRecord);
                
        // Add pointer to record from this to vaultRequestResult structure
        for (auto& adder : VaultRecordAdders)
            adder.second(vaultRecord);

        vaultRecord->Mtx->lock();
        vaultRecord->dependentVaultRequestResults.emplace(this);
        vaultRecord->Mtx->unlock();

    }

    void VaultRequestResult::Clear()
    {
        RecursiveReadWriteMtx.WriteLock();

        // Remove this from records
        for (VaultRecord* record : RecordsSet)
        {
            record->Mtx->lock();
            record->dependentVaultRequestResults.erase(this);
            record->Mtx->unlock();
        }

        // No need to delete each record because it is RequestResult and records will be delete in original vault
        RecordsSet.clear();

        RecursiveReadWriteMtx.WriteUnlock();
    }

    VaultRequestResult::~VaultRequestResult()
    {
        RecursiveReadWriteMtx.WriteLock();
        ParentVault->RecursiveReadWriteMtx.WriteLock();

        Clear();

        ParentVault->RecursiveReadWriteMtx.WriteUnlock();
        RecursiveReadWriteMtx.WriteUnlock();
    }
}