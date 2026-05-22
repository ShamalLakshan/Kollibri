<div align="center">
<h1>Kollibri</h1>
<img src="./project-logo/logo.png" alt="Kollibri Logo" width="200" style="max-width: 100%; height: auto;">
</div>

A lightweight C++ profiling library with RAII scoped timers, memory tracking,
and object lifetime tracking. Zero overhead when disabled.

> This project is a work in progress; I develop it to improve my C++ skills and to learn build systems and internals of standard C/CPP development.

## Prerequisites

- CMake >= 3.16
- A C++17 compiler (GCC, Clang, MSVC)
- (Optional) clangd for VS Code IntelliSense

## Build

### 1. Clone the repository

git clone https://github.com/ShamalLakshan/Kollibri.git
cd Kollibri

### 2. Configure

mkdir build
cd build
cmake ..

### 3. Build

cmake --build .

This builds `kollibri_static`, `kollibri_shared`, and `kollibri_example`.

### 4. Run the example

./kollibri_example         # On Windows: kollibri_example.exe

