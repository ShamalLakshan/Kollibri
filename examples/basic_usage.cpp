#include "../include/profiler/scoped_timer.h"
#include <iostream>
#include <thread>

void printRand()
{
    Timer timer;
    for(int i = 0; i < 100; i++)
    {
        std::cout << "Count : " << i << std::endl;
    }
    
}

void Sleep(int sleepTime)
{
    Timer timer;
    std::this_thread::sleep_for(std::chrono::seconds(sleepTime));
}

int main()
{
    // printRand();
    Sleep(1);
    return 0;
}