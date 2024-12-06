#include "VaultRecordSet.h"
#include "VaultRecordSet.hpp"

namespace mvlt
{
    std::unordered_set<VaultRecord *>::iterator VaultRecordSet::RemoveRecord(VaultRecord* recordToErase, bool* wasDeleted) noexcept
    {
        return Vault::RemoveRecord(recordToErase, wasDeleted);
    }

    void VaultRecordSet::CopySet(const VaultRecordSet& other) noexcept
    {
        WriteLock<RecursiveReadWriteMutex> writeLock(other.ParentVault->RecursiveReadWriteMtx);

        VaultDerivedClass = VaultDerivedClasses::VaultRecordSetDerived;
        ParentVault = other.ParentVault;
        KeysOrder = other.KeysOrder;

        for (auto& keyCopierIt : other.VaultKeyCopiers)
            keyCopierIt.second(*this);

        other.ParentVault->RecordSetsSet.emplace(this);

        for (VaultRecord* record : other.RecordsSet)
        {
            RecordsSet.emplace(record);

            for (auto& adder : VaultRecordAdders)
                adder.second(record);

            record->AddToDependentSets(this);
        }

        RecursiveReadWriteMtx.Disable();
    }

    VaultRecordSet::VaultRecordSet() noexcept 
    {
        VaultDerivedClass = VaultDerivedClasses::VaultRecordSetDerived;
        RecursiveReadWriteMtx.Disable();
    }

    VaultRecordSet::VaultRecordSet(const VaultRecordSet& other) noexcept
    {
        if (&other != this && other.GetIsParentVaultValid())
            CopySet(other);
    }

    VaultRecordSet& VaultRecordSet::operator=(const VaultRecordSet& other) noexcept
    {
        if (&other != this)
        {
            Reset();
            if (other.GetIsParentVaultValid())
                CopySet(other);
        }

        return *this;
    }

    bool VaultRecordSet::GetIsParentVaultValid() const noexcept
    {
        return ParentVault != nullptr;
    }

    std::string VaultRecordSet::GetParentVaultUniqueId() const noexcept
    {
        std::stringstream ss;

        if (GetIsParentVaultValid())
            ss << ParentVault;
        else
            ss << "null";
        
        return ss.str();
    }

    bool VaultRecordSet::IsKeyExist(const std::string& key) const noexcept
    {
        bool res = false;

        if (GetIsParentVaultValid())
            res = ParentVault->IsKeyExist(key);

        return res;
    }

    bool VaultRecordSet::GetKeyType(const std::string& key, std::type_index& keyType) const noexcept
    {
        bool res = false;

        if (GetIsParentVaultValid())
        {
            ReadLock<RecursiveReadWriteMutex> readLock(ParentVault->RecursiveReadWriteMtx);
            res = Vault::GetKeyType(key, keyType);
        }

        return res;
    }

    VaultOperationResult VaultRecordSet::AddRecord(const VaultRecordRef& recordRef) noexcept
    {
        VaultOperationResult res;

        if (GetIsParentVaultValid())
        {
            ReadLock<RecursiveReadWriteMutex> readLock(ParentVault->RecursiveReadWriteMtx);

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
                    recordRef.VaultRecordPtr->AddToDependentSets(this);

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
        }
        else 
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::ParentVaultNotValid;
        }

        return res;
    }

    bool VaultRecordSet::CheckRecord(const VaultRecordRef& ref) const noexcept
    {
        ReadLock<RecursiveReadWriteMutex> readLock(ParentVault->RecursiveReadWriteMtx);
        if (Vault::RecordsSet.find(ref.VaultRecordPtr) != Vault::RecordsSet.end()) return true;
        else return false;
    }

    void VaultRecordSet::Reset() noexcept
    {
        if (GetIsParentVaultValid())
        {
            WriteLock<RecursiveReadWriteMutex> writeLock(ParentVault->RecursiveReadWriteMtx);
            Clear();
            Vault::DropVault();

            ParentVault->RecordSetsSet.erase(this);
            ParentVault = nullptr;
        }
    }

    void VaultRecordSet::Clear() noexcept
    {
        // Remove this from records
        for (VaultRecord* record : RecordsSet)
            record->EraseDependentSet(this);

        // Clear structure
        for (const auto& vaultRecordClearersIt : VaultRecordClearers)
            vaultRecordClearersIt.second();

        // No need to delete each record because it is RecordSet and records will be delete in original vault
        RecordsSet.clear();
    }

    bool VaultRecordSet::RemoveRecord(const VaultRecordRef& recordRefToErase) noexcept
    {
        if (!GetIsParentVaultValid()) return false;
        
        ReadLock<RecursiveReadWriteMutex> readLock(ParentVault->RecursiveReadWriteMtx);
        return Vault::EraseRecord(recordRefToErase);
    }

    std::size_t VaultRecordSet::Size() const noexcept
    {
        std::size_t res = 0;

        if (GetIsParentVaultValid())
        {
            ReadLock<RecursiveReadWriteMutex> readLock(ParentVault->RecursiveReadWriteMtx);
            res = Vault::Size();
        }

        return res;
    }

    std::vector<std::string> VaultRecordSet::GetKeys() const noexcept
    {
        std::vector<std::string> res;

        if (GetIsParentVaultValid())
            res = ParentVault->GetKeys();

        return res;
    }

    std::vector<VaultRecordRef> VaultRecordSet::GetSortedRecords(const std::string& key, const bool& isReverse, const std::size_t& amountOfRecords) const noexcept
    {
        std::vector<VaultRecordRef> res;

        if (GetIsParentVaultValid())
        {
            ReadLock<RecursiveReadWriteMutex> readLock(ParentVault->RecursiveReadWriteMtx);
            res = Vault::GetSortedRecords(key, isReverse, amountOfRecords);
        }

        return res;
    }

    void VaultRecordSet::PrintSet(const std::size_t& amountOfRecords) const noexcept
    {
        if (GetIsParentVaultValid())
        {
            ReadLock<RecursiveReadWriteMutex> readLock(ParentVault->RecursiveReadWriteMtx);
            Vault::PrintVault(amountOfRecords);
        }
        else
            std::cout << "The parent Vault is not valid!" << std::endl;
    }

    void VaultRecordSet::PrintAsTable(bool isPrintId, const std::size_t& amountOfRecords, std::string primaryKey, const bool& isReverse,
            const std::list<std::string> keys) const noexcept
        {
        if (GetIsParentVaultValid())
        {
            ReadLock<RecursiveReadWriteMutex> readLock(ParentVault->RecursiveReadWriteMtx);
            Vault::PrintAsTable(isPrintId, amountOfRecords, primaryKey, isReverse, keys);
        }
        else
            std::cout << "The parent Vault is not valid!" << std::endl;
    }

    void VaultRecordSet::Join(const VaultRecordSet& a) noexcept
    {
        if (GetIsParentVaultValid() && a.GetIsParentVaultValid())
        {
            ReadLock<RecursiveReadWriteMutex> readLock(ParentVault->RecursiveReadWriteMtx);

            for (VaultRecord* record : a.RecordsSet)
            {
                if (RecordsSet.find(record) == RecordsSet.end())
                {
                    RecordsSet.emplace(record);

                    for (auto& adder : VaultRecordAdders)
                        adder.second(record);
                }
            }
        }
    }

    void VaultRecordSet::Exclude(const VaultRecordSet& a) noexcept
    {
        if (GetIsParentVaultValid() && a.GetIsParentVaultValid())
        {
            ReadLock<RecursiveReadWriteMutex> readLock(ParentVault->RecursiveReadWriteMtx);

            for (auto recordsSetIt = RecordsSet.begin(); recordsSetIt != RecordsSet.end();)
            {
                // if found record in a then delete it here
                if (a.RecordsSet.find(*recordsSetIt) != a.RecordsSet.end())
                    recordsSetIt = RemoveRecord(*recordsSetIt, nullptr);
                else
                    ++recordsSetIt;
            }
        }
    }

    void VaultRecordSet::Intersect(const VaultRecordSet& a) noexcept
    {
        if (GetIsParentVaultValid() && a.GetIsParentVaultValid())
        {
            ReadLock<RecursiveReadWriteMutex> readLock(ParentVault->RecursiveReadWriteMtx);

            for (auto recordsSetIt = RecordsSet.begin(); recordsSetIt != RecordsSet.end();)
            {
                // if not found record in a then delete it here
                if (a.RecordsSet.find(*recordsSetIt) == a.RecordsSet.end())
                    recordsSetIt = RemoveRecord(*recordsSetIt, nullptr);
                else
                    ++recordsSetIt;
            }
        }
    }

    bool VaultRecordSet::SaveToFile(const std::string& fileName, const std::vector<std::string> keys, const std::string& separator, const bool& isSaveKey) const noexcept
    {
        bool res = false;

        if (GetIsParentVaultValid())
        {
            ReadLock<RecursiveReadWriteMutex> readLock(ParentVault->RecursiveReadWriteMtx);
            res = Vault::SaveToFile(fileName, keys, separator, isSaveKey);
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

        if (a.ParentVault == b.ParentVault && a.RecordsSet == b.RecordsSet)
            res = true;

        return res;
    }
}