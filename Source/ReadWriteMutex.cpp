#include "ReadWriteMutex.h"

ReadWriteMutex::ReadWriteMutex() 
{ 
    ReadCounter.store(0);
    IsCondVarWaiting.store(false);
}

void ReadWriteMutex::ReadLock()
{
    WriteMutex.lock();
    ReadMutex.lock();
    ReadCounter.fetch_add(1);
    ReadMutex.unlock();
    WriteMutex.unlock();
}

void ReadWriteMutex::ReadUnlock()
{
    ReadMutex.lock();
    ReadCounter.fetch_sub(1);

    if (ReadCounter.load() == 0)
        while (IsCondVarWaiting.load()) Cv.notify_all();
    
    ReadMutex.unlock();
}

void ReadWriteMutex::WriteLock()
{
    std::mutex mtx;
    std::unique_lock<std::mutex> lk(mtx);

    WriteMutex.lock();
    
    IsCondVarWaiting.store(true);

    if (ReadCounter > 0) Cv.wait(lk);
    
    IsCondVarWaiting.store(false);
}

void ReadWriteMutex::WriteUnlock()
{
    WriteMutex.unlock();
}

thread_local std::size_t LocalThreadReadLockCounter = 0;
thread_local std::size_t LocalThreadWriteLockCounter = 0;

void RecursiveReadWriteMutex::ReadLock()
{
    if (LocalThreadWriteLockCounter == 0)
    {
        if (LocalThreadReadLockCounter == 0)
            Rwmx.ReadLock();

        ++LocalThreadReadLockCounter;
    }
}

void RecursiveReadWriteMutex::ReadUnlock()
{
    if (LocalThreadWriteLockCounter == 0)
    {
        if (LocalThreadReadLockCounter == 1)
            Rwmx.ReadUnlock();
        
        --LocalThreadReadLockCounter;
    }
}

void RecursiveReadWriteMutex::WriteLock()
{
    if (LocalThreadWriteLockCounter == 0)
    {
        if (LocalThreadReadLockCounter > 0)
            Rwmx.ReadUnlock();
        
        Rwmx.WriteLock();
    }

    ++LocalThreadWriteLockCounter;
}


void RecursiveReadWriteMutex::WriteUnlock()
{
    if (LocalThreadWriteLockCounter == 1)
    {
        Rwmx.WriteUnlock();
        if (LocalThreadReadLockCounter > 0)
            Rwmx.ReadLock();
    }
    --LocalThreadWriteLockCounter;
}