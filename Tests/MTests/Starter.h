#pragma once

#include <mutex>
#include <chrono>
#include <random>
#include <condition_variable>

class Starter
{
private:
    std::condition_variable Cv;
    int Threshold = 0;
    std::mt19937 Random;
public:
    Starter(const int& threshold = 100) noexcept;

    void Wait() noexcept;
};