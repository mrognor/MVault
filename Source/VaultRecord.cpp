#include "VaultRecord.h"

namespace mvlt
{
    VaultRecord::VaultRecord() noexcept {}
    
    VaultRecord::VaultRecord(const VaultRecord& other) noexcept
    {
        IsValid = true;
        RefCounter = 0;

        other.Mtx.lock();
        for (const auto& it : other.Container)
            Container.emplace(it);
        other.Mtx.unlock();
    }

    void VaultRecord::AddRef() noexcept
    {
        Mtx.lock();
        if (IsValid) ++RefCounter;
        Mtx.unlock();
    }

    void VaultRecord::RemoveRef() noexcept
    {
        bool isEnd = false;
        Mtx.lock();
        --RefCounter;
        if (!IsValid && RefCounter == 0) isEnd = true;
        Mtx.unlock();

        if (isEnd) delete this;
    }

    void VaultRecord::Invalidate() noexcept
    {
        bool isEnd = false;
        Mtx.lock();
        IsValid = false;
        if (RefCounter == 0) isEnd = true;
        Mtx.unlock();

        if (isEnd) delete this;
    }

    bool VaultRecord::GetIsValid() noexcept
    {
        bool res;
        Mtx.lock();
        res = IsValid;
        Mtx.unlock();
        return res;
    }
}