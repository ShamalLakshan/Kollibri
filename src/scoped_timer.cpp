#include "../include/profiler/scoped_timer.h"
#include <iostream>

profiler::ScopedTimer::ScopedTimer(const char* name) : m_Name(name)
{
    m_StartTime = std::chrono::high_resolution_clock::now();

}

profiler::ScopedTimer::~ScopedTimer() noexcept
{
    Stop();
}

void profiler::ScopedTimer::Stop()
{
    auto endTime = std::chrono::high_resolution_clock::now();

    auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTime).time_since_epoch().count();

    auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTime).time_since_epoch().count();

    auto duration = end - start;
    double ms = duration * 0.001;

    std::cout << "Function Name: " << this->m_Name << " | Function Time: " << duration << "us (" << ms << "ms)\n";

}