#include "../include/profiler/scoped_timer.h"


Timer::Timer()
{
    this->start = std::chrono::high_resolution_clock::now();
}

Timer::~Timer()
{
    this->end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> elapsed = this->end - this->start;
    std::cout << "Function Time: " << elapsed.count() << " ms" << std::endl;
}