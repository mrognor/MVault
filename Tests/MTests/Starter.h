#pragma once

#include <chrono>
#include <mutex>
#include <condition_variable>

class Starter
{
private:
    std::condition_variable Cv;
    std::size_t Delay;
public:
    Starter(std::size_t delay = 10) noexcept;

    void Wait() noexcept;
};