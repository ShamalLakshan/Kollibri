cmake_minimum_required(VERSION 3.16)
project(Kollibri VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Option to toggle profiling instrumentation on or off (zero-overhead kill switch)
option(ENABLE_PROFILING "Enable profiling instrumentation" ON)

set(KOLLIBRI_SOURCES
    src/scoped_timer.cpp
    src/session.cpp
    src/writers/console_writer.cpp
)

add_library(kollibri_static STATIC ${KOLLIBRI_SOURCES})
add_library(kollibri_shared SHARED ${KOLLIBRI_SOURCES})

target_include_directories(kollibri_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(kollibri_shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_definitions(kollibri_static PUBLIC KOLLIBRI_STATIC_BUILD)
target_compile_definitions(kollibri_shared PRIVATE KOLLIBRI_BUILD_DLL)

if(ENABLE_PROFILING)
    target_compile_definitions(kollibri_static PUBLIC ENABLE_PROFILING)
    target_compile_definitions(kollibri_shared PUBLIC ENABLE_PROFILING)
endif()

# Example executable
add_executable(kollibri_example examples/basic_usage.cpp)
target_include_directories(kollibri_example PRIVATE include)
target_link_libraries(kollibri_example PRIVATE kollibri_static)

# Tests
enable_testing()
add_executable(kollibri_tests tests/test_scoped_timer.cpp)
target_include_directories(kollibri_tests PRIVATE include)
target_link_libraries(kollibri_tests PRIVATE kollibri_static)
add_test(NAME scoped_timer_tests COMMAND kollibri_tests)
