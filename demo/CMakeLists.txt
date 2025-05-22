cmake_minimum_required(VERSION 3.24)

set(EP_PREFIX ${CMAKE_BINARY_DIR}/_deps)
set(EP_INSTALL_DIR ${CMAKE_BINARY_DIR}/_deps/install)
set(EP_BUILD_DIR ${CMAKE_BINARY_DIR}/_deps/build)
set(EP_SOURCE_DIR ${CMAKE_BINARY_DIR}/_deps/src)

# abseil-cpp via https://github.com/abseil/abseil-cpp/releases
set(abseil-cpp_version 20250127.0)
set(abseil-cpp_tag 20250127.0)
set(abseil-cpp_url "https://github.com/abseil/abseil-cpp/archive/refs/tags/${abseil-cpp_tag}.tar.gz")
set(abseil-cpp_name "${abseil-cpp_tag}.tar.gz")
set(abseil-cpp_md5 "dbc174bbc144525c45101a36d4027e7d")

# protobuf via https://github.com/protocolbuffers/protobuf/releases
set(protobuf_version 30.0)
set(protobuf_tag v30.0)
set(protobuf_url "https://github.com/protocolbuffers/protobuf/archive/refs/tags/${protobuf_tag}.tar.gz")
set(protobuf_name "${protobuf_tag}.tar.gz")
set(protobuf_md5 "63f3d276a8114f28ee1649443935e4af")

# grpc via https://github.com/grpc/grpc/releases
set(grpc_version 1.71.0)
set(grpc_tag v1.71.0)
set(grpc_url "https://github.com/grpc/grpc/archive/refs/tags/${grpc_tag}.tar.gz")
set(grpc_name "${grpc_tag}.tar.gz")
set(grpc_md5 "89ad442e1b174bc5d55c554aec583fa0")

include(ExternalProject)

set(ABSL_GIT_REPOSITORY "https://github.com/abseil/abseil-cpp.git")
set(ABSL_GIT_TAG "20250127.0")

ExternalProject_Add(
    absl
    GIT_REPOSITORY    ${ABSL_GIT_REPOSITORY}
    GIT_TAG           ${ABSL_GIT_TAG}
    SOURCE_DIR        "${EP_SOURCE_DIR}/abseil-cpp"
    BINARY_DIR        "${EP_BUILD_DIR}/abseil-cpp"
    INSTALL_DIR       ${EP_INSTALL_DIR}
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${EP_INSTALL_DIR}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_CXX_STANDARD=17
        -DCMAKE_POSITION_INDEPENDENT_CODE=ON
        -DBUILD_SHARED_LIBS=OFF
        -DABSL_BUILD_TESTING=OFF
        -DABSL_USE_GOOGLETEST_HEAD=OFF
        -DABSL_MSVC_STATIC_RUNTIME:BOOL=ON
        -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded$<$<CONFIG:Debug>:Debug>
    UPDATE_COMMAND    ""
)

set(PROTOBUF_GIT_REPOSITORY "https://github.com/protocolbuffers/protobuf.git")
set(PROTOBUF_GIT_TAG "v30.0")

ExternalProject_Add(
    protobuf
    GIT_REPOSITORY    ${PROTOBUF_GIT_REPOSITORY}
    GIT_TAG           ${PROTOBUF_GIT_TAG}
    SOURCE_DIR        "${EP_SOURCE_DIR}/protobuf"
    BINARY_DIR        "${EP_BUILD_DIR}/protobuf"
    INSTALL_DIR       ${EP_INSTALL_DIR}
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${EP_INSTALL_DIR}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_CXX_STANDARD=17
        -DCMAKE_POSITION_INDEPENDENT_CODE=ON
        -Dprotobuf_BUILD_TESTS=OFF
        -Dprotobuf_BUILD_SHARED_LIBS=OFF
        -DCMAKE_PREFIX_PATH=${EP_INSTALL_DIR}
        -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded$<$<CONFIG:Debug>:Debug>
    DEPENDS           absl
    UPDATE_COMMAND    ""
)

set(GRPC_GIT_REPOSITORY "https://github.com/grpc/grpc.git")
set(GRPC_GIT_TAG "v1.71.0")
set(GRPC_GIT_SUBMODULES "third_party/protobuf" "third_party/abseil-cpp") # Let gRPC use our versions

ExternalProject_Add(
    grpc
    GIT_REPOSITORY    ${GRPC_GIT_REPOSITORY}
    GIT_TAG           ${GRPC_GIT_TAG}
    GIT_SUBMODULES
    SOURCE_DIR        "${EP_SOURCE_DIR}/grpc"
    BINARY_DIR        "${EP_BUILD_DIR}/grpc"
    INSTALL_DIR       ${EP_INSTALL_DIR}
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${EP_INSTALL_DIR}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_CXX_STANDARD=17
        -DCMAKE_POSITION_INDEPENDENT_CODE=ON
        -DgRPC_INSTALL=ON
        -DgRPC_BUILD_CODEGEN=ON
        -DgRPC_BUILD_GRPC_CPP_PLUGIN=ON
        -DgRPC_BUILD_TESTS=OFF
        -DBUILD_SHARED_LIBS=OFF
        -DgRPC_STATIC_LINKAGE=ON
        -DgRPC_ABSL_PROVIDER=package
        -DgRPC_PROTOBUF_PROVIDER=package
        -DgRPC_ZLIB_PROVIDER=module
        -DgRPC_CARES_PROVIDER=module
        -DgRPC_SSL_PROVIDER=module
        -DCMAKE_PREFIX_PATH=${EP_INSTALL_DIR}
        -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded$<$<CONFIG:Debug>:Debug>
    DEPENDS           absl protobuf
    UPDATE_COMMAND    ""
)
