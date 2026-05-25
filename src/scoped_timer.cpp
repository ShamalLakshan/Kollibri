#include "../include/profiler/scoped_timer.h"

//constructor
profiler::ScopedTimer::ScopedTimer(const char* name) : m_Name(name)
{
    m_Depth = ProfilerSession::GetInstance().GetDepth();
    ProfilerSession::GetInstance().push(this);
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

    auto duration_us = end - start;  // duration in microseconds
    double ms = duration_us * 0.001;

    // Convert microseconds to nanoseconds
    auto duration_ns = std::chrono::nanoseconds(duration_us * 1000);

    // std::cout << "Record Call\n";
    ProfilerSession::GetInstance().RecordEvent(m_Name, duration_ns, m_Depth);
}