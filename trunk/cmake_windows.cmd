@ECHO off
cd projects
cmake ..\sources

if errorlevel 1 (
	pause
)
