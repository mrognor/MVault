#include "VaultRecord.h"
#include "VaultRecord.hpp"

namespace mvlt
{
    VaultRecord::VaultRecord() noexcept 
    {
        DBG_LOG_ENTER();
    }

    // codechecker_intentional [all] its not call parent class copy constructor because it is mutexes inside this class
    VaultRecord::VaultRecord(const VaultRecord& other) noexcept
    {
        DBG_LOG_ENTER();

        other.VaultRecordMutex.lock();
        for (const auto& containerIt : other.Container)
            Container.emplace(containerIt);
        other.VaultRecordMutex.unlock();
    }

    VaultRecord& VaultRecord::operator=(const VaultRecord& other) noexcept
    {
        DBG_LOG_ENTER();

        if (&other != this)
        {
            IsValid = true;
            RefCounter = 0;

            other.VaultRecordMutex.lock();
            Container.clear();
            for (const auto& containerIt : other.Container)
                Container.emplace(containerIt);
            other.VaultRecordMutex.unlock();
        }

        return *this;
    }

    VaultRecord::VaultRecord(VaultRecord&& other) noexcept
    {
        DBG_LOG_ENTER();

        other.VaultRecordMutex.lock();
        Container = std::move(other.Container);
        other.VaultRecordMutex.unlock();
    }

    VaultRecord& VaultRecord::operator=(VaultRecord&& other) noexcept
    {
        DBG_LOG_ENTER();

        if (&other != this)
        {
            IsValid = true;
            RefCounter = 0;

            other.VaultRecordMutex.lock();
            Container = std::move(other.Container);
            other.VaultRecordMutex.unlock();
        }

        return *this;
    }

    void VaultRecord::AddRef() noexcept
    {
        DBG_LOG_ENTER();

        VaultRecordMutex.lock();
        ++RefCounter;
        VaultRecordMutex.unlock();
    }

    void VaultRecord::RemoveRef() noexcept
    {
        DBG_LOG_ENTER();

        bool isEnd = false;
        VaultRecordMutex.lock();
        --RefCounter;
        if (!IsValid && RefCounter == 0) isEnd = true;
        VaultRecordMutex.unlock();

        if (isEnd) delete this;
    }

    void VaultRecord::Invalidate() noexcept
    {
        DBG_LOG_ENTER();

        bool isEnd = false;
        VaultRecordMutex.lock();
        IsValid = false;
        if (RefCounter == 0) isEnd = true;
        VaultRecordMutex.unlock();

        if (isEnd) delete this;
    }

    bool VaultRecord::GetIsValid() const noexcept
    {
        DBG_LOG_ENTER();

        bool res;
        VaultRecordMutex.lock();
        res = IsValid;
        VaultRecordMutex.unlock();
        return res;
    }

    void VaultRecord::RemoveFromDependentSets() noexcept
    {
        DBG_LOG_ENTER();

        VaultRecordMutex.lock();
        for (VaultRecordSet* set : dependentVaultRecordSets)
            set->RemoveRecord(this, nullptr);

        VaultRecordMutex.unlock();
    }

    void VaultRecord::AddToDependentSets(VaultRecordSet* vaultRecordSet) noexcept
    {
        DBG_LOG_ENTER();

        VaultRecordMutex.lock();
        dependentVaultRecordSets.emplace(vaultRecordSet);
        VaultRecordMutex.unlock();
    }

    void VaultRecord::EraseDependentSet(VaultRecordSet* vaultRecordSet) noexcept
    {
        DBG_LOG_ENTER();

        VaultRecordMutex.lock();
        dependentVaultRecordSets.erase(vaultRecordSet);
        VaultRecordMutex.unlock();
    }
}