@echo off

::::::::::::::::::::::::::::
:START
::::::::::::::::::::::::::::
setlocal & pushd .


@echo Yadoms build for Windows
@echo ========================
@echo.
@echo To enable Windows XP compatiblity, please enter the platform toolset
@echo.
@echo v120_xp : platform toolset "Visual Studio 2013 - WindowsXP"
@echo v140_xp : platform toolset "Visual Studio 2015 - WindowsXP"
@echo v141_xp : platform toolset "Visual Studio 2017 - WindowsXP"
@echo Leave empty to disable Windows XP compatibility
@echo.
@echo.

set xp_compatibility=%1%
call:getCMakeExecutable cmake_executable

"%cmake_executable%" --version

::Move to destination folder
cd /D %~dp0/projects

if [%xp_compatibility%]==[] (
   @echo Using default generator WITHOUT WindowsXP support
	"%cmake_executable%" %~dp0/sources
   goto:eof
)
if [%xp_compatibility%]==["v120_xp"] (
   @echo Generating using "Visual Studio 2013" with WindowsXP support
	"%cmake_executable%" %~dp0/sources -T %xp_compatibility%
   goto:eof
)
if [%xp_compatibility%]==["v140_xp"] (
   @echo Generating using "Visual Studio 2015" with WindowsXP support
	"%cmake_executable%" %~dp0/sources -T %xp_compatibility%
   goto:eof
)
if [%xp_compatibility%]==["v141_xp"] (
   @echo Generating using "Visual Studio 2017" with WindowsXP support
	"%cmake_executable%" %~dp0/sources -T %xp_compatibility%
   goto:eof
)
echo Error: invalid toolset selected
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