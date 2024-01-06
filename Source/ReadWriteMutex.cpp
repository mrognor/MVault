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


RecursiveReadWriteMutex::RecursiveReadWriteMutex() 
{ 
    ReadCounter.store(0);
    IsCondVarWaiting.store(false);
}

void RecursiveReadWriteMutex::ReadLock()
{
    WriteMutex.lock();
    ReadMutex.lock();
    ReadCounter.fetch_add(1);
    ReadMutex.unlock();
    WriteMutex.unlock();
}

void RecursiveReadWriteMutex::ReadUnlock()
{
    ReadMutex.lock();
    ReadCounter.fetch_sub(1);

    if (ReadCounter.load() == 0)
        while (IsCondVarWaiting.load()) Cv.notify_all();
    
    ReadMutex.unlock();
}

void RecursiveReadWriteMutex::WriteLock()
{
    std::mutex mtx;
    std::unique_lock<std::mutex> lk(mtx);

    WriteMutex.lock();
    
    IsCondVarWaiting.store(true);

    if (ReadCounter > 0) Cv.wait(lk);
    
    IsCondVarWaiting.store(false);
}

void RecursiveReadWriteMutex::WriteUnlock()
{
    WriteMutex.unlock();
}