# distant-paper

A real-time collaborative Whiteboard application implementing distributed systems architecture with gRPC-based remote procedure calls and Protocol Buffer serialization. Leverages the Simple and Fast Multimedia Library for a lightweight user interface capable of simple geometric drawing operations. Follows the publish/subscribe design pattern for stream-based, low-latency client synchronization.

## Features

* Third-party library dependencies (gRPC, Protobuf, SFML) are built from source and statically linked using a custom CMake build pipeline to ensure binary portability for eventual cross-platform distribution
* Data consistency enforced via a distributed object model using Protobuf IDL to generate type-safe objects, shared across the client and server sides of the application, leveraging Protobuf's binary serialization
* Event-driven synchronization for live updates carried out by streaming Remote Procedure Calls between clients and the server, following the publish-subscribe design pattern

## How to Compile, Build, and Run Code in this Project

This project supports cross-compiling. Below are instructions for building on Linux or Windows. MacOS is not yet supported.

### Build Prerequisites

* CMake Version 3.24 or greater ([CMake](https://cmake.org/download/))
* Netwide Assembler ([NASM](https://www.nasm.us/))

#### Linux Prerequisites

* pkg-config via `pkg-config` on Ubuntu
* gcc/g++ compiler via `build-essential` on Ubuntu
* Ninja via `ninja-build` on Ubuntu
* clang/clang++ via `clang` on Ubuntu 
* SFML requirements via [SFML Dependencies](https://www.sfml-dev.org/tutorials/3.0/getting-started/build-from-source/#installing-dependencies) and [Linux Requirements](https://www.sfml-dev.org/tutorials/3.0/getting-started/cmake/#requirements):

      sudo apt update && sudo apt install libxrandr-dev libxcursor-dev xorg openbox \
      x11-xserver-utils libxi-dev libudev-dev libflac-dev libvorbis-dev libgl1-mesa-dev \
      libegl1-mesa-dev libdrm-dev libgbm-dev libfreetype6-dev libpthread-stubs0-dev \
      vorbis-tools libogg-dev libogg0

#### Windows Prerequisites

* Windows Version 10/11
* Microsoft Visual Studio 2022 Community (version / edition can be modified, see [buildPrompt.bat](./cmake/buildPrompt.bat))

### Build Instructions

#### Linux Instructions

* Clone this repository
* Navigate to the `cmake/` directory and run `buildPrompt.sh`. This will verify that the build environment is correctly set up to build this project
* From the terminal, run `cmake -P bootstrap.cmake`

This will successfully generate the build system used to build this project.

#### Windows Instructions

* Clone this repository
* Extract NASM binary files to a local directory and add that directory to the PATH environment variable (e.g. C:\Users\kevin\nasm). Create another environment variable named exactly "ASM_NASM" with value equal to the executable name ("nasm.exe" by default)
* Navigate to the `cmake/` directory and run `buildPrompt.bat`. This will open a command prompt at the `cmake/` directory
* From the build prompt, run `cmake -P bootstrap.cmake`
* Once the build succeeds, run `buildSolutions.cmd`. This will generate Visual Studio solution files in the binary directory built for this repository, which will be output to the console

You should now be able to launch the Visual Studio solution for each configuration and make modifications. To add new source code files, you should add them directly in the repository folder, not in Visual Studio, and run `buildSolutions.cmd` afterwards to retrieve them in Visual Studio. Files may then be modified and saved from Visual Studio.

### Using Protobuf Compiler to Generate Code

Note: to generate cpp code, it may be helpful to copy the `grpc_cpp_plugin.exe` executable compiled during the build process (in the gRPC binary folder) to the folder from where the `protoc.exe` executable is run.

1) Open a command prompt and navigate to the folder where the `protoc.exe` executable was compiled during the build process (the protobuf binary folder).
2) Note the input folder where the .proto source files exist and the output folder where the `.pb.*` and `.grpc.pb.*` files should be compiled to. These may be the same or different folders. The folders will be referenced below as ${input_folder} and ${output_folder}.
3) Run the following command from the terminal: `protoc.exe --cpp_out=${output_folder} --grpc_out=${output_folder} --plugin=protoc-gen-grpc=${grpc_cpp_plugin.exe_folder}\grpc_cpp_plugin.exe --proto_path=${input_folder} ${proto_file}.proto`. `${grpc_cpp_plugin.exe_folder}` may be `.` if the plugin was copied to the folder with `protoc.exe`, or should point to the plugin folder otherwise.

## Pre-built Binaries

Pre-built binaries are not yet available for this repository. They'll be available once a release is prepared for distribution. If you'd like to run this project on your own system, see the [How to Compile, Build, and Run Code in this Project](#how-to-compile-build-and-run-code-in-this-project) section for detailed information on how to get it running.

## Contact

Kevin Chazotte - chazottek@gmail.com

[distant-paper](https://github.com/kevinchazotte/distant-paper)

