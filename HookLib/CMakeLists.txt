# CMakeList.txt : CMake project for StuntKit, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library (HookLib SHARED "HookLib.cpp")


# TODO: Add tests and install targets if needed.
