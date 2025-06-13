set(library "Utilities")

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_BINARY_DIR})

file(GLOB cpp_source_files "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB h_source_files "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB cc_source_files "${CMAKE_CURRENT_SOURCE_DIR}/../../proto/*.cc")

add_library(${library} STATIC 
  ${cpp_source_files} ${h_source_files} ${cc_source_files}
)

target_include_directories(${library} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../../proto/")
target_link_libraries(${library} SuperBuild::grpc++ SuperBuild::protobuf SuperBuild::SFML)
