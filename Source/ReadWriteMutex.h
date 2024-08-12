#pragma once

#include <mutex>
#include <atomic>
#include <thread>
#include <iostream>
#include <condition_variable>

namespace mvlt
{
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

        // A variable for switching the operation of the mutex. It is needed so that you can disable the mutex during inheritance
        bool IsActive = true;
    public:
        /// \brief Default constructor
        ReadWriteMutex() noexcept;

        /// \brief A function to disable the mutex. It is needed so that you can disable the mutex during inheritance
        void Disable() noexcept;

        /**
            \brief A method for locking a section of code for reading

            Using this method, you can lock the code section for reading, which means that all threads using the read lock will have access to data inside the code section
            but threads using the write lock will wait until all read operations are completed.
        */
        void ReadLock() noexcept;

        /// \brief A method for unlocking a section of code for reading
        void ReadUnlock() noexcept;

        /**
            \brief A method for locking a section of code for writing

            This method provides exclusive access to a section of code for a single thread.
            All write operations will be performed sequentially.
            This method takes precedence over the read lock, which means that after calling this method, no new read operations will be started.
        */
        void WriteLock() noexcept;

        /// \brief A method for unlocking a section of code for writing
        void WriteUnlock() noexcept;
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

        // A variable for switching the operation of the mutex. It is needed so that you can disable the mutex during inheritance
        bool IsActive = true;
    public:

        /// \brief A function to disable the mutex. It is needed so that you can disable the mutex during inheritance
        void Disable() noexcept;

        /**
            \brief A method for locking a section of code for reading

            Using this method, you can lock the code section for reading, which means that all threads using the read lock will have access to data inside the code section
            but threads using the write lock will wait until all read operations are completed.
            Note that, in fact, blocking for reading inside writing does not make sense, 
            since the code section is already locked and therefore nothing will happen inside the function in such a situation.
        */
        void ReadLock() noexcept;

        /// \brief A method for unlocking a section of code for reading
        void ReadUnlock() noexcept;

        /**
            \brief A method for locking a section of code for writing

            This method provides exclusive access to a section of code for a single thread.
            All write operations will be performed sequentially.
            This method takes precedence over the read lock, which means that after calling this method, no new read operations will be started.
            Note that if the write lock is called inside the read lock, then this will be equivalent to unlocking for reading and then locking for writing.
        */
        void WriteLock() noexcept;

        /**
            \brief A method for unlocking a section of code for writing

            Note that if the write unlock is called inside the read lock, then this will be equivalent to unlocking for writing and then locking for reading.
        */
        void WriteUnlock() noexcept;
    };

    /**
        \brief A class for blocking a stream for reading

        In the constructor, this class captures Mutex and blocks it for reading, and unlocks it in the destructor.

        \tparam <Mutex> a mutex that supports read and write lock operations
        \tparam ReadWriteMutex a standard mutex that supports read and write lock operations
        \tparam RecursiveReadWriteMutex a recursive mutex that supports read and write lock operations
    */
    template <class Mutex>
    class ReadLock
    {
    private:
        Mutex& Mtx;
    public:
        /**
            \brief Constructor

            Lock thread to read

            \param [in] mtx mutex to capture and lock
        */
        ReadLock(Mutex& mtx) : Mtx(mtx) 
        {
            Mtx.ReadLock();
        }

        /**
            \brief Destructor

            Unlock captured mutex
        */
        ~ReadLock()
        {
            Mtx.ReadUnlock();
        } 
    };

    /**
        \brief A class for blocking a stream for writing

        In the constructor, this class captures Mutex and blocks it for writing, and unlocks it in the destructor.

        \tparam <Mutex> a mutex that supports read and write lock operations
        \tparam ReadWriteMutex a standard mutex that supports read and write lock operations
        \tparam RecursiveReadWriteMutex a recursive mutex that supports read and write lock operations
    */
    template <class Mutex>
    class WriteLock
    {
    private:
        Mutex& Mtx;
    public:
        /**
            \brief Constructor

            Lock thread to write

            \param [in] mtx mutex to capture and lock
        */
        WriteLock(Mutex& mtx) : Mtx(mtx) 
        {
            Mtx.WriteLock();
        }

        /**
            \brief Destructor

            Unlock captured mutex
        */
        ~WriteLock()
        {
            Mtx.WriteUnlock();
        } 
    };
}