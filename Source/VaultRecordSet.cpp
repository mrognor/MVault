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
                record->VaultRecordMutex.lock();
                record->dependentVaultRecordSets.emplace(this);
                record->VaultRecordMutex.unlock();
            }
        }

        return *this;
    }

    bool VaultRecordSet::GetIsParentVaultValid() const noexcept
    {
        return IsParentVaultValid;
    }

    std::string VaultRecordSet::GetParentVaultUniqueId() const noexcept
    {
        std::stringstream ss;

        if (IsParentVaultValid)
            ss << ParentVault;
        else
            ss << "null";
        
        return ss.str();
    }

    bool VaultRecordSet::IsKeyExist(const std::string& key) const noexcept
    {
        bool res = false;

        if (IsParentVaultValid)
            res = ParentVault->IsKeyExist(key);

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

        if (IsParentVaultValid)
        {
            ParentVault->RecursiveReadWriteMtx.ReadLock();

            // Check if vault record ref depends on same vault as this
            if (recordRef.Vlt == ParentVault && recordRef.IsValid())
            {
                // Add pointer to record from recordRef to this
                std::pair<decltype(RecordsSet.begin()), bool> emplaceRes = RecordsSet.emplace(recordRef.VaultRecordPtr);
                
                if (emplaceRes.second)
                {
                    // Add pointer to record from recordRef to this::vaultRecordRef structure
                    for (auto& adder : VaultRecordAdders)
                        adder.second(recordRef.VaultRecordPtr);

                    // Lock VaultRecord to thread safety add new dependent VaultRecordSet
                    recordRef.VaultRecordPtr->VaultRecordMutex.lock();
                    recordRef.VaultRecordPtr->dependentVaultRecordSets.emplace(this);
                    recordRef.VaultRecordPtr->VaultRecordMutex.unlock();

                    res.IsOperationSuccess = true;
                    res.ResultCode = VaultOperationResultCode::Success;
                }
                else
                {
                    res.IsOperationSuccess = false;
                    res.ResultCode = VaultOperationResultCode::RecordAlredyInSet;
                }
            }
            else 
            {
                res.IsOperationSuccess = false;
                if (!recordRef.IsValid())
                    res.ResultCode = VaultOperationResultCode::DataRecordNotValid;
                else
                    res.ResultCode = VaultOperationResultCode::ParentVaultNotMatch;
            }

            ParentVault->RecursiveReadWriteMtx.ReadUnlock();
        }
        else 
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::ParentVaultNotValid;
        }

        return res;
    }

    void VaultRecordSet::Reset() noexcept
    {
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
    }

    void VaultRecordSet::Clear() noexcept
    {
        // Remove this from records
        for (VaultRecord* record : RecordsSet)
        {
            record->VaultRecordMutex.lock();
            record->dependentVaultRecordSets.erase(this);
            record->VaultRecordMutex.unlock();
        }

        // Clear structure
        for (const auto& vaultRecordClearersIt : VaultRecordClearers)
            vaultRecordClearersIt.second();

        // No need to delete each record because it is RecordSet and records will be delete in original vault
        RecordsSet.clear();
    }

    bool VaultRecordSet::RemoveRecord(const VaultRecordRef& recordRefToErase) noexcept
    {
        return Vault::EraseRecord(recordRefToErase);
    }

    std::size_t VaultRecordSet::Size() const noexcept
    {
        std::size_t res = 0;

        if (IsParentVaultValid)
        {
            ParentVault->RecursiveReadWriteMtx.ReadLock();
            res = Vault::Size();
            ParentVault->RecursiveReadWriteMtx.ReadUnlock();
        }

        return res;
    }

    std::vector<std::string> VaultRecordSet::GetKeys() const noexcept
    {
        std::vector<std::string> res;

        if (IsParentVaultValid)
            res = ParentVault->GetKeys();

        return res;
    }

    std::vector<VaultRecordRef> VaultRecordSet::GetSortedRecords(const std::string& key, const bool& isReverse, const std::size_t& amountOfRecords) const noexcept
    {
        std::vector<VaultRecordRef> res;

        if (IsParentVaultValid)
        {
            ParentVault->RecursiveReadWriteMtx.ReadLock();
            res = Vault::GetSortedRecords(key, isReverse, amountOfRecords);
            ParentVault->RecursiveReadWriteMtx.ReadUnlock();
        }

        return res;
    }

    void VaultRecordSet::PrintSet(const std::size_t& amountOfRecords) const noexcept
    {
        if (IsParentVaultValid)
        {
            ParentVault->RecursiveReadWriteMtx.ReadLock();
            Vault::PrintVault(amountOfRecords);
            ParentVault->RecursiveReadWriteMtx.ReadUnlock();
        }
        else
            std::cout << "The parent Vault is not valid!" << std::endl;
    }

    void VaultRecordSet::PrintAsTable(bool isPrintId, const std::size_t& amountOfRecords, std::string primaryKey, const bool& isReverse,
            const std::list<std::string> keys) const noexcept
        {
        if (IsParentVaultValid)
        {
            ParentVault->RecursiveReadWriteMtx.ReadLock();
            Vault::PrintAsTable(isPrintId, amountOfRecords, primaryKey, isReverse, keys);
            ParentVault->RecursiveReadWriteMtx.ReadUnlock();
        }
        else
            std::cout << "The parent Vault is not valid!" << std::endl;
    }

    void VaultRecordSet::Join(const VaultRecordSet& a) noexcept
    {
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
    }

    void VaultRecordSet::Exclude(const VaultRecordSet& a) noexcept
    {
        if (IsParentVaultValid && a.IsParentVaultValid)
        {
            ParentVault->RecursiveReadWriteMtx.ReadLock();

            for (auto recordsSetIt = RecordsSet.begin(); recordsSetIt != RecordsSet.end();)
            {
                // if found record in a then delete it here
                if (a.RecordsSet.find(*recordsSetIt) != a.RecordsSet.end())
                    recordsSetIt = Vault::RemoveRecord(*recordsSetIt, nullptr);
                else
                    ++recordsSetIt;
            }

            ParentVault->RecursiveReadWriteMtx.ReadUnlock();
        }
    }

    void VaultRecordSet::Intersect(const VaultRecordSet& a) noexcept
    {
        if (IsParentVaultValid && a.IsParentVaultValid)
        {
            ParentVault->RecursiveReadWriteMtx.ReadLock();

            for (auto recordsSetIt = RecordsSet.begin(); recordsSetIt != RecordsSet.end();)
            {
                // if not found record in a then delete it here
                if (a.RecordsSet.find(*recordsSetIt) == a.RecordsSet.end())
                    recordsSetIt = Vault::RemoveRecord(*recordsSetIt, nullptr);
                else
                    ++recordsSetIt;
            }

            ParentVault->RecursiveReadWriteMtx.ReadUnlock();
        }
    }

    bool VaultRecordSet::SaveToFile(const std::string& fileName, const std::string& separator, const bool& isSaveKey) const noexcept
    {
        bool res = false;

        if (IsParentVaultValid)
        {
            ParentVault->RecursiveReadWriteMtx.ReadLock();
            res = Vault::SaveToFile(fileName);
            ParentVault->RecursiveReadWriteMtx.ReadUnlock();
        }

        return res;
    }

    VaultRecordSet::~VaultRecordSet() noexcept
    {
        Reset();
    }


    bool operator==(const VaultRecordSet& a, const VaultRecordSet& b)
    {
        bool res = false;

        /// \todo Потокобезопасность
        if (a.ParentVault == b.ParentVault && a.RecordsSet == b.RecordsSet)
            res = true;

        return res;
    }
}