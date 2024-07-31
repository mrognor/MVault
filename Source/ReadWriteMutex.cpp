#include "ReadWriteMutex.h"

namespace mvlt
{
    ReadWriteMutex::ReadWriteMutex() noexcept
    { 
        ReadCounter.store(0);
        IsCondVarWaiting.store(false);
    }

    void ReadWriteMutex::ReadLock() noexcept
    {
        WriteMutex.lock();
        ReadMutex.lock();
        ReadCounter.fetch_add(1);
        ReadMutex.unlock();
        WriteMutex.unlock();
    }

    void ReadWriteMutex::ReadUnlock() noexcept
    {
        ReadMutex.lock();
        ReadCounter.fetch_sub(1);

        if (ReadCounter.load() == 0)
            while (IsCondVarWaiting.load()) Cv.notify_all();
        
        ReadMutex.unlock();
    }

    void ReadWriteMutex::WriteLock() noexcept
    {
        std::mutex mtx;
        std::unique_lock<std::mutex> lk(mtx);

        WriteMutex.lock();
        
        IsCondVarWaiting.store(true);

        if (ReadCounter > 0) Cv.wait(lk);
        
        IsCondVarWaiting.store(false);
    }

    void ReadWriteMutex::WriteUnlock() noexcept
    {
        WriteMutex.unlock();
    }

    thread_local std::size_t LocalThreadReadLockCounter = 0;
    thread_local std::size_t LocalThreadWriteLockCounter = 0;

    void RecursiveReadWriteMutex::ReadLock() noexcept
    {
        if (LocalThreadWriteLockCounter == 0)
        {
            if (LocalThreadReadLockCounter == 0)
                Rwmx.ReadLock();

            ++LocalThreadReadLockCounter;
        }
    }

    void RecursiveReadWriteMutex::ReadUnlock() noexcept
    {
        if (LocalThreadWriteLockCounter == 0)
        {
            if (LocalThreadReadLockCounter == 1)
                Rwmx.ReadUnlock();
            
            --LocalThreadReadLockCounter;
        }
    }

    void RecursiveReadWriteMutex::WriteLock() noexcept
    {
        if (LocalThreadWriteLockCounter == 0)
        {
            if (LocalThreadReadLockCounter > 0)
                Rwmx.ReadUnlock();
            
            Rwmx.WriteLock();
        }

        ++LocalThreadWriteLockCounter;
    }


    void RecursiveReadWriteMutex::WriteUnlock() noexcept
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