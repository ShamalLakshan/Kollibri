#include <chrono>
#include <cstddef>
#include <mutex>
#include "profiler/session.h"
#include "profiler/scoped_timer.h"
#include "profiler/console_writer.h"

// Defining a thread local vector // Each thread has its own copy
thread_local std::vector<Event> profiler::ProfilerSession::m_eventList;
thread_local std::vector<const profiler::ScopedTimer*> profiler::ProfilerSession::m_Stack;

// Map to track which threads exist
static std::map<std::thread::id, std::vector<Event>*> g_threadVectors;// requires synchronization
static std::mutex g_threadsMutex;

profiler::ProfilerSession& profiler::ProfilerSession::GetInstance()
{
    static ProfilerSession m_session;
    return m_session;
}

void profiler::ProfilerSession::RecordEvent(const char* name, std::chrono::nanoseconds duration, size_t depth)
{
    // Register this threads vector on first call
    static thread_local bool registered = false;
    if (!registered) 
    {
        std::lock_guard<std::mutex> lock(g_threadsMutex);
        g_threadVectors[std::this_thread::get_id()] = &m_eventList;
        registered = true;
    }
    
    // NO LOCKS NEEDED - each thread writes to its own vector
    m_eventList.push_back(Event(name, duration, depth));
}

std::map<std::thread::id, std::vector<Event>> profiler::ProfilerSession::CollectAllThreadEvents()
{   
    std::map<std::thread::id, std::vector<Event>> allEvents;
    {
        std::lock_guard<std::mutex> lock(g_threadsMutex);
        for (auto& pair : g_threadVectors)
        {
            allEvents[pair.first] = *pair.second;
        }
    }
    
    return allEvents;
}

void profiler::ProfilerSession::DumpReport()
{
    auto allEvents = GetInstance().CollectAllThreadEvents();
    ConsoleWriter consoleWriter;
    consoleWriter.WriteHeader();
}

void profiler::ProfilerSession::push(const profiler::ScopedTimer* timerObject) 
{
    GetInstance().m_Stack.push_back(timerObject);
}

void profiler::ProfilerSession::pop()
{
    auto& stack = GetInstance().m_Stack;
    if (!stack.empty())
    {
        stack.pop_back();
    }
}

size_t profiler::ProfilerSession::GetDepth()
{
    return GetInstance().m_Stack.size();
}