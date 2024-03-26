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
            other.RecursiveReadWriteMtx.ReadLock();
            RecursiveReadWriteMtx.WriteLock();

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

            RecursiveReadWriteMtx.WriteUnlock();
            other.RecursiveReadWriteMtx.ReadUnlock();
        }
        
        return *this;
    }

    bool VaultRequestResult::GetIsParentVaultValid() const
    {
        bool res;
        RecursiveReadWriteMtx.ReadLock();
        res = IsParenVaultValid;
        RecursiveReadWriteMtx.ReadUnlock();
        return res;
    }

    bool VaultRequestResult::IsKeyExist(const std::string& key) const
    {
        bool res = false;
        RecursiveReadWriteMtx.ReadLock();

        // Thread safety because in Vault destructor blocking this RecursiveReadWriteMtx to write
        if (IsParenVaultValid)
        {
            ParentVault->RecursiveReadWriteMtx.ReadLock();
            res = Vault::IsKeyExist(key);
            ParentVault->RecursiveReadWriteMtx.ReadUnlock();
        }
        
        RecursiveReadWriteMtx.ReadUnlock();
        return res;
    }

    bool VaultRequestResult::GetKeyType(const std::string& key, std::type_index& keyType) const
    {
        bool res = false;
        RecursiveReadWriteMtx.ReadLock();

        // Thread safety because in Vault destructor blocking this RecursiveReadWriteMtx to write
        if (IsParenVaultValid)
        {
            ParentVault->RecursiveReadWriteMtx.ReadLock();
            res = Vault::GetKeyType(key, keyType);
            ParentVault->RecursiveReadWriteMtx.ReadUnlock();
        }
        
        RecursiveReadWriteMtx.ReadUnlock();
        return res;
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

    void VaultRequestResult::PrintAsTable(bool isPrintId, const std::size_t& amountOfRecords, const std::vector<std::string> keys) const
    {
        RecursiveReadWriteMtx.ReadLock();

        // Thread safety because in Vault destructor blocking this RecursiveReadWriteMtx to write
        if (IsParenVaultValid)
        {
            ParentVault->RecursiveReadWriteMtx.ReadLock();
            Vault::PrintAsTable(isPrintId, amountOfRecords, keys);
            ParentVault->RecursiveReadWriteMtx.ReadUnlock();
        }
        else 
            std::cout << "The parent Vault is not valid!" << std::endl;
        
        RecursiveReadWriteMtx.ReadUnlock();
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