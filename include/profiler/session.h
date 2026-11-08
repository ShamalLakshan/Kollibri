#pragma once
#include <cstddef>
#ifndef SESSION_H
#define SESSION_H

#include <vector>
#include <chrono>
#include <thread>
#include <string>
#include <map>
#include <profiler/Event.h>
#include <profiler/export.h>
#include <profiler/IWriter.h>


namespace profiler {
    class ScopedTimer;   // Forward declaration

    class ProfilerSession
    {
    private:
        // Each thread gets its own vector pointer (thread_local)
        static thread_local std::vector<Event>* m_pEventList;
        
        // Collect all events from all threads -- called during reporting
        std::map<std::thread::id, std::vector<Event>> CollectAllThreadEvents();
        
        // Private default constructor - singleton pattern
        ProfilerSession() = default;
        ~ProfilerSession();

        // Flag to prevent double-dumping report
        bool m_ReportDumped = false;
        
        // Delete copy operations
        ProfilerSession(const ProfilerSession&) = delete;
        ProfilerSession& operator=(const ProfilerSession&) = delete;

        static thread_local std::vector<const ScopedTimer*> m_Stack;
        
    public:
        // Singleton instance for global access
        KOLLIBRI_API static ProfilerSession& GetInstance();
        
        KOLLIBRI_API static void RecordEvent(const char* name, std::chrono::nanoseconds duration, size_t depth);
        KOLLIBRI_API static void DumpReport();

        // Thread naming operations
        KOLLIBRI_API static void SetThreadName(const char* name);
        KOLLIBRI_API static std::string GetThreadName(std::thread::id id);

        // Depth Stack Operations
        KOLLIBRI_API static void push(const ScopedTimer* timerObject);
        KOLLIBRI_API static void pop();
        KOLLIBRI_API static size_t GetDepth();
    };
}



#endif