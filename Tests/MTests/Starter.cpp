#include "Starter.h"

Starter::Starter(const int& threshold) noexcept : Threshold(threshold) {}

void Starter::Wait() noexcept
{
    std::mutex Mtx;
    std::unique_lock<std::mutex> Lk(Mtx);
    std::uniform_int_distribution<int> dist(0, Threshold);
    Cv.wait_for(Lk, std::chrono::nanoseconds(Threshold));
}