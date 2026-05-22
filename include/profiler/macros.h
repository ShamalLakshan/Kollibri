#pragma once
#ifndef MACROS_H
#define MACROS_H

#define MP_CONCAT_IMPL(x, y) x##y
#define MP_CONCAT(x, y) MP_CONCAT_IMPL(x, y)

#ifdef ENABLE_PROFILING
    // scope macro : required name input
    #define PROFILE_SCOPE(name) profiler::ScopedTimer MP_CONCAT(_timer_, __COUNTER__)(name)

    // automated function macro : just the call is enough.
    #define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCTION__)
#else
    // this else block resolves to nothing if profiling is disabled
    #define PROFILE_SCOPE(name)
    #define PROFILE_FUNCTION()
#endif

#endif