#pragma once

#include <mutex>
#include <chrono>
#include <random>
#include <condition_variable>

class Starter
{
private:
    std::condition_variable Cv;
    std::size_t Threshold;
    std::mt19937 Random;
public:
    Starter(const std::size_t& threshold = 100) noexcept;

    void Wait() noexcept;
};