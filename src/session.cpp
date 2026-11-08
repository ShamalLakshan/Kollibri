#include <chrono>
#include <cstddef>
#include <mutex>
#include <sstream>
#include "profiler/session.h"
#include "profiler/scoped_timer.h"
#include "profiler/console_writer.h"

// Defining a thread local vector pointer // Each thread has its own copy
thread_local std::vector<Event>* profiler::ProfilerSession::m_pEventList = nullptr;
thread_local std::vector<const profiler::ScopedTimer*> profiler::ProfilerSession::m_Stack;

// Map to track which threads exist
static std::map<std::thread::id, std::vector<Event>*> g_threadVectors;// requires synchronization
static std::map<std::thread::id, std::string> g_threadNames;
static std::mutex g_threadsMutex;

profiler::ProfilerSession& profiler::ProfilerSession::GetInstance()
{
    static ProfilerSession m_session;
    return m_session;
}

profiler::ProfilerSession::~ProfilerSession()
{
    // Auto-dump report on exit if not dumped yet
    if (!m_ReportDumped)
    {
        DumpReport();
    }

    // Clean up allocated per-thread event vectors
    std::lock_guard<std::mutex> lock(g_threadsMutex);
    for (auto& pair : g_threadVectors)
    {
        delete pair.second;
    }
    g_threadVectors.clear();
}

void profiler::ProfilerSession::RecordEvent(const char* name, std::chrono::nanoseconds duration, size_t depth)
{
    // Register this thread's vector on first call
    if (m_pEventList == nullptr) 
    {
        m_pEventList = new std::vector<Event>();
        std::lock_guard<std::mutex> lock(g_threadsMutex);
        g_threadVectors[std::this_thread::get_id()] = m_pEventList;
    }
    
    // NO LOCKS NEEDED - each thread writes to its own vector
    m_pEventList->push_back(Event(name, duration, depth));
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
    GetInstance().m_ReportDumped = true;
    auto allEvents = GetInstance().CollectAllThreadEvents();
    ConsoleWriter consoleWriter;
    consoleWriter.WriteHeader();
    consoleWriter.WriteEntry(allEvents, true);
    consoleWriter.WriteFooter();
}

void profiler::ProfilerSession::SetThreadName(const char* name)
{
    std::lock_guard<std::mutex> lock(g_threadsMutex);
    g_threadNames[std::this_thread::get_id()] = name;
}

std::string profiler::ProfilerSession::GetThreadName(std::thread::id id)
{
    std::lock_guard<std::mutex> lock(g_threadsMutex);
    auto it = g_threadNames.find(id);
    if (it != g_threadNames.end())
    {
        return it->second;
    }

    // Default to string representation of thread id if no name was set
    std::stringstream ss;
    ss << id;
    return ss.str();
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