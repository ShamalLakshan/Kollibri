#include "../include/profiler/session.h"
#include <iostream>
#include <iomanip>
#include <mutex>

// Defining a thread local vector // Each thread has its own copy
thread_local std::vector<Event> ProfilerSession::m_eventList;

// Map to track which threads exist
static std::map<std::thread::id, std::vector<Event>*> g_threadVectors;// requires synchronization
static std::mutex g_threadsMutex;

ProfilerSession& ProfilerSession::GetInstance()
{
    static ProfilerSession m_session;
    return m_session;
}

void ProfilerSession::RecordEvent(const char* name, std::chrono::nanoseconds duration)
{
    // Register this threads vector on first call
    static thread_local bool registered = false;
    if (!registered) {
        std::lock_guard<std::mutex> lock(g_threadsMutex);
        g_threadVectors[std::this_thread::get_id()] = &m_eventList;
        registered = true;
    }
    
    // NO LOCKS NEEDED - each thread writes to its own vector
    m_eventList.push_back(Event(name, duration));
}

std::map<std::thread::id, std::vector<Event>> ProfilerSession::CollectAllThreadEvents()
{
    // This needs some way to collect from all threads
    // One pattern: each thread registers its vector when first recording
    // Another: use a callback or thread exit handler
    
    std::map<std::thread::id, std::vector<Event>> allEvents;
    
    {
        std::lock_guard<std::mutex> lock(g_threadsMutex);
        for (auto& [threadId, events] : g_threadVectors) {
            allEvents[threadId] = *events;  // Copy all events
        }
    }
    
    return allEvents;
}

void ProfilerSession::DumpReport()
{
    auto allEvents = GetInstance().CollectAllThreadEvents();
    
    std::cout << "=== Profiler Report ===\n";
    std::cout << std::setw(20) << "Thread ID" 
              << std::setw(25) << "Event Name" 
              << std::setw(20) << "Duration (ns)" << "\n";
    std::cout << std::string(65, '-') << "\n";
    
    for (auto& [threadId, events] : allEvents) {
        for (auto& event : events) {
            std::cout << std::setw(20) << threadId
                      << std::setw(25) << event.name
                      << std::setw(20) << event.duration.count() << "\n";
        }
    }
}