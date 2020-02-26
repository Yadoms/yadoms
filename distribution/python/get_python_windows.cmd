@echo off

::Move to destination folder
cd /D %~dp0/../../projects

::download Python embed win32 version
if not exist python.zip (
	powershell -Command "(New-Object Net.WebClient).DownloadFile('https://www.python.org/ftp/python/3.8.1/python-3.8.1-embed-win32.zip', 'python.zip')"
)

::extract it, and update python38._pth (needed to download pip)
if not exist .\python (
	powershell -Command "expand-archive -path 'python.zip' -destinationpath '.\python'"
	echo import site>> .\python\python38._pth
)

::download pip
if not exist .\python\git-pip.py (
	powershell -Command "(New-Object Net.WebClient).DownloadFile('https://bootstrap.pypa.io/get-pip.py', 'get-pip.py')"
	.\python\python.exe get-pip.py
)


:: Simply use pip : \Scripts\pip --install --user pandas
