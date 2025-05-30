cmake_minimum_required(VERSION 3.24)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(proj src)
project(${proj})

file(GLOB cpp_source_files "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB h_source_files "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB cc_source_files "${CMAKE_CURRENT_SOURCE_DIR}/../proto/*.cc")

add_subdirectory(Utilities)

add_executable(${proj}
  ${cpp_source_files} ${h_source_files} ${cc_source_files}
)

target_include_directories(${proj} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../proto/")
target_link_libraries(${proj} SuperBuild::abseil-cpp SuperBuild::protobuf SuperBuild::grpc SuperBuild::SFML Utilities)
