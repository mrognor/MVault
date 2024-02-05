#include "Tests.h"

void MultithreadingReadTest()
{
    mvlt::Vault vlt;
    std::condition_variable cv;

    std::cout << "\t\tReading test started: " << std::flush;

    vlt.SetKey<int>("key", -1);

    for (int i = 0; i < 100000; ++i)
        vlt.CreateRecord({ {"key", i} });

    auto timeout = std::chrono::steady_clock::now() + std::chrono::milliseconds(500);


    std::thread th1([&]()
    {
        std::mutex mtx;
        std::unique_lock<std::mutex> lk(mtx);
    
        // Wait for start all threads at same time
        cv.wait_until(lk, timeout);

        for (int i = 0; i < 100000; ++i)
            mvlt::VaultRecordRef vltrr = vlt.GetRecord("key", i);
    });

    // Thread for forward records reading
    std::thread th2([&]()
    {
        std::mutex mtx;
        std::unique_lock<std::mutex> lk(mtx);
    
        // Wait for start all threads at same time
        cv.wait_until(lk, timeout);

        for (int i = 0; i < 100000; ++i)
            mvlt::VaultRecordRef vltrr = vlt.GetRecord("key", i);
    });

    // Thread for backward records reading
    std::thread th3([&]()
    {
        std::mutex mtx;
        std::unique_lock<std::mutex> lk(mtx);
    
        // Wait for start all threads at same time
        cv.wait_until(lk, timeout);

        for (int i = 99999; i >= 0; --i)
            mvlt::VaultRecordRef vltrr = vlt.GetRecord("key", i);
    });

        // Thread for backward records reading
    std::thread th4([&]()
    {
        std::mutex mtx;
        std::unique_lock<std::mutex> lk(mtx);
    
        // Wait for start all threads at same time
        cv.wait_until(lk, timeout);

        for (int i = 99999; i >= 0; --i)
            mvlt::VaultRecordRef vltrr = vlt.GetRecord("key", i);
    });

    th1.join();
    th2.join();
    th3.join();
    th4.join();

    ColorizedPrint("\r\t\tReading test started: pass", ConsoleTextColor::Green);
}

void MultithreadingKeysTest()
{
    mvlt::Vault vlt;
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
            vlt.SetKey(std::to_string(i), i);
        
    });

    // Thread for removing keys
    std::thread th2([&]()
    {
        std::mutex mtx;
        std::unique_lock<std::mutex> lk(mtx);
    
        // Wait for start all threads at same time
        cv.wait_until(lk, timeout);

        for (int i = 0; i < 100000; ++i)
            vlt.RemoveKey(std::to_string(i));
    });

    th1.join();
    th2.join();

    ColorizedPrint("\r\t\tKeys test started: pass", ConsoleTextColor::Green);
}

void MultithreadingRecordsTest()
{
    mvlt::Vault vlt;

    std::condition_variable cv;

    vlt.SetKey("id", -1);
    vlt.SetKey("num", -1);

    std::cout << "\t\tRecords test started: " << std::flush;

    auto timeout = std::chrono::steady_clock::now() + std::chrono::milliseconds(5);

    // Thread for adding records
    std::thread th1([&]()
    {
        for (int i = 0; i < 100000; ++i)
            vlt.CreateRecord({ {"id", i} });
        
    });

    // Thread for changing records
    std::thread th2([&]()
    {
        for (int i = 0; i < 100000; ++i)
        {
            mvlt::VaultRecordRef vltrr = vlt.GetRecord("id", i);
            vltrr.SetData("num", i);
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
            mvlt::VaultRecordRef vltrr = vlt.GetRecord("id", i);
            vlt.EraseRecord(vltrr);
        }
    });

    th1.join();
    th2.join();
    th3.join();

    ColorizedPrint("\r\t\tRecords test started: pass", ConsoleTextColor::Green);
}   