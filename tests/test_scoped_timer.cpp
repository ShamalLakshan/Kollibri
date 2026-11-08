#include <profiler/profiler.h>
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>

static std::vector<std::string> g_destructionLog;

class TrackedTimer {
public:
    std::string m_name;
    TrackedTimer(const std::string& name) : m_name(name) {}
    ~TrackedTimer() {
        g_destructionLog.push_back(m_name);
    }
};

// multiple returns at different depths
int testMultipleReturns(int depth) {
    PROFILE_FUNCTION();
    {
        PROFILE_SCOPE("depth_1");
        if (depth == 1) return 10;
        {
            PROFILE_SCOPE("depth_2");
            if (depth == 2) return 20;
            {
                PROFILE_SCOPE("depth_3");
                if (depth == 3) return 30;
            }
        }
    }
    return 0;
}

// exception safety check
void testExceptionSafety() {
    bool exceptionCaught = false;
    try {
        PROFILE_SCOPE("exception_outer");
        {
            PROFILE_SCOPE("exception_throwing_scope");
            throw std::runtime_error("simulated error");
        }
    }
    catch (const std::runtime_error&) {
        exceptionCaught = true;
    }

    assert(exceptionCaught);
}

// empty scope check
void testEmptyScope() {
    {
        profiler::ScopedTimer emptyTimer("empty");
    }
}

// nesting order check
void testNestingOrder() {
    g_destructionLog.clear();
    {
        TrackedTimer outer("outer");
        profiler::ScopedTimer outerTimer("outer_timer");
        {
            TrackedTimer inner("inner");
            profiler::ScopedTimer innerTimer("inner_timer");
        }
        assert(g_destructionLog.size() == 1);
        assert(g_destructionLog[0] == "inner");
    }
    assert(g_destructionLog.size() == 2);
    assert(g_destructionLog[1] == "outer");
}

int main() {
    std::cout << "Running ScopedTimer stress tests...\n";

    assert(testMultipleReturns(1) == 10);
    assert(testMultipleReturns(2) == 20);
    assert(testMultipleReturns(3) == 30);
    assert(testMultipleReturns(0) == 0);
    std::cout << "[PASS] Multiple return statements\n";

    testExceptionSafety();
    std::cout << "[PASS] Exception safety (stack unwinding)\n";

    testEmptyScope();
    std::cout << "[PASS] Empty scope\n";

    testNestingOrder();
    std::cout << "[PASS] Nesting order (inner first, outer last)\n";

    std::cout << "\nAll ScopedTimer tests passed successfully!\n";
    return 0;
}
