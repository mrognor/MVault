#include "VaultRecord.h"

namespace mvlt
{
    VaultRecord::VaultRecord() {}
    
    VaultRecord::VaultRecord(const VaultRecord& other)
    {
        IsValid = true;
        RefCounter = 0;

        other.Mtx.lock();
        for (const auto& it : other.Container)
            Container.emplace(it);
        other.Mtx.unlock();
    }

    void VaultRecord::AddRef()
    {
        Mtx.lock();
        if (IsValid) ++RefCounter;
        Mtx.unlock();
    }

    void VaultRecord::RemoveRef()
    {
        bool isEnd = false;
        Mtx.lock();
        --RefCounter;
        if (!IsValid && RefCounter == 0) isEnd = true;
        Mtx.unlock();

        if (isEnd) delete this;
    }

    void VaultRecord::Invalidate()
    {
        bool isEnd = false;
        Mtx.lock();
        IsValid = false;
        if (RefCounter == 0) isEnd = true;
        Mtx.unlock();

        if (isEnd) delete this;
    }

    bool VaultRecord::GetIsValid()
    {
        bool res;
        Mtx.lock();
        res = IsValid;
        Mtx.unlock();
        return res;
    }
}