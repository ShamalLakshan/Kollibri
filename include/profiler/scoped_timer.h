#pragma once
#ifndef SCOPED_TIMER_H
#define SCOPED_TIMER_H

#include <profiler/export.h>
#include "./macros.h"
#include "./session.h"
#include <chrono>

namespace profiler 
{
    class ScopedTimer
    {
    private:
        const char* m_Name;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTime;

    public:
        // named constructor
        KOLLIBRI_API ScopedTimer(const char* name);

        // destructor
        KOLLIBRI_API ~ScopedTimer() noexcept; // destructor should be disable of throws.

        // delete copy constructor
        ScopedTimer(const ScopedTimer&) = delete;

        // delete copy assign oprator
        ScopedTimer& operator=(const ScopedTimer&) = delete;

        // delete move constructor
        ScopedTimer(ScopedTimer&&) = delete;

        //delete move assign operator
        ScopedTimer& operator=(ScopedTimer&&) = delete;

        // stop sequence isolated from destructor
        KOLLIBRI_API void Stop();
    };
}

#endif