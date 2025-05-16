@echo off
rem This .bat assumes you are running Visual Studio Community 2022 with this install directory.
rem For other configurations, modify this code as necessary.
if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" (
	cmd /k ""C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64"
) else (
	cmd /k && echo Check Visual Studio Installation
)
