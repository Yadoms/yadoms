REM TODO : review all suppress as several Cppcheck warnings are coming from a Cppcheck bug (http://trac.cppcheck.net/ticket/7388),
REM in the "pragma once" managment. Fix is expected on Cppcheck v1.73.

echo off

cd ../sources

echo Find missing includes...
cppcheck --check-config --enable=all . --std=c++03 --xml -i"external-libs" -i"plugins/ZWave/OpenZWave" --suppress=missingIncludeSystem --suppress=unusedFunction --suppress=unreadVariable:"server/database/sqlite/SQLiteDatabaseTables.h" --suppress=*:"scriptInterpreters/Python/yScriptApiWrapper/swigpyrun.h" -I"shared" -I"shared/windows" -I"shared/linux" -I"shared/mac" -I"server" -I"plugins/OneWire" -I"plugins/SystemInformation/CommonLinux" -I"plugins/SystemInformation/Linux" -I"plugins/SystemInformation/Mac" -I"plugins/SystemInformation/Windows" -I"plugins/SystemInformation/RaspberryPI" -I"scriptInterpreters/Python/platformSpecific/linux" -I"scriptInterpreters/Python/platformSpecific/mac" -I"scriptInterpreters/Python/platformSpecific/windows" -I"server/tools" -I"server/tools/windows" -I"server/tools/mac" -I"server/tools/linux" >../projects/out.txt 2> ../projects/cppcheck_missing_includes.xml

echo Normal check...
cppcheck --enable=all . --std=c++03 --xml -v -i"external-libs" -i"plugins/ZWave/OpenZWave" --suppress=missingIncludeSystem --suppress=unusedFunction --suppress=unreadVariable:"server/database/sqlite/SQLiteDatabaseTables.h" --suppress=unreadVariable:"server/database/sqlite/SQLiteSystemTables.h" --suppress=*:"scriptInterpreters/Python/yScriptApiWrapper/swigpyrun.h" --suppress=noExplicitConstructor:"shared/shared/enumeration/EnumHelpers.hpp" -I"shared" -I"shared/windows" -I"shared/linux" -I"shared/mac" -I"server" -I"plugins/OneWire" -I"plugins/SystemInformation/CommonLinux" -I"plugins/SystemInformation/Linux" -I"plugins/SystemInformation/Mac" -I"plugins/SystemInformation/Windows" -I"plugins/SystemInformation/RaspberryPI" -I"scriptInterpreters/Python/platformSpecific/linux" -I"scriptInterpreters/Python/platformSpecific/mac" -I"scriptInterpreters/Python/platformSpecific/windows" -I"server/tools" -I"server/tools/windows" -I"server/tools/mac" -I"server/tools/linux" >../projects/out.txt 2> ../projects/cppcheck_result.xml

echo Finished

cd ../analysis