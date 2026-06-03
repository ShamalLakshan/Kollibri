#pragma once
#ifndef EVENT_H
#define EVENT_H

#include <chrono>
#include <profiler/export.h>

struct Event {
    const char* name;
    std::chrono::nanoseconds duration;
    size_t depth;        // for nesting later
    
    Event(const char* name, std::chrono::nanoseconds duration, size_t depth = 0) 
        : name(name), duration(duration), depth(depth) {} // ctor
};

#endif