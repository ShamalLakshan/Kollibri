#pragma once

#if defined(_WIN32)  // on Windows (MSVC or MinGW)
  #if defined(MEMPROF_STATIC_BUILD)
    #define KOLLIBRI_API  // Static lib: no special decoration needed
  #elif defined(MEMPROF_BUILD_DLL)
    #define KOLLIBRI_API __declspec(dllexport)  // Building the DLL: export symbols
  #else
    #define KOLLIBRI_API __declspec(dllimport)  // Using the DLL: import symbols
  #endif
#elif defined(__GNUC__) || defined(__clang__)  // GCC or Clang on Linux/macOS/MinGW
  #define KOLLIBRI_API __attribute__((visibility("default")))  // Mark public symbols visible in shared library
#else
  #define KOLLIBRI_API  // Unknown compiler: hope for the best - no visibility control
#endif