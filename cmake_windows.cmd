@echo off

::::::::::::::::::::::::::::
:START
::::::::::::::::::::::::::::
setlocal & pushd .

::Move to destination folder
cd /D %~dp0/projects

::Ask to generate project files with WindowsXP compatibility
choice /C:xazertyuiopqsdfghjklmwcvbn0123456789 /N /M "Hit X to generate solution compatible with WindowsXP. Any other key or wait 5 seconds to use classic generation" /T 5 /D M
if errorlevel 1 (
	::generate for WindowsXP
	cmake %~dp0/sources -DBUILD_FOR_WINXP=ON
) else (
	::use classic generation
	cmake %~dp0/sources
)

::After generating project files, ask the user to start the build (without opening VisualStudio)
if errorlevel 0 (
	choice /C:bazertyuiopqsdfghjklmwxcvn0123456789 /N /M "Hit B to start building. Any other key or wait 5 seconds to exit" /T 5 /D M
	if errorlevel 1 (
		cmake --build . -- /verbosity:minimal
		if errorlevel 1 (
			pause
		)
	)
) else (
	if errorlevel 1 (
		pause
	)
)
