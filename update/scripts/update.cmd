@ECHO OFF
 
::/ NAME
::/  update.cmd
::/ 
::/ SYNOPSIS
::/  Wait for exit of Yadoms, update the software and restart Yadoms
::/  
::/ 
::/ SYNTAX
::/  update YadomsPath YadomsCurrentPid timeout(default 60)
::/ 
::/ DETAILED DESCRIPTION
::/  The update.cmd wait for the end of execution of yadoms, 
::/  update the yadoms application and ask for a restart.
::/  There is a global timeout which avoid an infinity wait
 
SETLOCAL ENABLEDELAYEDEXPANSION
 
SET YadomsExecutable=yadoms.exe
 
set CurrentScriptDir=%~dp0

SET YadomsCurrentPid=%~1
SET YadomsPath=%~2
 
::Parse  command line arguments
IF "%1"=="/?" (
    TYPE "%~f0" | findstr.exe /R "^::/"
    GOTO :END
)
 
IF NOT DEFINED YadomsPath (
    ECHO %~n0 : Cannot bind argument to parameter 'YadomsPath' because it is empty.
    GOTO :END
)
IF NOT DEFINED YadomsCurrentPid (
    ECHO %~n0 : Cannot bind argument to parameter 'YadomsCurrentPid' because it is empty.
    GOTO :END
)

SET SecTimeout=%~2
IF NOT DEFINED SecTimeout (
    SET SecTimeout=60
)

::Check if program is running
SET programPid=""
FOR /F "tokens=2 skip=3" %%i IN (
    'tasklist.exe /FI "IMAGENAME eq %YadomsExecutable%"'
) DO (
SET programPid=%%~i
)

::If not running then continue script
if %programPid%=="" (
	echo %YadomsExecutable% is not running
	goto :PROCESSIFENDED
)

::Wait for process to exit
set secWait=1
:while1
if not %programPid%=="" (
	echo Wait for %YadomsExecutable% to exit...
	timeout /t 1 /nobreak > nul
	SET programPid=""
	FOR /F "tokens=2 skip=3" %%i IN (
		'tasklist.exe /FI "IMAGENAME eq %YadomsExecutable%"'
	) DO (
	SET programPid=%%~i
	)

	if %secWait% GEQ %SecTimeout% (
		echo %YadomsExecutable% is still running after %secWait% seconds
		echo Could not apply script
		goto :END
	)
	
	set /A secWait+=1 

    goto :while1
)
echo %YadomsExecutable% ends

:PROCESSIFENDED

@ECHO ON

echo Updating Yadoms ...

::INSERT UPDATE COMMAND HERE
copy "%CurrentScriptDir%package\*.*" "%YadomsPath%"

echo Yadoms updated successfully

timeout /t 5 /nobreak > nul

echo Restarting Yadoms ...

explorer %YadomsPath%%YadomsExecutable%

goto :END


:END
