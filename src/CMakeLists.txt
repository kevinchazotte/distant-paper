cmake_minimum_required(VERSION 3.24)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(src)

add_executable(src main.cpp ../proto/greeter.grpc.pb.cc ../proto/greeter.pb.cc)
target_include_directories(src PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../proto/")
target_link_libraries(src SuperBuild::abseil-cpp SuperBuild::protobuf SuperBuild::grpc SuperBuild::SFML)
