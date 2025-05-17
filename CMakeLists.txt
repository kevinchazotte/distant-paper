# Top-level CMakeLists.txt
cmake_minimum_required(VERSION 3.24)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

project(distant-paper)

set(PROJECT_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build/")

# Check for SuperBuild completion
if(NOT EXISTS "${PROJECT_BUILD_DIR}/build_complete.marker")
    message(FATAL_ERROR "Dependencies not built. Please see bootstrap.cmake first.")
endif()

# Find the libraries installed by SuperBuild
find_package(build REQUIRED PATHS "${PROJECT_BUILD_DIR}")

add_subdirectory(src)
