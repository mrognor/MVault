#include "Starter.h"

Starter::Starter(std::size_t delay) noexcept : Delay(delay) {}

void Starter::Wait() noexcept
{
    std::mutex Mtx;
    std::unique_lock<std::mutex> Lk(Mtx);
    Cv.wait_for(Lk, std::chrono::microseconds(Delay));
}