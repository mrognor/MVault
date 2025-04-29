#include "Measure.h"

Measure::Measure(int64_t& delay) noexcept : Delay(delay)
{
    Start = std::chrono::high_resolution_clock::now();
}

Measure::Measure(int64_t& delay, std::chrono::microseconds a) noexcept : Delay(delay)
{
    Start = std::chrono::high_resolution_clock::now();
    Unit = MeasureUnit::micro;
}

Measure::Measure(int64_t& delay, std::chrono::nanoseconds a) noexcept : Delay(delay)
{
    Start = std::chrono::high_resolution_clock::now();
    Unit = MeasureUnit::nano;
}

void Measure::Stop() noexcept
{
    if (Unit == MeasureUnit::micro)
        Delay = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - Start).count();
    else if (Unit == MeasureUnit::nano)
        Delay = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - Start).count();
    
    IsRun = false;
}

Measure::~Measure() noexcept
{
    if (IsRun) Stop();
}