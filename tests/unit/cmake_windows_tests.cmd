@echo off

::::::::::::::::::::::::::::
:START
::::::::::::::::::::::::::::
setlocal & pushd .


call:getCMakeExecutable cmake_executable

"%cmake_executable%" --version

::Move to destination folder
cd /D %~dp0/projects


::check if bincrafters repo is added
conan remote list | findstr other-conan-repo >nul 2>>&1
if errorlevel 1 (
	::add it
    conan remote add other-conan-repo https://api.bintray.com/conan/bincrafters/public-conan
)


::Move to destination folder
cd /D %~dp0/projects

::get/build dependencies
conan install --build missing -s compiler.runtime=MTd -s arch=x86 -s build_type=Debug ../sources
if errorlevel 1 (
	exit 1
)
conan install --build missing -s compiler.runtime=MT -s arch=x86 -s build_type=Release ../sources
if errorlevel 1 (
	exit 1
)



"%cmake_executable%" %~dp0/sources


goto:eof

:: Function to find cmake executable
:getCMakeExecutable
@setlocal enableextensions enabledelayedexpansion
set file=sources/CMakeListsUserConfig.txt
set key=ALTERNATIVE_CMAKE_ROOT
set value=
for /f "usebackq delims=" %%a in ("!file!") do (
   set ln=%%a
   if not "x!ln:~0,1!"=="x#" (
      for /f tokens^=2^ delims^=^(^)^  %%b in ("!ln!") do (
         set current_key=%%b
         if "x!key!"=="x!current_key!" (
            for /f tokens^=2^ delims^=^" %%c in ("!ln!") do (
               set value=%%c
            )
         )
      )
   )
)
if [!value!] == [] (
   echo Use cmake from PATH
   endlocal&set "%~1=cmake"
) else (
   echo Use cmake from "%value%"
   endlocal&set "%~1=%value%/bin/cmake"
)
goto:eof
