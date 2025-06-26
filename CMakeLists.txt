# Top-level CMakeLists.txt
cmake_minimum_required(VERSION 3.24)

set(proj distant-paper)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/install")

project(${proj} VERSION 1.0.0)

set(PROJECT_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build/")

# Check for SuperBuild completion
if(NOT EXISTS "${PROJECT_BUILD_DIR}/build_complete.marker")
  message(FATAL_ERROR "Dependencies not built. Please see bootstrap.cmake first.")
endif()

# Find the libraries installed by SuperBuild
find_package(build REQUIRED PATHS "${PROJECT_BUILD_DIR}")

file(COPY ${CMAKE_CURRENT_LIST_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(src)

install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/assets DESTINATION .)

if(WIN32 AND ${CMAKE_BUILD_TYPE} STREQUAL "Release")
  set(CPACK_GENERATOR "NSIS")
  set(CPACK_NSIS_PACKAGE_NAME "${proj}")
  set(CPACK_NSIS_DISPLAY_NAME "${proj}")
  include(CPack)
endif()
