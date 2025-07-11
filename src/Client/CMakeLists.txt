cmake_minimum_required(VERSION 3.24)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(proj Client)
project(${proj})

file(GLOB cpp_source_files "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB h_source_files "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB cc_source_files "${CMAKE_CURRENT_SOURCE_DIR}/../../proto/*.cc")

add_executable(${proj}
  ${cpp_source_files} ${h_source_files} ${cc_source_files}
)

add_dependencies(${proj} Utilities)
target_include_directories(${proj} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../../proto/")
target_link_libraries(${proj} Utilities SuperBuild::grpc++ SuperBuild::protobuf SuperBuild::SFML)

install(TARGETS ${proj} DESTINATION .)
