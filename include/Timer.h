#include <chrono>
#include <iostream>

class Timer
{
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
    std::chrono::time_point<std::chrono::high_resolution_clock> end;
    std::chrono::time_point<std::chrono::high_resolution_clock> functionTime;
public:
    Timer();
    ~Timer();
};