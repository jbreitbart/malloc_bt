# General project settings
project(malloc_bt)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_BUILD_TYPE Release)

OPTION(runtime "Measure runtime" OFF)
OPTION(zero "Zero memory on free" ON)

########
# Compiler/linker options based on the different compilers
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  ADD_DEFINITIONS(-g -Ofast -march=native -std=c++11)

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # Using GCC
  ADD_DEFINITIONS(-g -O2 -march=native -std=c++11)

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
  # Using Intel C++
  ADD_DEFINITIONS(-g -fast -std=c++11)
endif()

if (runtime)
    ADD_DEFINITIONS(-DRUNTIME)
endif()
if (zero)
    ADD_DEFINITIONS(-DZERO)
endif()
########

add_library(malloc_bt SHARED malloc_bt.cpp)

add_executable(simple_test simple_test.cpp)
