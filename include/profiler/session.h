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
<<<<<<< HEAD
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
=======
    std::vector<Event> m_eventList;
public:
    static ProfilerSession& GetInstance();
    void RecordEvent(const char* name, double duration);
    void DumpReport(); // called at end of main or on demand
    void updateEventList(const Event& event);   
>>>>>>> 42b9668f94c9c42d1a7e9122f77a755bb5062777
};

#endif