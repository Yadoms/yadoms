$ScriptDir = Split-Path $script:MyInvocation.MyCommand.Path
cd $ScriptDir\..\..

Write-Host "Copy build config file"
cp $Env:YADOMS_DEPS_PATH\CMakeListsUserConfig.txt sources

Write-Host "Create makefile"
cmd.exe /c 'cmake_windows.cmd'

Write-Host "Build Yadoms"
cd projects
#because all_unity vcxproj is not always at same place, need to find it in project folder
#To do : cmake --build . --config Release --target all_unity
#The powershell ((dir -Filter all_unity.vcxproj -Recurse | %{$_.DirectoryName} | Resolve-Path -Relative).substring(2)+"\all_unity" -replace '\\','/')
#give the relative path of all_unity.vcxproj; removing extensions, removing first '.\' and replacing \ by /
cmake --build . --config Release --target ((dir -Filter all_unity.vcxproj -Recurse | %{$_.DirectoryName} | Resolve-Path -Relative).substring(2)+"\all_unity" -replace '\\','/')
if(! $?)
{
   "Buil failed"
   exit(1)
}

Write-Host "Build Yadoms package"
cp ..\config\yadoms.ini ..\builds\Release
cmake --build . --config Release --target package
if(! $?)
{
   "Package failed"
   exit(2)
}
cd ..

md package
move builds\Release package\package
cp update\scripts\update.cmd .\package\

$gitDate = git --git-dir=.git log -1 --format=%cI 
$yadomsVersion = (Select-String -Path sources\server\changelog.md -Pattern '^### (.*)$')[0] | Foreach-Object {$_.Matches} | Foreach-Object {$_.Groups[1].Value}
Write-Host "Generating package.zip for Yadoms $yadomsVersion release on $gitDate"

cd package
$content = Get-Content '..\distribution\package.json.in'
$content = $content -replace "__version__", $yadomsVersion
$content = $content -replace "__gitdate__", $gitDate
Out-File -InputObject $content -FilePath .\package.json -Encoding UTF8
Copy-Item ..\sources\server\changelog.md .\
Get-ChildItem * -Include *.ilk -Recurse | Remove-Item
Get-ChildItem * -Include yadoms.ini -Recurse | Remove-Item
Compress-Archive -Path .\* -DestinationPath ..\builds\package\update-package-Windows.zip
cd ..
  