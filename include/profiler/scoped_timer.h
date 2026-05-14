#include <chrono>

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


namespace profiler 
{
    class ScopedTimer
    {
    private:
        const char* m_Name;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTime;

    public:
        // named constructor
        ScopedTimer(const char* name);

        // destructor
        ~ScopedTimer() noexcept; // destructor should be disable of throws.

        // delete copy constructor
        ScopedTimer(const ScopedTimer&) = delete;

        // delete copy assign oprator
        ScopedTimer& operator=(const ScopedTimer&) = delete;

        // delete move constructor
        ScopedTimer(ScopedTimer&&) = delete;

        //delete move assign operator
        ScopedTimer& operator=(ScopedTimer&&) = delete;

        // stop sequence isolated from destructor
        void Stop();
    };
}
