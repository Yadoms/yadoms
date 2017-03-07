@echo off

::::::::::::::::::::::::::::
:START
::::::::::::::::::::::::::::
setlocal & pushd .

::Move to destination folder
cd /D %~dp0/projects


@echo Yadoms build for Windows
@echo ========================
@echo.
@echo To enable Windows XP compatiblity, please enter the platform toolset
@echo.
@echo v120_xp : platform toolset "Visual Studio 2013 - WindowsXP"
@echo v140_xp : platform toolset "Visual Studio 2015 - WindowsXP"
@echo Leave empty to disable Windows XP compatibility
@echo.
@echo.

set xp_compatibility=%1%

if [%xp_compatibility%]==[] (
	cmake %~dp0/sources
   goto end
)
if [%xp_compatibility%]==["v120_xp"] (
	cmake %~dp0/sources -T %input%
   goto end
)
if [%xp_compatibility%]==["v140_xp"] (
	cmake %~dp0/sources -T %input%
   goto end
)

:end