#pragma once

#include <thread>
#include <mutex>
#include <iostream>
#include <atomic>
#include <condition_variable>

/**
    \brief A class for synchronizing threads

    A class for thread management that allows you to lock sections of code for reading or writing.
    A write lock will ensure that there can only be one thread in the code section at a time.
    The read lock will ensure that no thread using the write lock gets into the code section until all threads using the read lock are unblocked.
    At the same time, after the write lock, no new threads with a read lock will enter the code section until all threads using the write lock are unblocked.
*/
class ReadWriteMutex
{
private:
    std::mutex WriteMutex, ReadMutex;
    std::atomic_int ReadCounter;
    std::atomic_bool IsCondVarWaiting;
    std::condition_variable Cv;
public:
    /// \brief Default constructor
    ReadWriteMutex();

    /**
        \brief A method for locking a section of code for reading

        Using this method, you can lock the code section for reading, which means that all threads using the read lock will have access to data inside the code section
        but threads using the write lock will wait until all read operations are completed.
    */
    void ReadLock();

    /// \brief A method for unlocking a section of code for reading
    void ReadUnlock();

    /**
        \brief A method for locking a section of code for writing

        This method provides exclusive access to a section of code for a single thread.
        All write operations will be performed sequentially.
        This method takes precedence over the read lock, which means that after calling this method, no new read operations will be started.
    */
    void WriteLock();

    /// \brief A method for unlocking a section of code for writing
    void WriteUnlock();
};

/**
    \brief A class for synchronizing threads

    A class for thread management that allows you to lock sections of code for reading or writing.
    A write lock will ensure that there can only be one thread in the code section at a time.
    The read lock will ensure that no thread using the write lock gets into the code section until all threads using the read lock are unblocked.
    At the same time, after the write lock, no new threads with a read lock will enter the code section until all threads using the write lock are unblocked.
    Recursiveness allows you to call blocking methods in the same thread multiple times without self-locking.
*/
class RecursiveReadWriteMutex
{
private:
    ReadWriteMutex Rwmx;
public:

    /**
        \brief A method for locking a section of code for reading

        Using this method, you can lock the code section for reading, which means that all threads using the read lock will have access to data inside the code section
        but threads using the write lock will wait until all read operations are completed.
        Note that, in fact, blocking for reading inside writing does not make sense, 
        since the code section is already locked and therefore nothing will happen inside the function in such a situation.
    */
    void ReadLock();

    /// \brief A method for unlocking a section of code for reading
    void ReadUnlock();

    /**
        \brief A method for locking a section of code for writing

        This method provides exclusive access to a section of code for a single thread.
        All write operations will be performed sequentially.
        This method takes precedence over the read lock, which means that after calling this method, no new read operations will be started.
        Note that if the write lock is called inside the read lock, then this will be equivalent to unlocking for reading and then locking for writing.
    */
    void WriteLock();

    /// \brief A method for unlocking a section of code for writing
    /// Note that if the write unlock is called inside the read lock, then this will be equivalent to unlocking for writing and then locking for reading.
    void WriteUnlock();
};