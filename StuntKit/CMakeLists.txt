# CMakeList.txt : CMake project for StuntKit, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DUNICODE)
add_definitions(-D_UNICODE)

add_executable (StuntKit WIN32 "StuntKit.cpp" "StuntKit.hpp" "InjectDll.cpp" "InjectDll.hpp")

add_executable (StuntKit_D3D WIN32 "StuntKit.cpp" "StuntKit.hpp" "InjectDll.cpp" "InjectDll.hpp")
target_compile_definitions(StuntKit_D3D PUBLIC EXE_NAME=L"StuntGP_D3D.exe")

add_executable (StuntKit_Glide WIN32 "StuntKit.cpp" "StuntKit.hpp" "InjectDll.cpp" "InjectDll.hpp")
target_compile_definitions(StuntKit_Glide PUBLIC EXE_NAME=L"StuntGP_Glide.exe")

# TODO: Add tests and install targets if needed.
