@echo off

::::::::::::::::::::::::::::
:START
::::::::::::::::::::::::::::
setlocal & pushd .

cd /D %~dp0/projects
cmake %~dp0/sources -DBUILD_FOR_WINXP=ON

if errorlevel 1 (
	pause
)
