#pragma once

#include <chrono>

enum class MeasureUnit : std::uint8_t
{
    micro,
    nano
};

class Measure
{
private:
    decltype(std::chrono::high_resolution_clock::now()) Start;
    bool IsRun = true;
    std::int64_t& Delay;
    
    MeasureUnit Unit = MeasureUnit::micro;
public:
    Measure(int64_t& delay) noexcept;
    Measure(int64_t& delay, std::chrono::microseconds a) noexcept;
    Measure(int64_t& delay, std::chrono::nanoseconds a) noexcept;

    void Stop() noexcept;

    ~Measure() noexcept;
};
