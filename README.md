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
