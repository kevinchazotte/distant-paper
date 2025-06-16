cmake_minimum_required(VERSION 3.24)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(proj Server)
project(${proj})

set(SFML_STATIC_LIBRARIES ON)
find_package(SFML COMPONENTS Graphics Window System REQUIRED)
find_package(gRPC REQUIRED)
find_package(protobuf REQUIRED)
find_package(absl REQUIRED)

file(GLOB cpp_source_files "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB h_source_files "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB cc_source_files "${CMAKE_CURRENT_SOURCE_DIR}/../../proto/*.cc")

add_executable(${proj}
  ${cpp_source_files} ${h_source_files} ${cc_source_files}
)

target_include_directories(${proj} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../../proto/")
target_link_libraries(${proj} PUBLIC Utilities gRPC::grpc++ protobuf::libprotobuf SFML::Graphics SFML::Window SFML::System)

