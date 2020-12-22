$ScriptDir = Split-Path $script:MyInvocation.MyCommand.Path
cd $ScriptDir\..\..
cd tests/unit

Write-Host "Copy build config file"
cp $Env:YADOMS_DEPS_PATH\CMakeListsUserConfig.txt sources

Write-Host "Create solution/project files"
cmd.exe /c 'cmake_windows_tests.cmd'

Write-Host "Build Yadoms UnitTests"
cd projects
#because all_unity vcxproj is not always at same place, need to find it in project folder
#To do : cmake --build . --config Release --target all_unity
#The powershell ((dir -Filter all_unity.vcxproj -Recurse | %{$_.DirectoryName} | Resolve-Path -Relative).substring(2)+"\all_unity" -replace '\\','/')
#give the relative path of all_unity.vcxproj; removing extensions, removing first '.\' and replacing \ by /
cmake --build . --config Release --target yadomsTests
if(! $?)
{
   "Buil failed"
   exit(1)
}

