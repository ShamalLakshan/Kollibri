#include "../include/profiler/session.h"
#include <iostream>

ProfilerSession& ProfilerSession::GetInstance()
{
    static ProfilerSession m_session;
    return m_session;
}

void ProfilerSession::RecordEvent(const char* name, double duration)
{
    Event event(name, duration);
    std::cout <<  event.m_Name << " | " << event.m_Duration << '\n';

    GetInstance().updateEventList(event);

    // std::cout << "recorded: "<< ProfilerSession::GetInstance().m_eventList.size() << "\n";
}

void ProfilerSession::updateEventList(const Event& event)
{
    m_eventList.push_back(event);
    std::cout << "update call\n";
}


void ProfilerSession::DumpReport()
{
    std::cout << "Dump Event Log\n";
    std::cout << "--------------------------";
    for (auto event : m_eventList)
    {
        std::cout << "Name: " << event.m_Name << "\t| Time: " << event.m_Duration << "\t|";
    }
    std::cout << '\n';
}