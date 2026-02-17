@echo off

::::::::::::::::::::::::::::
:START
::::::::::::::::::::::::::::
setlocal & pushd .


@echo Yadoms build for Windows
@echo ========================
@echo.
@echo vs2019 : platform toolset "Visual Studio 16 2019"
@echo vs2022 : platform toolset "Visual Studio 17 2022"
@echo vs2026 : platform toolset "Visual Studio 18 2026"
@echo Leave empty to use default toolset
@echo.
@echo.

cmake --version

if "%1%" == "" (
   @echo Generating using default generator
	cmake -S sources -B projects -A Win32
   goto:eof
)
if "%1%" == "vs2019" (
   @echo Generating using "Visual Studio 16 2019"
	cmake -S sources -B projects -G "Visual Studio 16 2019" -A Win32
   goto:eof
)
if "%1%" == "vs2022" (
   @echo Generating using "Visual Studio 17 2022"
	cmake -S sources -B projects -G "Visual Studio 17 2022" -A Win32
   goto:eof
)
if "%1%" == "vs2026" (
   @echo Generating using "Visual Studio 18 2026"
	cmake -S sources -B projects -G "Visual Studio 18 2026" -A Win32
   goto:eof
)
echo Error: invalid toolset selected
goto:eof
