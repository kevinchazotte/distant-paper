unset(projects)

list(APPEND projects abseil-cpp protobuf)

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
