#pragma once
#ifndef MACROS_H
#define MACROS_H

#ifdef ENABLE_PROFILING
    // scope macro : required name input
    #define PROFILE_SCOPE(name) profiler::ScopedTimer _timer_##__LINE__(name)

    // automated function macro : just the call is enough.
    #define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCTION__)
#else
    // this else block resolves to nothing if profiling is disabled
    #define PROFILE_SCOPE(name)
    #define PROFILE_FUNCTION()
#endif

#endif