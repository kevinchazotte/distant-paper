# distant-paper

### Build Prerequisites
* Windows Version 10/11
* Microsoft Visual Studio 2022 Community (version / edition can be modified, see [buildPrompt.bat](./cmake/buildPrompt.bat))
* Netwide Assembler ([NASM](https://www.nasm.us/))
* CMake Version 3.24 or greater ([CMake](https://cmake.org/download/))

### Build Instructions
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
