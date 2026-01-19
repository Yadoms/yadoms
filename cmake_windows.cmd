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
@echo Leave empty to use default toolset
@echo.
@echo.

cmake --version

if "%1%" == "" (
   @echo Generating using default generator
	cmake -S sources -B projects -DYADOMS_BINARY_DIR=bin -A Win32
   goto:eof
)
if "%1%" == "v142" (
   @echo Generating using "Visual Studio 2019"
	cmake -S sources -B projects -G "Visual Studio 17 2019" -DYADOMS_BINARY_DIR=bin -A Win32
   goto:eof
)
if "%1%" == "v143" (
   @echo Generating using "Visual Studio 2022"
	cmake -S sources -B projects -G "Visual Studio 17 2022" -DYADOMS_BINARY_DIR=bin -A Win32
   goto:eof
)
echo Error: invalid toolset selected
goto:eof
