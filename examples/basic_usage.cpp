#include <profiler/profiler.h>
#include <thread>
#include <iostream>

void printRand()
{
    PROFILE_FUNCTION(); // Automatically names this "printRand"
    {
        PROFILE_SCOPE("just for loop");
        int j;
        for(int i = 0; i < 100; i++)
        {
            j *= j;
        }
    }
}

void printRand02()
{
    PROFILE_FUNCTION();
    int j = 0;
    for(int i = 0; i < 100; i++)
    {
        j+=2;
    }
}

// void Sleep(int sleepTime)
// {
//     PROFILE_FUNCTION(); // Automatically names this "Sleep"
    
//     std::this_thread::sleep_for(std::chrono::seconds(sleepTime));
// }

int main()
{   
    printRand();
    printRand02();
    // Sleep(1);

    ProfilerSession::GetInstance().DumpReport();

    return 0;
}
