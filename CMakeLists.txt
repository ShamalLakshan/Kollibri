cmake_minimum_required(VERSION 3.16)
project(MemProf VERSION 1.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(memprof_example examples/basic_usage.cpp src/scoped_timer.cpp)
target_include_directories(memprof_example PRIVATE include)