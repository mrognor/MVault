#include "VaultRequestResult.h"

namespace mvlt
{
    VaultRequestResult::VaultRequestResult() {}

    VaultRequestResult::VaultRequestResult(const VaultRequestResult& other)
    {
        *this = other;
    }

    VaultRequestResult& VaultRequestResult::operator=(const VaultRequestResult& other)
    {
        if (&other != this)
        {
            ParentVault = other.ParentVault;

            for (auto& keyCopierIt : other.VaultKeyCopiers)
                keyCopierIt.second(*this);
        
            for (VaultRecord* record : other.RecordsSet)
            {
                RecordsSet.emplace(record);
                
                for (auto& adder : VaultRecordAdders)
                    adder.second(record);

                // Lock VaultRecord to thread safety add new dependent VaultRequestResult
                record->Mtx.lock();
                record->dependentVaultRequestResults.emplace(this);
                record->Mtx.unlock();
            }
        }
        
        return *this;
    }

    void VaultRequestResult::Clear()
    {
        RecursiveReadWriteMtx.WriteLock();

        // Remove this from records
        for (VaultRecord* record : RecordsSet)
        {
            record->Mtx.lock();
            record->dependentVaultRequestResults.erase(this);
            record->Mtx.unlock();
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
        ParentVault->RequestsResultsSet.erase(this);

        ParentVault->RecursiveReadWriteMtx.WriteUnlock();
        RecursiveReadWriteMtx.WriteUnlock();
    }
}