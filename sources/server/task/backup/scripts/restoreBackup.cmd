@ECHO OFF
 
::/ NAME
::/  restoreBackup.cmd
::/ 
::/ SYNOPSIS
::/  Wait for exit of Yadoms, restore data and restart Yadoms
::/  
::/ 
::/ SYNTAX
::/  restoreBackup yadomsPid restoredDataDirectory timeout (default 60)
::/ 
::/ DETAILED DESCRIPTION
::/  The restoreBackup.cmd waits for the end of execution of yadoms, 
::/  restore the data and asks for a restart.
::/  There is a global timeout which avoid an infinity wait
 
SETLOCAL ENABLEDELAYEDEXPANSION
 
SET YadomsExecutable=yadoms.exe
SET BeforeRestoreDirectory=beforeBackupRestore
 
set CurrentScriptDir=%~dp0
 
::Parse  command line arguments
IF "%1"=="/?" (
    TYPE "%~f0" | findstr.exe /R "^::/"
    GOTO :END
)

SET yadomsPid=%~1
IF NOT DEFINED yadomsPid (
    ECHO %~n0 : Cannot bind argument to parameter 'yadomsPid' because it is empty.
    GOTO :END
)


SET restoredDataDirectory=%~2
IF NOT DEFINED restoredDataDirectory (
    ECHO %~n0 : Cannot bind argument to parameter 'restoredDataDirectory' because it is empty.
    GOTO :END
)

SET SecTimeout=%~3
IF NOT DEFINED SecTimeout (
    SET SecTimeout=60
)



::Wait for process to exit
set secWait=1
SET programPid=%yadomsPid%
:while1
if not %programPid%=="" (
	echo Wait for %YadomsExecutable% to exit...
	timeout /t 1 /nobreak > nul
	SET programPid=""
	FOR /F "tokens=2 skip=3" %%i IN (
		'tasklist.exe /FI "PID eq %programPid%"'
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

echo Move current data ...
if exist %BeforeRestoreDirectory% (
   rmdir /S /Q %BeforeRestoreDirectory%
   )
mkdir %BeforeRestoreDirectory%
:: Database
if exist yadoms.db3 (
   move /Y yadoms.db3 %BeforeRestoreDirectory%
   )
:: Yadoms.ini
if exist yadoms.ini (
   move /Y yadoms.ini %BeforeRestoreDirectory%
   )
:: Scripts
if exist scripts (
   move /Y scripts %BeforeRestoreDirectory%
   )
:: Plugin data
if exist data (
   move /Y data %BeforeRestoreDirectory%
   )

echo Restoring data ...
:: Merge all the backup content into Yadoms current directory
for /d %%i in ("%restoredDataDirectory%\*.*") do move /Y "%%i" "%CurrentScriptDir%"
for %%i in ("%restoredDataDirectory%\*.*") do move /Y "%%i" "%CurrentScriptDir%"

echo Cleanup ...
rmdir /S /Q %restoredDataDirectory%


echo Data restored successfully

timeout /t 5 /nobreak > nul

echo Restarting Yadoms ...

explorer %YadomsPath%%YadomsExecutable%

goto :END


:END
