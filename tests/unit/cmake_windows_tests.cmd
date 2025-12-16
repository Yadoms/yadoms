@echo off

::::::::::::::::::::::::::::
:START
::::::::::::::::::::::::::::
setlocal & pushd .


@echo Yadoms build for Windows
@echo ========================
@echo.
@echo v142 : platform toolset "Visual Studio 2019"
@echo v143 : platform toolset "Visual Studio 2022"
@echo v144 : platform toolset "Visual Studio 2026"
@echo.
@echo Leave empty to use default toolset
@echo.
@echo.

set platformToolset=%1%

call:getCMakeExecutable cmake_executable
"%cmake_executable%" --version

::Move to destination folder
cd /D %~dp0/projects

if "%platformToolset%" == "" (
   @echo Using default generator
	"%cmake_executable%" %~dp0/sources -A Win32
   goto:eof
)
if "%platformToolset%" == "v142" (
   @echo Generating using "Visual Studio 2019"
	"%cmake_executable%" -G "Visual Studio 16 2019" %~dp0/sources -A Win32
   goto:eof
)
if "%platformToolset%" == "v143" (
   @echo Generating using "Visual Studio 2022"
	"%cmake_executable%" -G "Visual Studio 17 2022" %~dp0/sources -A Win32
   goto:eof
)
if "%platformToolset%" == "v144" (
   @echo Generating using "Visual Studio 2026"
	"%cmake_executable%" -G "Visual Studio 18 2026" %~dp0/sources -A Win32
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