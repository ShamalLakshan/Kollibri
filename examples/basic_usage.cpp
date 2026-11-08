#include <profiler/profiler.h>
#include <thread>
#include <iostream>
#include <vector>
#include <cmath>

// ------------------------------------------------------------------
// simple function with a single scope
// ------------------------------------------------------------------
void simpleFunction() {
    PROFILE_FUNCTION();
    int x = 0;
    for (int i = 0; i < 1000; ++i) x += i;
    std::cout << "simpleFunction done\n";
}

// ------------------------------------------------------------------
// nested scopes
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
// call tree
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
// three-level call chain
// ------------------------------------------------------------------
void deepLeaf() { PROFILE_FUNCTION(); volatile int x=0; for(int i=0;i<1000;++i)x++; }
void midLevel() { PROFILE_FUNCTION(); deepLeaf(); }
void topLevel() { PROFILE_FUNCTION(); midLevel(); }

// ------------------------------------------------------------------
// recursive calls
// ------------------------------------------------------------------
void recursive(int depth) {
    PROFILE_FUNCTION();
    if (depth <= 0) return;
    volatile int z = 0;
    for (int i=0;i<100;++i) z+=i;
    recursive(depth-1);
}

// ------------------------------------------------------------------
// early return inside scope
// ------------------------------------------------------------------
void earlyReturn(bool doReturn) {
    PROFILE_FUNCTION();
    {
        PROFILE_SCOPE("early scope");
        if (doReturn) return;
        int x = 0;
        for (int i=0;i<1000;++i) x+=i;
    }
}

// ------------------------------------------------------------------
// exception throw and catch
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
    }
}

// ------------------------------------------------------------------
// loop with scopes
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
// multiple calls to helper
// ------------------------------------------------------------------
void helper() { PROFILE_FUNCTION(); volatile int x=0; for(int i=0;i<500;++i)x+=i; }
void multipleCalls() {
    PROFILE_FUNCTION();
    for (int i=0; i<5; ++i) {
        helper();
    }
}

// ------------------------------------------------------------------
// flat functions
// ------------------------------------------------------------------
void busyWork1() { PROFILE_FUNCTION(); volatile int s=0; for(int i=0;i<20000;++i)s+=i; }
void busyWork2() { PROFILE_FUNCTION(); volatile int s=0; for(int i=0;i<40000;++i)s+=i; }

// ------------------------------------------------------------------
// kill switch check
// ------------------------------------------------------------------
void dummyWork() {
    PROFILE_FUNCTION();
    volatile int x=0;
    for (int i=0;i<5000;++i)x++;
}

// ------------------------------------------------------------------
// multiple scopes on same level
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
// conditional scope
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
// deep nesting
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
// empty scope
// ------------------------------------------------------------------
void emptyFunction() {
    PROFILE_FUNCTION();
}

// ------------------------------------------------------------------
// vector work
// ------------------------------------------------------------------
void vectorWork() {
    PROFILE_FUNCTION();
    std::vector<int> v;
    for (int i=0;i<5000;++i) v.push_back(i);
}

// ------------------------------------------------------------------
// math loop
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
// lambda with profiling
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
// thread sleep
// ------------------------------------------------------------------
void sleepFunction() {
    PROFILE_FUNCTION();
    {
        PROFILE_SCOPE("sleep 10ms");
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

// ------------------------------------------------------------------
// rapid timer creation
// ------------------------------------------------------------------
void rapidScopes() {
    PROFILE_FUNCTION();
    for (int i=0; i<5; ++i) {
        profiler::ScopedTimer t("rapid");
        volatile int x=0; for(int j=0;j<1000;++j)x+=j;
    }
}

// ------------------------------------------------------------------
// worker thread
// ------------------------------------------------------------------
void workerTask() {
    PROFILE_SET_THREAD_NAME("WorkerThread");
    PROFILE_FUNCTION();
    {
        PROFILE_SCOPE("worker computation");
        volatile int sum = 0;
        for (int i = 0; i < 20000; ++i) sum += i;
    }
}

// ------------------------------------------------------------------
// Main driver
// ------------------------------------------------------------------
int main() {
    PROFILE_SET_THREAD_NAME("MainThread");

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

    // Run work on a separate worker thread
    std::thread worker(workerTask);
    worker.join();

    std::cout << "\n==== Profiler Report ====\n";
    profiler::ProfilerSession::GetInstance().DumpReport();

    return 0;
}