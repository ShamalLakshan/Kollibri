#pragma once
#ifndef SESSION_H
#define SESSION_H

#include <vector>
#include <chrono>
#include <map>
#include <thread>

struct Event {
    const char* name;
    std::chrono::nanoseconds duration;
    size_t depth;        // for nesting later
    
    Event(const char* name, std::chrono::nanoseconds duration, size_t depth = 0) 
        : name(name), duration(duration), depth(depth) {} // ctor
};

class ProfilerSession
{
private:
    // Each thread gets its own vector (thread_local)
    static thread_local std::vector<Event> m_eventList;
    
    // Collect all events from all threads -- called during reporting
    std::map<std::thread::id, std::vector<Event>> CollectAllThreadEvents();
    
    // Private default constructor - singleton pattern
    ProfilerSession() = default;
    
    // Delete copy operations
    ProfilerSession(const ProfilerSession&) = delete;
    ProfilerSession& operator=(const ProfilerSession&) = delete;
    
public:
    // Singleton instance for global access
    static ProfilerSession& GetInstance();
    
    static void RecordEvent(const char* name, std::chrono::nanoseconds duration);
    static void DumpReport();
};

#endif