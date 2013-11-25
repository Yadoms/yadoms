@ECHO off

cmake %~dp0/sources --build %~dp0/projects

if errorlevel 1 (
	pause
)
