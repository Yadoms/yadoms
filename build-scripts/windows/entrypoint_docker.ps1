$ScriptDir = Split-Path $script:MyInvocation.MyCommand.Path
cd $ScriptDir\..\..

Write-Host "Copy build config file"
cp $Env:YADOMS_DEPS_PATH\CMakeListsUserConfig.txt sources

Write-Host "Create makefile"
cmd.exe /c 'cmake_windows.cmd'

Write-Host "Build Yadoms"
cd projects
cmake --build . --config Release --target shared/all_unity

Write-Host "Build Yadoms package"
cp ..\config\yadoms.ini ..\builds\Release
cmake --build . --config Release --target package
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
Compress-Archive -Path .\* -DestinationPath ..\build\package\update-package-Windows.zip
cd ..
  