# General project settings
project(malloc_bt)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_BUILD_TYPE Release)

########
# Compiler/linker options based on the different compilers
find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  ADD_DEFINITIONS(-g -Ofast -march=native -std=c++11)

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # Using GCC
  ADD_DEFINITIONS(-g -O2 -march=native -std=c++11)

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
  # Using Intel C++
  ADD_DEFINITIONS(-g -fast -std=c++11)
endif()
########

add_library(malloc_bt SHARED malloc_bt.cpp)

add_executable(test test.cpp)
