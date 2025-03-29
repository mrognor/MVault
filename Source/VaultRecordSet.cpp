#include "VaultRecordSet.h"
#include "VaultRecordSet.hpp"

namespace mvlt
{
    std::unordered_set<VaultRecord*>::iterator VaultRecordSet::RemoveRecord(VaultRecord* recordToErase, bool* wasDeleted) noexcept
    {
        DBG_LOG_ENTER();

        return Vault::RemoveRecord(recordToErase, wasDeleted);
    }

    void VaultRecordSet::CopySet(const VaultRecordSet& other) noexcept
    {
        DBG_LOG_ENTER();

        WriteLock<RecursiveReadWriteMutex> writeLock(other.ParentVault->RecursiveReadWriteMtx);

        VaultDerivedClass = VaultDerivedClasses::VaultRecordSetDerived;
        ParentVault = other.ParentVault;

        for (auto& keyCopierIt : other.VaultKeyCopiers)
            keyCopierIt.second(this);

        KeysOrder = other.KeysOrder;
        UniqueKeys = other.UniqueKeys;

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

    void VaultRecordSet::MoveSet(VaultRecordSet& other) noexcept
    {
        DBG_LOG_ENTER();

        WriteLock<RecursiveReadWriteMutex> writeLock(other.ParentVault->RecursiveReadWriteMtx);

        VaultDerivedClass = VaultDerivedClasses::VaultRecordSetDerived;

        ParentVault = other.ParentVault;
        other.ParentVault = nullptr;

        RecordTemplate = std::move(other.RecordTemplate);
        VaultHashMapStructure = std::move(other.VaultHashMapStructure);
        VaultMapStructure = std::move(other.VaultMapStructure);
        KeysTypes = std::move(other.KeysTypes);
        VaultRecordAdders = std::move(other.VaultRecordAdders);
        VaultRecordClearers = std::move(other.VaultRecordClearers);
        VaultRecordErasers = std::move(other.VaultRecordErasers);
        VaultRecordSorters = std::move(other.VaultRecordSorters);
        VaultKeyCopiers = std::move(other.VaultKeyCopiers);
        KeysOrder = std::move(other.KeysOrder);
        UniqueKeys = std::move(other.UniqueKeys);
        InvalidFileRecords = std::move(other.InvalidFileRecords);
        RecordsSet = std::move(other.RecordsSet);
        RecordSetsSet = std::move(other.RecordSetsSet);

        ParentVault->RecordSetsSet.erase(&other);
        ParentVault->RecordSetsSet.emplace(this);

        for (VaultRecord* record : other.RecordsSet)
        {
            record->EraseDependentSet(&other);
            record->AddToDependentSets(this);
        }

        RecursiveReadWriteMtx.Disable();
    }

    VaultRecordSet::VaultRecordSet() noexcept
    {
        DBG_LOG_ENTER();

        VaultDerivedClass = VaultDerivedClasses::VaultRecordSetDerived;
        RecursiveReadWriteMtx.Disable();
    }

    // codechecker_intentional [all] its not call parent class copy constructor because it is other copying style
    VaultRecordSet::VaultRecordSet(const VaultRecordSet& other) noexcept : Vault()
    {
        DBG_LOG_ENTER();

        if (other.GetIsParentVaultValid())
            CopySet(other);
    }

    VaultRecordSet& VaultRecordSet::operator=(const VaultRecordSet& other) noexcept
    {
        DBG_LOG_ENTER();

        if (&other != this)
        {
            Reset();
            if (other.GetIsParentVaultValid())
                CopySet(other);
        }

        return *this;
    }

    VaultRecordSet::VaultRecordSet(VaultRecordSet&& other) noexcept
    {
        DBG_LOG_ENTER();

        if (other.GetIsParentVaultValid())
            MoveSet(other);
    }

    VaultRecordSet& VaultRecordSet::operator=(VaultRecordSet&& other) noexcept
    {
        DBG_LOG_ENTER();

        if (&other != this)
        {
            if (other.GetIsParentVaultValid())
                MoveSet(other);
            else
                Reset();
        }

        return *this;
    }

    bool VaultRecordSet::GetIsParentVaultValid() const noexcept
    {
        DBG_LOG_ENTER();

        return ParentVault != nullptr;
    }

    std::string VaultRecordSet::GetParentVaultUniqueId() const noexcept
    {
        DBG_LOG_ENTER();

        std::stringstream ss;

        if (GetIsParentVaultValid())
            ss << ParentVault;
        else
            ss << "null";

        return ss.str();
    }

    bool VaultRecordSet::IsKeyExist(const std::string& key) const noexcept
    {
        DBG_LOG_ENTER();

        bool res = false;

        if (GetIsParentVaultValid())
            res = ParentVault->IsKeyExist(key);

        return res;
    }

    bool VaultRecordSet::GetKeyType(const std::string& key, std::type_index& keyType) const noexcept
    {
        DBG_LOG_ENTER();

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
        DBG_LOG_ENTER();

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
        DBG_LOG_ENTER();

        if (GetIsParentVaultValid())
        {
            ReadLock<RecursiveReadWriteMutex> readLock(ParentVault->RecursiveReadWriteMtx);
            if (Vault::RecordsSet.find(ref.VaultRecordPtr) != Vault::RecordsSet.end())
                return true;
        }
        return false;
    }

    void VaultRecordSet::Reset() noexcept
    {
        DBG_LOG_ENTER();

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
        DBG_LOG_ENTER();

        if (GetIsParentVaultValid())
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
    }

    bool VaultRecordSet::RemoveRecord(const VaultRecordRef& recordRefToErase) noexcept
    {
        DBG_LOG_ENTER();

        if (!GetIsParentVaultValid()) return false;

        ReadLock<RecursiveReadWriteMutex> readLock(ParentVault->RecursiveReadWriteMtx);
        return Vault::EraseRecord(recordRefToErase);
    }

    std::size_t VaultRecordSet::Size() const noexcept
    {
        DBG_LOG_ENTER();

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
        DBG_LOG_ENTER();

        std::vector<std::string> res;

        if (GetIsParentVaultValid())
            res = ParentVault->GetKeys();

        return res;
    }

    std::vector<std::string> VaultRecordSet::GetUniqueKeys() const noexcept
    {
        DBG_LOG_ENTER();

        std::vector<std::string> res;

        if (GetIsParentVaultValid())
            res = ParentVault->GetUniqueKeys();

        return res;
    }

    std::vector<VaultRecordRef> VaultRecordSet::GetSortedRecords(const std::string& key, const bool& isReverse, const std::size_t& amountOfRecords) const noexcept
    {
        DBG_LOG_ENTER();

        std::vector<VaultRecordRef> res;

        if (GetIsParentVaultValid())
        {
            ReadLock<RecursiveReadWriteMutex> readLock(ParentVault->RecursiveReadWriteMtx);
            res = Vault::GetSortedRecords(key, isReverse, amountOfRecords);
        }

        return res;
    }

    std::string VaultRecordSet::ToJson(const bool& isFormat, const std::size_t& tabSize, const bool& isUseRecordTemplate,
        const std::string& recordTemplate) const noexcept
    {
        DBG_LOG_ENTER();

        if (GetIsParentVaultValid())
        {
            ReadLock<RecursiveReadWriteMutex> readLock(ParentVault->RecursiveReadWriteMtx);
            return Vault::ToJson(isFormat, tabSize, isUseRecordTemplate, recordTemplate);
        }
        else
            return "{}";
    }

    std::vector<std::vector<std::pair<std::string, std::string>>> VaultRecordSet::ToStrings() const noexcept
    {
        DBG_LOG_ENTER();

        if (GetIsParentVaultValid())
        {
            ReadLock<RecursiveReadWriteMutex> readLock(ParentVault->RecursiveReadWriteMtx);
            return Vault::ToStrings();
        }
        else
            return std::vector<std::vector<std::pair<std::string, std::string>>>();
    }

    void VaultRecordSet::Print(const bool& isPrintId, const std::size_t& amountOfRecords, const std::string& primaryKey, const bool& isReverse,
        const std::list<std::string>& keys) const noexcept
    {
        DBG_LOG_ENTER();

        if (GetIsParentVaultValid())
        {
            ReadLock<RecursiveReadWriteMutex> readLock(ParentVault->RecursiveReadWriteMtx);
            Vault::Print(isPrintId, amountOfRecords, primaryKey, isReverse, keys);
        }
        else
            std::cout << "The parent Vault is not valid!" << "\n";
    }

    VaultOperationResult VaultRecordSet::Join(const VaultRecordSet& a) noexcept
    {
        DBG_LOG_ENTER();

        VaultOperationResult res;
        res.IsOperationSuccess = true;

        // Handle errors
        if (!GetIsParentVaultValid())
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::ParentVaultNotValid;
        }

        if (res.IsOperationSuccess && !a.GetIsParentVaultValid())
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::OtherParentVaultNotValid;
        }

        if (res.IsOperationSuccess && a.ParentVault != ParentVault)
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::ParentVaultNotMatch;
        }

        if (res.IsOperationSuccess && this == &a)
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::SameVaultRecordSet;
        }

        if (res.IsOperationSuccess)
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

        return res;
    }

    VaultOperationResult VaultRecordSet::Exclude(const VaultRecordSet& a) noexcept
    {
        DBG_LOG_ENTER();

        VaultOperationResult res;
        res.IsOperationSuccess = true;

        // Handle errors
        if (!GetIsParentVaultValid())
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::ParentVaultNotValid;
        }

        if (res.IsOperationSuccess && !a.GetIsParentVaultValid())
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::OtherParentVaultNotValid;
        }

        if (res.IsOperationSuccess && a.ParentVault != ParentVault)
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::ParentVaultNotMatch;
        }

        if (res.IsOperationSuccess && this == &a)
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::SameVaultRecordSet;
        }

        if (res.IsOperationSuccess)
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

        return res;
    }

    VaultOperationResult VaultRecordSet::Intersect(const VaultRecordSet& a) noexcept
    {
        DBG_LOG_ENTER();

        VaultOperationResult res;
        res.IsOperationSuccess = true;

        // Handle errors
        if (!GetIsParentVaultValid())
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::ParentVaultNotValid;
        }

        if (res.IsOperationSuccess && !a.GetIsParentVaultValid())
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::OtherParentVaultNotValid;
        }

        if (res.IsOperationSuccess && a.ParentVault != ParentVault)
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::ParentVaultNotMatch;
        }

        if (res.IsOperationSuccess && this == &a)
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::SameVaultRecordSet;
        }

        if (res.IsOperationSuccess)
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

        return res;
    }

    bool VaultRecordSet::SaveToFile(const std::string& fileName, const std::vector<std::string>& keys, const std::string& separator, const bool& isSaveKey) const noexcept
    {
        DBG_LOG_ENTER();

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
        DBG_LOG_ENTER();

        Reset();
    }

    bool operator==(const VaultRecordSet& a, const VaultRecordSet& b) noexcept
    {
        DBG_LOG_ENTER();

        bool res = false;

        if (a.ParentVault == b.ParentVault && a.RecordsSet == b.RecordsSet)
            res = true;

        return res;
    }

    VaultOperationResult Union(const VaultRecordSet& a, const VaultRecordSet& b, VaultRecordSet& res) noexcept
    {
        DBG_LOG_ENTER();

        VaultOperationResult vor;

        if (&a == &b || &a == &res || &b == &res)
        {
            vor.IsOperationSuccess = false;
            vor.ResultCode = VaultOperationResultCode::SameVaultRecordSet;
        }
        else
        {
            res = a;
            vor = res.Join(b);
        }

        return vor;
    }

    VaultOperationResult Intersection(const VaultRecordSet& a, const VaultRecordSet& b, VaultRecordSet& res) noexcept
    {
        DBG_LOG_ENTER();

        VaultOperationResult vor;
        vor.IsOperationSuccess = true;

        // Handle errors
        if (&a == &b || &a == &res || &b == &res)
        {
            vor.IsOperationSuccess = false;
            vor.ResultCode = VaultOperationResultCode::SameVaultRecordSet;
        }

        if (vor.IsOperationSuccess && !a.GetIsParentVaultValid())
        {
            vor.IsOperationSuccess = false;
            vor.ResultCode = VaultOperationResultCode::ParentVaultNotValid;
        }

        if (vor.IsOperationSuccess && !b.GetIsParentVaultValid())
        {
            vor.IsOperationSuccess = false;
            vor.ResultCode = VaultOperationResultCode::OtherParentVaultNotValid;
        }

        if (vor.IsOperationSuccess && a.ParentVault != b.ParentVault)
        {
            vor.IsOperationSuccess = false;
            vor.ResultCode = VaultOperationResultCode::ParentVaultNotMatch;
        }

        if (vor.IsOperationSuccess)
        {
            if (res.GetIsParentVaultValid()) res.Reset();

            a.ParentVault->RecursiveReadWriteMtx.ReadLock();

            res.ParentVault = a.ParentVault;

            for (auto& keyCopierIt : a.VaultKeyCopiers)
                keyCopierIt.second(&res);

            // Set proper key order
            res.KeysOrder = a.KeysOrder;

            // Set unique keys
            res.UniqueKeys = a.UniqueKeys;

            a.ParentVault->RecordSetsSet.emplace(&res);

            // Pick less set to iterate
            if (a.RecordsSet.size() < b.RecordsSet.size())
            {
                // Iterate over all records in a
                for (VaultRecord* record : a.RecordsSet)
                {
                    // if record in b then add it to the res
                    if (b.RecordsSet.find(record) != b.RecordsSet.end())
                    {
                        res.RecordsSet.emplace(record);

                        for (auto& adder : res.VaultRecordAdders)
                            adder.second(record);
                    }
                }
            }
            else
            {
                // Iterate over all records in b
                for (VaultRecord* record : b.RecordsSet)
                {
                    // if record in b then add it to the res
                    if (a.RecordsSet.find(record) != a.RecordsSet.end())
                    {
                        res.RecordsSet.emplace(record);

                        for (auto& adder : res.VaultRecordAdders)
                            adder.second(record);
                    }
                }
            }

            a.ParentVault->RecursiveReadWriteMtx.ReadUnlock();
        }

        return vor;
    }
}