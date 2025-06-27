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

if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
  set(CPACK_PACKAGE_NAME "${proj}")
  set(CPACK_PACKAGE_CONTACT "chazottek@gmail.com")
  set(CPACK_PACKAGE_DESCRIPTION "Collaborative sheet of paper")
  if(WIN32)
    set(CPACK_GENERATOR "NSIS;ZIP")
  elseif(UNIX AND ${CMAKE_BUILD_TYPE} STREQUAL "Release")
    set(CPACK_GENERATOR "TGZ;DEB")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libx11-6, libxcb1, libxau6, libxdmcp6, libxrandr2, libxext6, libxrender1, libxcursor1, libxfixes3, libxi6, libfreetype6, libbz2-1.0, zlib1g, libpng16-16, libbrotli1, libudev1, libstdc++6, libgcc-s1, libc6, libbsd0, libcap2, libmd0")
  endif()
  include(CPack)
endif()

