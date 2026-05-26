#include <profiler/profiler.h>
#include <thread>
#include <iostream>
#include <vector>
#include <cmath>

// ------------------------------------------------------------------
// 1. Simple function with a single scope
// ------------------------------------------------------------------
void simpleFunction() {
    PROFILE_FUNCTION();
    int x = 0;
    for (int i = 0; i < 1000; ++i) x += i;
    std::cout << "simpleFunction done\n";
}

// ------------------------------------------------------------------
// 2. Nested manual scopes
// ------------------------------------------------------------------
void nestedScopes() {
    PROFILE_FUNCTION();
    {
        PROFILE_SCOPE("outer");
        {
            PROFILE_SCOPE("middle");
            {
                PROFILE_SCOPE("inner");
                int x = 0; for (int i=0;i<5000;++i) x|=i;
            }
        }
    }
}

// ------------------------------------------------------------------
// 3. Function calling another function (two levels)
// ------------------------------------------------------------------
void leafFunction() {
    PROFILE_FUNCTION();
    int s = 0;
    for (int i=0;i<2000;++i) s+=i;
}
void callTree() {
    PROFILE_FUNCTION();
    leafFunction();
}

// ------------------------------------------------------------------
// 4. Three-level call chain
// ------------------------------------------------------------------
void deepLeaf() { PROFILE_FUNCTION(); volatile int x=0; for(int i=0;i<1000;++i)x++; }
void midLevel() { PROFILE_FUNCTION(); deepLeaf(); }
void topLevel() { PROFILE_FUNCTION(); midLevel(); }

// ------------------------------------------------------------------
// 5. Recursive function (limited depth)
// ------------------------------------------------------------------
void recursive(int depth) {
    PROFILE_FUNCTION();
    if (depth <= 0) return;
    volatile int z = 0;
    for (int i=0;i<100;++i) z+=i;
    recursive(depth-1);
}

// ------------------------------------------------------------------
// 6. Early return inside a profiled scope
// ------------------------------------------------------------------
void earlyReturn(bool doReturn) {
    PROFILE_FUNCTION();
    {
        PROFILE_SCOPE("early scope");
        if (doReturn) return;   // destructor still fires for the scope
        int x = 0;
        for (int i=0;i<1000;++i) x+=i;
    }
}

// ------------------------------------------------------------------
// 7. Exception throw and catch inside a profiled function
// ------------------------------------------------------------------
void thrower() {
    PROFILE_FUNCTION();
    {
        PROFILE_SCOPE("before throw");
        throw std::runtime_error("test exception");
    }
}
void exceptionTest() {
    PROFILE_FUNCTION();
    try {
        thrower();
    } catch (...) {
        // ignored
    }
}

// ------------------------------------------------------------------
// 8. Loop with scoped timers inside
// ------------------------------------------------------------------
void loopWithScopes() {
    PROFILE_FUNCTION();
    for (int i=0; i<3; ++i) {
        PROFILE_SCOPE("iteration");
        volatile int x=0;
        for (int j=0;j<10000;++j) x+=j;
    }
}

// ------------------------------------------------------------------
// 9. Function that calls another profiled function multiple times
// ------------------------------------------------------------------
void helper() { PROFILE_FUNCTION(); volatile int x=0; for(int i=0;i<500;++i)x+=i; }
void multipleCalls() {
    PROFILE_FUNCTION();
    for (int i=0; i<5; ++i) {
        helper();
    }
}

// ------------------------------------------------------------------
// 10. Two separate functions to compare flat times
// ------------------------------------------------------------------
void busyWork1() { PROFILE_FUNCTION(); volatile int s=0; for(int i=0;i<20000;++i)s+=i; }
void busyWork2() { PROFILE_FUNCTION(); volatile int s=0; for(int i=0;i<40000;++i)s+=i; }

// ------------------------------------------------------------------
// 11. Same function with and without profiling macro (disabled by kill switch)
// ------------------------------------------------------------------
void dummyWork() {
    PROFILE_FUNCTION();
    volatile int x=0;
    for (int i=0;i<5000;++i)x++;
}

// ------------------------------------------------------------------
// 12. Function with multiple scopes on the same level
// ------------------------------------------------------------------
void multipleScopesSameLevel() {
    PROFILE_FUNCTION();
    {
        PROFILE_SCOPE("first block");
        volatile int a=0; for(int i=0;i<1000;++i)a++;
    }
    {
        PROFILE_SCOPE("second block");
        volatile int b=0; for(int i=0;i<2000;++i)b++;
    }
}

// ------------------------------------------------------------------
// 13. Scoped timer inside a conditional block
// ------------------------------------------------------------------
void conditionalScope(bool flag) {
    PROFILE_FUNCTION();
    if (flag) {
        PROFILE_SCOPE("flag true");
        volatile int x=0; for(int i=0;i<10000;++i)x+=i;
    } else {
        PROFILE_SCOPE("flag false");
        volatile int y=0; for(int i=0;i<5000;++i)y+=i;
    }
}

// ------------------------------------------------------------------
// 14. Very deep nesting (but not recursive)
// ------------------------------------------------------------------
void deepNesting() {
    PROFILE_FUNCTION();
    { PROFILE_SCOPE("level1");
        { PROFILE_SCOPE("level2");
            { PROFILE_SCOPE("level3");
                { PROFILE_SCOPE("level4");
                    volatile int x=0; for(int i=0;i<1000;++i)x++;
                }
            }
        }
    }
}

// ------------------------------------------------------------------
// 15. Function that does nothing (empty scope)
// ------------------------------------------------------------------
void emptyFunction() {
    PROFILE_FUNCTION();
    // nothing
}

// ------------------------------------------------------------------
// 16. STL vector push_back inside profiled scope (heap usage)
// ------------------------------------------------------------------
void vectorWork() {
    PROFILE_FUNCTION();
    std::vector<int> v;
    for (int i=0;i<5000;++i) v.push_back(i);
}

// ------------------------------------------------------------------
// 17. Math function with scoped operation
// ------------------------------------------------------------------
void mathHeavy() {
    PROFILE_FUNCTION();
    {
        PROFILE_SCOPE("sqrt loop");
        double sum = 0.0;
        for (double x=0.0; x<10000.0; x+=1.0) sum += std::sqrt(x);
        std::cout << "sqrt sum: " << sum << "\n";
    }
}

// ------------------------------------------------------------------
// 18. Function that calls a lambda with profiling
// ------------------------------------------------------------------
void lambdaTest() {
    PROFILE_FUNCTION();
    auto work = []() {
        PROFILE_SCOPE("inside lambda");
        volatile int x=0; for(int i=0;i<1000;++i)x++;
    };
    work();
    work();
}

// ------------------------------------------------------------------
// 19. Thread sleep to see measurable duration
// ------------------------------------------------------------------
void sleepFunction() {
    PROFILE_FUNCTION();
    {
        PROFILE_SCOPE("sleep 10ms");
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

// ------------------------------------------------------------------
// 20. Function with a loop that creates and destroys timers rapidly
// ------------------------------------------------------------------
void rapidScopes() {
    PROFILE_FUNCTION();
    for (int i=0; i<5; ++i) {
        profiler::ScopedTimer t("rapid");
        volatile int x=0; for(int j=0;j<1000;++j)x+=j;
    }
}

// ------------------------------------------------------------------
// Main driver
// ------------------------------------------------------------------
int main() {
    // Optionally start the session explicitly (if your design supports it)
    // profiler::ProfilerSession::GetInstance().BeginSession();

    std::cout << "==== Running Profiler Tests ====\n";

    simpleFunction();
    nestedScopes();
    callTree();
    topLevel();
    recursive(4);
    earlyReturn(true);
    earlyReturn(false);
    exceptionTest();
    loopWithScopes();
    multipleCalls();
    busyWork1();
    busyWork2();
    dummyWork();
    multipleScopesSameLevel();
    conditionalScope(true);
    conditionalScope(false);
    deepNesting();
    emptyFunction();
    vectorWork();
    mathHeavy();
    lambdaTest();
    sleepFunction();
    rapidScopes();

    std::cout << "\n==== Profiler Report ====\n";
    profiler::ProfilerSession::GetInstance().DumpReport();

    return 0;
}