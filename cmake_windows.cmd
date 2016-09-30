@echo off

::::::::::::::::::::::::::::
:START
::::::::::::::::::::::::::::
setlocal & pushd .

::Move to destination folder
cd /D %~dp0/projects
cmake %~dp0/sources
