#include "VaultRecordSet.h"
#include "VaultRecordSet.hpp"

namespace mvlt
{
    VaultRecordSet::VaultRecordSet() noexcept 
    {
        VaultDerivedClass = VaultDerivedClasses::VaultRecordSetDerived;
    }

    VaultRecordSet::VaultRecordSet(const VaultRecordSet& other) noexcept
    {
        *this = other;
    }

    VaultRecordSet& VaultRecordSet::operator=(const VaultRecordSet& other) noexcept
    {
        if (&other != this)
        {
            other.RecursiveReadWriteMtx.ReadLock();
            RecursiveReadWriteMtx.WriteLock();

            VaultDerivedClass = VaultDerivedClasses::VaultRecordSetDerived;
            ParentVault = other.ParentVault;
            IsParentVaultValid = true;
            KeysOrder = other.KeysOrder;

            for (auto& keyCopierIt : other.VaultKeyCopiers)
                keyCopierIt.second(*this);

            other.ParentVault->RecordSetsSet.emplace(this);

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

    bool VaultRecordSet::GetIsParentVaultValid() const noexcept
    {
        bool res;
        RecursiveReadWriteMtx.ReadLock();
        res = IsParentVaultValid;
        RecursiveReadWriteMtx.ReadUnlock();
        return res;
    }

    std::string VaultRecordSet::GetParentVaultUniqueId() const noexcept
    {
        std::stringstream ss;

        RecursiveReadWriteMtx.ReadLock();

        if (IsParentVaultValid)
            ss << ParentVault;
        else
            ss << "null";

        RecursiveReadWriteMtx.ReadUnlock();
        
        return ss.str();
    }

    bool VaultRecordSet::IsKeyExist(const std::string& key) const noexcept
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

    bool VaultRecordSet::GetKeyType(const std::string& key, std::type_index& keyType) const noexcept
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

    VaultOperationResult VaultRecordSet::AddRecord(const VaultRecordRef& recordRef) noexcept
    {
        VaultOperationResult res;

        RecursiveReadWriteMtx.WriteLock();

        // Thread safety because in Vault destructor blocking this RecursiveReadWriteMtx to write
        if (IsParentVaultValid)
        {
            ParentVault->RecursiveReadWriteMtx.ReadLock();
            recordRef.ReadLock();

            // Check if vault record ref depends on same vault as this
            if (recordRef.Vlt == ParentVault)
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

                res.IsOperationSuccess = true;
                res.ResultCode = VaultOperationResultCode::Success;
            }
            else 
            {
                res.IsOperationSuccess = false;
                res.ResultCode = VaultOperationResultCode::ParentVaultNotMatch;
            }
            
            recordRef.ReadUnlock();
            ParentVault->RecursiveReadWriteMtx.ReadUnlock();
        }
        else 
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::ParentVaultNotValid;
        }

        RecursiveReadWriteMtx.WriteUnlock();

        return res;
    }

    void VaultRecordSet::Reset() noexcept
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

    void VaultRecordSet::Clear() noexcept
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

    bool VaultRecordSet::RemoveRecord(const VaultRecordRef& recordRefToErase) noexcept
    {
        return Vault::EraseRecord(recordRefToErase);
    }

    std::size_t VaultRecordSet::Size() const noexcept
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

    std::vector<std::string> VaultRecordSet::GetKeys() const noexcept
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

    std::vector<VaultRecordRef> VaultRecordSet::GetSortedRecords(const std::string& key, const bool& isReverse, const std::size_t& amountOfRecords) const noexcept
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

    void VaultRecordSet::PrintSet(const std::size_t& amountOfRecords) const noexcept
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

    void VaultRecordSet::PrintAsTable(bool isPrintId, const std::size_t& amountOfRecords, std::string primaryKey, const bool& isReverse,
            const std::list<std::string> keys) const noexcept
        {
        RecursiveReadWriteMtx.ReadLock();

        // Thread safety because in Vault destructor blocking this RecursiveReadWriteMtx to write
        if (IsParentVaultValid)
        {
            ParentVault->RecursiveReadWriteMtx.ReadLock();
            Vault::PrintAsTable(isPrintId, amountOfRecords, primaryKey, isReverse, keys);
            ParentVault->RecursiveReadWriteMtx.ReadUnlock();
        }
        else
            std::cout << "The parent Vault is not valid!" << std::endl;

        RecursiveReadWriteMtx.ReadUnlock();
    }

    void VaultRecordSet::Join(const VaultRecordSet& a) noexcept
    {
        a.RecursiveReadWriteMtx.ReadLock();
        RecursiveReadWriteMtx.WriteLock();

        // Thread safety because in Vault destructor blocking this RecursiveReadWriteMtx to write
        if (IsParentVaultValid && a.IsParentVaultValid)
        {
            ParentVault->RecursiveReadWriteMtx.ReadLock();

            for (VaultRecord* record : a.RecordsSet)
            {
                if (RecordsSet.find(record) == RecordsSet.end())
                {
                    RecordsSet.emplace(record);

                    for (auto& adder : VaultRecordAdders)
                        adder.second(record);
                }
            }

            ParentVault->RecursiveReadWriteMtx.ReadUnlock();
        }

        RecursiveReadWriteMtx.WriteUnlock();
        a.RecursiveReadWriteMtx.ReadUnlock();
    }

    void VaultRecordSet::Exclude(const VaultRecordSet& a) noexcept
    {
        a.RecursiveReadWriteMtx.ReadLock();
        RecursiveReadWriteMtx.WriteLock();

        // Thread safety because in Vault destructor blocking this RecursiveReadWriteMtx to write
        if (IsParentVaultValid && a.IsParentVaultValid)
        {
            ParentVault->RecursiveReadWriteMtx.ReadLock();

            for (auto it = RecordsSet.begin(); it != RecordsSet.end();)
            {
                // if found record in a then delete it here
                if (a.RecordsSet.find(*it) != a.RecordsSet.end())
                    it = Vault::RemoveRecord(*it, nullptr);
                else
                    ++it;
            }

            ParentVault->RecursiveReadWriteMtx.ReadUnlock();
        }

        RecursiveReadWriteMtx.WriteUnlock();
        a.RecursiveReadWriteMtx.ReadUnlock();
    }

    void VaultRecordSet::Intersect(const VaultRecordSet& a) noexcept
    {
        a.RecursiveReadWriteMtx.ReadLock();
        RecursiveReadWriteMtx.WriteLock();

        // Thread safety because in Vault destructor blocking this RecursiveReadWriteMtx to write
        if (IsParentVaultValid && a.IsParentVaultValid)
        {
            ParentVault->RecursiveReadWriteMtx.ReadLock();

            for (auto it = RecordsSet.begin(); it != RecordsSet.end();)
            {
                // if not found record in a then delete it here
                if (a.RecordsSet.find(*it) == a.RecordsSet.end())
                    it = Vault::RemoveRecord(*it, nullptr);
                else
                    ++it;
            }

            ParentVault->RecursiveReadWriteMtx.ReadUnlock();
        }

        RecursiveReadWriteMtx.WriteUnlock();
        a.RecursiveReadWriteMtx.ReadUnlock();
    }

    bool VaultRecordSet::SaveToFile(const std::string& fileName, const std::string& separator, const bool& isSaveKey) const noexcept
    {
        bool res = false;
        RecursiveReadWriteMtx.ReadLock();

        // Thread safety because in Vault destructor blocking this RecursiveReadWriteMtx to write
        if (IsParentVaultValid)
        {
            ParentVault->RecursiveReadWriteMtx.ReadLock();
            res = Vault::SaveToFile(fileName);
            ParentVault->RecursiveReadWriteMtx.ReadUnlock();
        }

        RecursiveReadWriteMtx.ReadUnlock();
        return res;
    }

    VaultRecordSet::~VaultRecordSet() noexcept
    {
        Reset();
    }


    bool operator==(const VaultRecordSet& a, const VaultRecordSet& b)
    {
        bool res = false;
        a.RecursiveReadWriteMtx.ReadLock();
        b.RecursiveReadWriteMtx.ReadLock();

        if (a.ParentVault == b.ParentVault && a.RecordsSet == b.RecordsSet)
            res = true;

        b.RecursiveReadWriteMtx.ReadUnlock();
        a.RecursiveReadWriteMtx.ReadUnlock();

        return res;
    }
}