#include "VaultRecord.h"
#include "VaultRecord.hpp"

namespace mvlt
{
    VaultRecord::VaultRecord() noexcept {}
    
    VaultRecord::VaultRecord(const VaultRecord& other) noexcept
    {
        IsValid = true;
        RefCounter = 0;

        other.VaultRecordMutex.lock();
        for (const auto& containerIt : other.Container)
            Container.emplace(containerIt);
        other.VaultRecordMutex.unlock();
    }

    void VaultRecord::AddRef() noexcept
    {
        VaultRecordMutex.lock();
        ++RefCounter;
        VaultRecordMutex.unlock();
    }

    void VaultRecord::RemoveRef() noexcept
    {
        bool isEnd = false;
        VaultRecordMutex.lock();
        --RefCounter;
        if (!IsValid && RefCounter == 0) isEnd = true;
        VaultRecordMutex.unlock();

        if (isEnd) delete this;
    }

    void VaultRecord::Invalidate() noexcept
    {
        bool isEnd = false;
        VaultRecordMutex.lock();
        IsValid = false;
        if (RefCounter == 0) isEnd = true;
        VaultRecordMutex.unlock();

        if (isEnd) delete this;
    }

    bool VaultRecord::GetIsValid() const noexcept
    {
        bool res;
        VaultRecordMutex.lock();
        res = IsValid;
        VaultRecordMutex.unlock();
        return res;
    }

    void VaultRecord::RemoveFromDependentSets() noexcept
    {
        VaultRecordMutex.lock();
        for (VaultRecordSet* set : dependentVaultRecordSets)
            set->RemoveRecord(this, nullptr);

        VaultRecordMutex.unlock();
    }

    void VaultRecord::AddToDependentSets(VaultRecordSet* vaultRecordSet) noexcept
    {
        VaultRecordMutex.lock();
        dependentVaultRecordSets.emplace(vaultRecordSet);
        VaultRecordMutex.unlock();
    }

    void VaultRecord::EraseDependentSet(VaultRecordSet* vaultRecordSet) noexcept
    {
        VaultRecordMutex.lock();
        dependentVaultRecordSets.erase(vaultRecordSet);
        VaultRecordMutex.unlock();
    }
}