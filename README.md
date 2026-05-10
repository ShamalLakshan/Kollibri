# MemProf

A lightweight C++ profiling library with RAII scoped timers, memory tracking,
and object lifetime instrumentation. Zero overhead when disabled.

## Prerequisites

- CMake >= 3.16
- A C++17 compiler (GCC, Clang, MSVC)
- (Optional) clangd for VS Code IntelliSense

## Build

### 1. Clone the repository

git clone https://github.com/your-org/MemProf.git
cd MemProf

### 2. Configure

mkdir build
cd build
cmake ..

### 3. Build

cmake --build .

### 4. Run the example

./memprof_example          # On Windows: memprof_example.exe

## clangd setup (VS Code)

For accurate IntelliSense, generate a compilation database and create a
symlink at the project root:

1. Ensure your CMakeLists.txt contains:

   set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

2. Reconfigure:

   cd build && cmake ..

3. From the project root, create the link:

   ln -s build/compile_commands.json .

   (On Windows you can use `mklink` from an admin command prompt or simply
   copy the file with `cp build/compile_commands.json .`)

4. Restart the clangd language server in VS Code.

## Project structure

MemProf/
├── include/profiler/   # Public headers
├── src/                # Implementation
│   └── writers/        # Output backends (console, JSON)
├── examples/           # Usage examples
├── tests/              # Unit tests
├── CMakeLists.txt      # Build configuration
└── README.md           # This file