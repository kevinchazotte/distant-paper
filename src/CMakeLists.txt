cmake_minimum_required(VERSION 3.24)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(src)

add_executable(src main.cpp ../proto/test.pb.cc)
target_link_libraries(src SuperBuild::abseil-cpp SuperBuild::protobuf)
