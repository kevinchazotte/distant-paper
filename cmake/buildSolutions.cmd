@echo off

FOR %%p in (distant-paper) do (
  FOR %%c in (Release RelWithDebInfo Debug) do (
    echo.
    echo Configuring project "%%p" with config "%%c" to: "%~dp0../build/bld/x64/%%c/%%p/"
    echo.
    cmake -DCMAKE_BUILD_TYPE=%%c -DCMAKE_CONFIGURATION_TYPES=%%c -DCMAKE_C_COMPILER=cl -DCMAKE_CXX_COMPILER=cl -S "%~dp0../../%%p/" -B "%~dp0../build/bld/x64/%%c/%%p/"
  )
)
