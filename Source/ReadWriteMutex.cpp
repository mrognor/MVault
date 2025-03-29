#include "ReadWriteMutex.h"

namespace mvlt
{
    ReadWriteMutex::ReadWriteMutex() noexcept
    {
        ReadCounter.store(0);
        IsCondVarWaiting.store(false);
    }

    void ReadWriteMutex::Disable() noexcept
    {
        IsActive = false;
    }

    void ReadWriteMutex::ReadLock() noexcept
    {
        if (IsActive)
        {
            WriteMutex.lock();
            ReadMutex.lock();
            ReadCounter.fetch_add(1);
            ReadMutex.unlock();
            WriteMutex.unlock();
        }
    }

    void ReadWriteMutex::ReadUnlock() noexcept
    {
        if (IsActive)
        {
            ReadMutex.lock();
            ReadCounter.fetch_sub(1);

            if (ReadCounter.load() == 0)
                while (IsCondVarWaiting.load())
                    Cv.notify_all();

            ReadMutex.unlock();
        }
    }

    void ReadWriteMutex::WriteLock() noexcept
    {
        if (IsActive)
        {
            std::mutex mtx;
            std::unique_lock<std::mutex> lk(mtx);

            WriteMutex.lock();

            IsCondVarWaiting.store(true);

            if (ReadCounter > 0) Cv.wait(lk);

            IsCondVarWaiting.store(false);
        }
    }

    void ReadWriteMutex::WriteUnlock() noexcept
    {
        if (IsActive)
        {
            WriteMutex.unlock();
        }
    }

    thread_local std::size_t LocalThreadReadLockCounter = 0;
    thread_local std::size_t LocalThreadWriteLockCounter = 0;

    void RecursiveReadWriteMutex::Disable() noexcept
    {
        IsActive = false;
    }

    void RecursiveReadWriteMutex::ReadLock() noexcept
    {
        if (IsActive)
        {
            if (LocalThreadWriteLockCounter == 0)
            {
                if (LocalThreadReadLockCounter == 0)
                    Rwmx.ReadLock();

                ++LocalThreadReadLockCounter;
            }
        }
    }

    void RecursiveReadWriteMutex::ReadUnlock() noexcept
    {
        if (IsActive)
        {
            if (LocalThreadWriteLockCounter == 0)
            {
                if (LocalThreadReadLockCounter == 1)
                    Rwmx.ReadUnlock();

                --LocalThreadReadLockCounter;
            }
        }
    }

    void RecursiveReadWriteMutex::WriteLock() noexcept
    {
        if (IsActive)
        {
            if (LocalThreadWriteLockCounter == 0)
            {
                if (LocalThreadReadLockCounter > 0)
                    Rwmx.ReadUnlock();

                Rwmx.WriteLock();
            }

            ++LocalThreadWriteLockCounter;
        }
    }

    void RecursiveReadWriteMutex::WriteUnlock() noexcept
    {
        if (IsActive)
        {
            if (LocalThreadWriteLockCounter == 1)
            {
                Rwmx.WriteUnlock();
                if (LocalThreadReadLockCounter > 0)
                    Rwmx.ReadLock();
            }
            --LocalThreadWriteLockCounter;
        }
    }
}