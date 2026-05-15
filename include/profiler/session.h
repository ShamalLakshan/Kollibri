#pragma once
#ifndef SESSION_H
#define SESSION_H

#include <memory>
#include <vector>

struct Event {
    const char* m_Name;
    double m_Duration;
    size_t m_Depth;         // for nesting later
                          // later add type tag
    Event(const char* name, double duration) : m_Name(name), m_Duration(duration) {};
};

class ProfilerSession
{
private:
    std::vector<Event> m_eventList;
public:
    static ProfilerSession GetInstance();
    void RecordEvent(const char* name, double duration);
    void DumpReport(); // called at end of main or on demand
    void updateEventList(Event& event);

    // // delete copy constructor
    // ProfilerSession(const ProfilerSession&) = delete;
    // // delete copy assign oprator
    ProfilerSession& operator=(const ProfilerSession&) = delete;
    // // delete move constructor
    // ProfilerSession(ProfilerSession&&) = delete;
    // //delete move assign operator
    // ProfilerSession& operator=(ProfilerSession&&) = delete;
};


#endif