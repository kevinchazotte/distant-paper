unset(projects)

list(APPEND projects SFML abseil-cpp zlib boringssl cares re2 protobuf grpc)

# SFML via https://github.com/SFML/SFML/releases
set(SFML_version 3.0.1)
set(SFML_tag 3.0.1)
set(SFML_url "https://github.com/SFML/SFML/archive/refs/tags/${SFML_tag}.tar.gz")
set(SFML_name "SFML-${SFML_tag}.tar.gz")
set(SFML_md5 "b0cf66a658d9e5852fb7d2c6233a49ea")

# abseil-cpp via https://github.com/abseil/abseil-cpp/releases
set(abseil-cpp_version 20250127.0)
set(abseil-cpp_tag 20250127.0)
set(abseil-cpp_url "https://github.com/abseil/abseil-cpp/archive/refs/tags/${abseil-cpp_tag}.tar.gz")
set(abseil-cpp_name "${abseil-cpp_tag}.tar.gz")
set(abseil-cpp_md5 "dbc174bbc144525c45101a36d4027e7d")

# zlib via https://github.com/madler/zlib/releases
set(zlib_version 1.3.1)
set(zlib_tag v1.3.1)
set(zlib_url "https://github.com/madler/zlib/archive/refs/tags/${zlib_tag}.tar.gz")
set(zlib_name "${zlib_tag}.tar.gz")
set(zlib_md5 "ddb17dbbf2178807384e57ba0d81e6a1")

# boringssl via https://github.com/google/boringssl/releases
set(boringssl_version 0.20250311.0)
set(boringssl_tag 0.20250311.0)
set(boringssl_url "https://github.com/google/boringssl/archive/refs/tags/${boringssl_tag}.tar.gz")
set(boringssl_name "${boringssl_tag}.tar.gz")
set(boringssl_md5 "2716ecfdee919512ca372b0ada512f7a")

# cares via https://github.com/c-ares/c-ares/releases
set(cares_version 1.34.5)
set(cares_tag v1.34.5)
set(cares_url "https://github.com/c-ares/c-ares/archive/refs/tags/${cares_tag}.tar.gz")
set(cares_name "${cares_tag}.tar.gz")
set(cares_md5 "ba8e0067b17fad5cba87d3b047e906d2")

# re2 via https://github.com/google/re2/releases
set(re2_version 2024-07-02)
set(re2_tag 2024-07-02)
set(re2_url "https://github.com/google/re2/archive/refs/tags/${re2_tag}.tar.gz")
set(re2_name "${re2_tag}.tar.gz")
set(re2_md5 "73c94c78678bd2d641beaf8d5a3fd352")

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
