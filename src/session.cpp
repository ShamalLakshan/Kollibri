#include "../include/profiler/session.h"
#include <iostream>
#include <iomanip>

ProfilerSession& ProfilerSession::GetInstance()
{
    static ProfilerSession m_session;
    return m_session;
}

void ProfilerSession::RecordEvent(const char* name, double duration)
{
    Event event(name, duration);

    GetInstance().updateEventList(event);
}

void ProfilerSession::updateEventList(const Event& event)
{
    m_eventList.push_back(event);
}


void ProfilerSession::DumpReport()
{
    std::cout << "Dump Event Log\n";
    std::cout << "--------------------------\n";
    std::cout << std::left 
            << std::setw(20) << "Name" 
            << std::setw(10) << "Time" << "\n";
    for (auto event : m_eventList)
    {
        std::cout << std::setfill('-') << std::setw(32) << "-" << std::setfill(' ') << "\n";
        std::cout << std::left 
                << std::setw(20) << event.m_Name 
                << std::setw(10) << event.m_Duration << "\n";
    }
    std::cout << '\n';
}