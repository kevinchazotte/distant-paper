set(library Utilities)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_BINARY_DIR})

file(GLOB cpp_source_files "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB h_source_files "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB cc_source_files "${CMAKE_CURRENT_SOURCE_DIR}/../../proto/*.cc")

set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML COMPONENTS Graphics Window System REQUIRED)
find_package(gRPC REQUIRED)
find_package(protobuf REQUIRED)
find_package(absl REQUIRED)

add_library(${library} STATIC 
  ${cpp_source_files} ${h_source_files} ${cc_source_files}
)

target_include_directories(${library} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../../proto/")
target_link_libraries(${library} PUBLIC gRPC::grpc++ protobuf::libprotobuf SFML::Graphics SFML::Window SFML::System)

