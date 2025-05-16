cmake_minimum_required(VERSION 3.24)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
set(PROJECT_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../build/")
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_MSVC_STATIC_RUNTIME ON)

# consider: https://stackoverflow.com/questions/1208681/with-cmake-how-would-you-disable-in-source-builds
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/SuperBuildConfig.cmake" "${PROJECT_BUILD_DIR}/SuperBuildConfig.cmake" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/projects.cmake" "${PROJECT_BUILD_DIR}/projects.cmake" COPYONLY)
include("${PROJECT_BUILD_DIR}/SuperBuildConfig.cmake")

# add custom download-all step so that we can trigger a download-all build
# this allows us to ensure the download step is never skipped via a manual build target
if(NOT TARGET download-all)
  add_custom_target(download-all)
endif()

include(ExternalProject)
# add all third party projects via ExternalProject_Add
foreach(proj ${projects})
  # set cmake DOWNLOAD_DIR and SOURCE_DIR to be picked up by ExternalProject_Add call
  set(DOWNLOAD_DIR "$ENV{USERPROFILE}/Downloads")
  set(SOURCE_DIR "${PROJECT_BUILD_DIR}/src/${proj}")
  foreach(build_type Release;RelWithDebInfo;Debug)
    set(CMAKE_BUILD_TYPE ${build_type})
    set(BINARY_DIR "${PROJECT_BUILD_DIR}/bld/x64/${CMAKE_BUILD_TYPE}/${proj}")
    set(INSTALL_DIR "${PROJECT_BUILD_DIR}/x64/${CMAKE_BUILD_TYPE}/${proj}")
    # include project via "${proj}.cmake" file in this directory
    include(${proj})
    # if the project cmake file creates a `download-${proj}` target, make download-all depend on it
    if(TARGET download-${proj})
      add_dependencies(download-all download-${proj})
    endif()
  endforeach()
endforeach()
