#include "VaultRecordSet.h"

namespace mvlt
{
    VaultRecordSet::VaultRecordSet() {}

    VaultRecordSet::VaultRecordSet(const VaultRecordSet& other)
    {
        *this = other;
    }

    VaultRecordSet& VaultRecordSet::operator=(const VaultRecordSet& other)
    {
        if (&other != this)
        {
            other.RecursiveReadWriteMtx.ReadLock();
            RecursiveReadWriteMtx.WriteLock();

            ParentVault = other.ParentVault;
            IsParentVaultValid = true;
            
            for (auto& keyCopierIt : other.VaultKeyCopiers)
                keyCopierIt.second(*this);
        
            for (VaultRecord* record : other.RecordsSet)
            {
                RecordsSet.emplace(record);
                
                for (auto& adder : VaultRecordAdders)
                    adder.second(record);

                // Lock VaultRecord to thread safety add new dependent VaultRecordSet
                record->Mtx.lock();
                record->dependentVaultRecordSets.emplace(this);
                record->Mtx.unlock();
            }

            RecursiveReadWriteMtx.WriteUnlock();
            other.RecursiveReadWriteMtx.ReadUnlock();
        }
        
        return *this;
    }

    bool VaultRecordSet::GetIsParentVaultValid() const
    {
        bool res;
        RecursiveReadWriteMtx.ReadLock();
        res = IsParentVaultValid;
        RecursiveReadWriteMtx.ReadUnlock();
        return res;
    }

    bool VaultRecordSet::IsKeyExist(const std::string& key) const
    {
        bool res = false;
        RecursiveReadWriteMtx.ReadLock();

        // Thread safety because in Vault destructor blocking this RecursiveReadWriteMtx to write
        if (IsParentVaultValid)
        {
            ParentVault->RecursiveReadWriteMtx.ReadLock();
            res = Vault::IsKeyExist(key);
            ParentVault->RecursiveReadWriteMtx.ReadUnlock();
        }
        
        RecursiveReadWriteMtx.ReadUnlock();
        return res;
    }

    bool VaultRecordSet::GetKeyType(const std::string& key, std::type_index& keyType) const
    {
        bool res = false;
        RecursiveReadWriteMtx.ReadLock();

        // Thread safety because in Vault destructor blocking this RecursiveReadWriteMtx to write
        if (IsParentVaultValid)
        {
            ParentVault->RecursiveReadWriteMtx.ReadLock();
            res = Vault::GetKeyType(key, keyType);
            ParentVault->RecursiveReadWriteMtx.ReadUnlock();
        }
        
        RecursiveReadWriteMtx.ReadUnlock();
        return res;
    }

    void VaultRecordSet::AddRecord(const VaultRecordRef& recordRef)
    {
        RecursiveReadWriteMtx.WriteLock();

        // Thread safety because in Vault destructor blocking this RecursiveReadWriteMtx to write
        if (IsParentVaultValid)
        {
            ParentVault->RecursiveReadWriteMtx.ReadLock();

            // Add pointer to record from recordRef to this
            RecordsSet.emplace(recordRef.DataRecord);
                
            // Add pointer to record from recordRef to this::vaultRecordRef structure
            for (auto& adder : VaultRecordAdders)
                adder.second(recordRef.DataRecord);

            // Lock VaultRecord to thread safety add new dependent VaultRecordSet
            recordRef.DataRecord->Mtx.lock();
            recordRef.DataRecord->dependentVaultRecordSets.emplace(this);
            recordRef.DataRecord->Mtx.unlock();

            ParentVault->RecursiveReadWriteMtx.ReadUnlock();    
        }
        
        RecursiveReadWriteMtx.WriteUnlock();
    }

    void VaultRecordSet::AddRecords(const std::vector<VaultRecordRef> recordsRefs)
    {
        RecursiveReadWriteMtx.WriteLock();

        // Thread safety because in Vault destructor blocking this RecursiveReadWriteMtx to write
        if (IsParentVaultValid)
        {
            ParentVault->RecursiveReadWriteMtx.ReadLock();

            for (auto& recordRef : recordsRefs)
            {
                // Add pointer to record from recordRef to this
                RecordsSet.emplace(recordRef.DataRecord);
                    
                // Add pointer to record from recordRef to this::vaultRecordRef structure
                for (auto& adder : VaultRecordAdders)
                    adder.second(recordRef.DataRecord);

                // Lock VaultRecord to thread safety add new dependent VaultRecordSet
                recordRef.DataRecord->Mtx.lock();
                recordRef.DataRecord->dependentVaultRecordSets.emplace(this);
                recordRef.DataRecord->Mtx.unlock();
            }

            ParentVault->RecursiveReadWriteMtx.ReadUnlock();    
        }
        
        RecursiveReadWriteMtx.WriteUnlock();
    }

    void VaultRecordSet::Reset()
    {
        RecursiveReadWriteMtx.WriteLock();

        if (IsParentVaultValid)
        {
            ParentVault->RecursiveReadWriteMtx.WriteLock();
            Clear();
            Vault::DropVault();

            ParentVault->RecordSetsSet.erase(this);
            ParentVault->RecursiveReadWriteMtx.WriteUnlock();
            ParentVault = nullptr;
            IsParentVaultValid = false;
        }

        RecursiveReadWriteMtx.WriteUnlock();
    }

    void VaultRecordSet::Clear()
    {
        RecursiveReadWriteMtx.WriteLock();

        // Remove this from records
        for (VaultRecord* record : RecordsSet)
        {
            record->Mtx.lock();
            record->dependentVaultRecordSets.erase(this);
            record->Mtx.unlock();
        }

        // Clear structure
        for (auto& it : VaultRecordClearers)
            it.second();

        // No need to delete each record because it is RecordSet and records will be delete in original vault
        RecordsSet.clear();

        RecursiveReadWriteMtx.WriteUnlock();
    }

    bool VaultRecordSet::RemoveRecord(const VaultRecordRef& recordRefToErase)
    {
        return Vault::RemoveRecord(false, recordRefToErase);
    }

    std::size_t VaultRecordSet::Size() const
    {
        std::size_t res = 0;
        RecursiveReadWriteMtx.ReadLock();

        // Thread safety because in Vault destructor blocking this RecursiveReadWriteMtx to write
        if (IsParentVaultValid)
        {
            ParentVault->RecursiveReadWriteMtx.ReadLock();
            res = Vault::Size();
            ParentVault->RecursiveReadWriteMtx.ReadUnlock();
        }

        RecursiveReadWriteMtx.ReadUnlock();
        return res;
    }

    std::vector<std::string> VaultRecordSet::GetKeys() const
    {
        std::vector<std::string> res;
        RecursiveReadWriteMtx.ReadLock();

        // Thread safety because in Vault destructor blocking this RecursiveReadWriteMtx to write
        if (IsParentVaultValid)
        {
            ParentVault->RecursiveReadWriteMtx.ReadLock();
            res = Vault::GetKeys();
            ParentVault->RecursiveReadWriteMtx.ReadUnlock();
        }

        RecursiveReadWriteMtx.ReadUnlock();
        return res;
    }

    std::vector<VaultRecordRef> VaultRecordSet::GetSortedRecords(const std::string& key, const bool& isReverse, const std::size_t& amountOfRecords) const
    {
        std::vector<VaultRecordRef> res;
        RecursiveReadWriteMtx.ReadLock();

        // Thread safety because in Vault destructor blocking this RecursiveReadWriteMtx to write
        if (IsParentVaultValid)
        {
            ParentVault->RecursiveReadWriteMtx.ReadLock();
            res = Vault::GetSortedRecords(key, isReverse, amountOfRecords);
            ParentVault->RecursiveReadWriteMtx.ReadUnlock();
        }

        RecursiveReadWriteMtx.ReadUnlock();
        return res;
    }

    void VaultRecordSet::PrintVault(const std::size_t& amountOfRecords) const
    {
        RecursiveReadWriteMtx.ReadLock();

        // Thread safety because in Vault destructor blocking this RecursiveReadWriteMtx to write
        if (IsParentVaultValid)
        {
            ParentVault->RecursiveReadWriteMtx.ReadLock();
            Vault::PrintVault(amountOfRecords);
            ParentVault->RecursiveReadWriteMtx.ReadUnlock();
        }
        else 
            std::cout << "The parent Vault is not valid!" << std::endl;
        
        RecursiveReadWriteMtx.ReadUnlock();
    }

    void VaultRecordSet::PrintAsTable(bool isPrintId, const std::size_t& amountOfRecords, const std::vector<std::string> keys) const
    {
        RecursiveReadWriteMtx.ReadLock();

        // Thread safety because in Vault destructor blocking this RecursiveReadWriteMtx to write
        if (IsParentVaultValid)
        {
            ParentVault->RecursiveReadWriteMtx.ReadLock();
            Vault::PrintAsTable(isPrintId, amountOfRecords, keys);
            ParentVault->RecursiveReadWriteMtx.ReadUnlock();
        }
        else 
            std::cout << "The parent Vault is not valid!" << std::endl;
        
        RecursiveReadWriteMtx.ReadUnlock();
    }

    VaultRecordSet::~VaultRecordSet()
    {
        Reset();
    }
}