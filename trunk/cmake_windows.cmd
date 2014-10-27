@echo off

::::::::::::::::::::::::::::
:START
::::::::::::::::::::::::::::
setlocal & pushd .

cd /D %~dp0/projects
cmake %~dp0/sources

if errorlevel 1 (
	pause
)
