#include "Tests.h"

void MultithreadingReadTest()
{
    DataStorage ds;
    std::condition_variable cv;

    std::cout << "\t\tReading test started: " << std::flush;

    ds.SetKey<int>("key", -1);

    for (int i = 0; i < 100000; ++i)
        ds.CreateRecord({ {"key", i} });

    auto timeout = std::chrono::steady_clock::now() + std::chrono::milliseconds(500);


    std::thread th1([&]()
    {
        std::mutex mtx;
        std::unique_lock<std::mutex> lk(mtx);
    
        // Wait for start all threads at same time
        cv.wait_until(lk, timeout);

        for (int i = 0; i < 100000; ++i)
            DataStorageRecordRef dsrr = ds.GetRecord("key", i);
    });

    // Thread for forward records reading
    std::thread th2([&]()
    {
        std::mutex mtx;
        std::unique_lock<std::mutex> lk(mtx);
    
        // Wait for start all threads at same time
        cv.wait_until(lk, timeout);

        for (int i = 0; i < 100000; ++i)
            DataStorageRecordRef dsrr = ds.GetRecord("key", i);
    });

    // Thread for backward records reading
    std::thread th3([&]()
    {
        std::mutex mtx;
        std::unique_lock<std::mutex> lk(mtx);
    
        // Wait for start all threads at same time
        cv.wait_until(lk, timeout);

        for (int i = 99999; i >= 0; --i)
            DataStorageRecordRef dsrr = ds.GetRecord("key", i);
    });

        // Thread for backward records reading
    std::thread th4([&]()
    {
        std::mutex mtx;
        std::unique_lock<std::mutex> lk(mtx);
    
        // Wait for start all threads at same time
        cv.wait_until(lk, timeout);

        for (int i = 99999; i >= 0; --i)
            DataStorageRecordRef dsrr = ds.GetRecord("key", i);
    });

    th1.join();
    th2.join();
    th3.join();
    th4.join();

    ColorizedPrint("\r\t\tReading test started: pass", ConsoleTextColor::Green);
}

void MultithreadingKeysTest()
{
    DataStorage ds;
    std::condition_variable cv;

    std::cout << "\t\tKeys test started: " << std::flush;

    auto timeout = std::chrono::steady_clock::now() + std::chrono::milliseconds(500);

    // Thread for adding keys
    std::thread th1([&]()
    {
        std::mutex mtx;
        std::unique_lock<std::mutex> lk(mtx);
    
        // Wait for start all threads at same time
        cv.wait_until(lk, timeout);

        for (int i = 0; i < 100000; ++i)
            ds.SetKey(std::to_string(i), i);
        
    });

    // Thread for removing keys
    std::thread th2([&]()
    {
        std::mutex mtx;
        std::unique_lock<std::mutex> lk(mtx);
    
        // Wait for start all threads at same time
        cv.wait_until(lk, timeout);

        for (int i = 0; i < 100000; ++i)
            ds.RemoveKey(std::to_string(i));
    });

    th1.join();
    th2.join();

    ColorizedPrint("\r\t\tKeys test started: pass", ConsoleTextColor::Green);
}

void MultithreadingRecordsTest()
{
    DataStorage ds;

    std::condition_variable cv;

    ds.SetKey("id", -1);
    ds.SetKey("num", -1);

    std::cout << "\t\tRecords test started: " << std::flush;

    auto timeout = std::chrono::steady_clock::now() + std::chrono::milliseconds(5);

    // Thread for adding records
    std::thread th1([&]()
    {
        for (int i = 0; i < 100000; ++i)
            ds.CreateRecord({ {"id", i} });
        
    });

    // Thread for changing records
    std::thread th2([&]()
    {
        for (int i = 0; i < 100000; ++i)
        {
            DataStorageRecordRef dsrr = ds.GetRecord("id", i);
            dsrr.SetData("num", i);
        }
    });

    // Thread for removing records
    std::thread th3([&]()
    {
        std::mutex mtx;
        std::unique_lock<std::mutex> lk(mtx);
    
        // Wait for start all threads at same time
        cv.wait_until(lk, timeout);

        for (int i = 0; i < 100000; ++i)
        {
            DataStorageRecordRef dsrr = ds.GetRecord("id", i);
            ds.EraseRecord(dsrr);
        }
    });

    th1.join();
    th2.join();
    th3.join();

    ColorizedPrint("\r\t\tRecords test started: pass", ConsoleTextColor::Green);
}   