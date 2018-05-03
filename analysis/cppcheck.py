import os
   
os.chdir('../sources')

print 'Find missing includes...'
   
os.system("cppcheck --check-config --enable=all . --std=c++11 --xml --suppressions-list=cppcheck.suppressions -i\"external-libs\" -i\"plugins/ZWave/OpenZWave\" -i\"plugins/Piface2/libmcp23s17\" -i\"plugins/Piface2/libpifacedigital\" -I\"shared\" -I\"shared/windows\" -I\"shared/linux\" -I\"shared/mac\" -I\"server\" -I\"plugins/OneWire\" -I\"plugins/SystemInformation/CommonLinux\" -I\"plugins/SystemInformation/Linux\" -I\"plugins/SystemInformation/Mac\" -I\"plugins/SystemInformation/Windows\" -I\"plugins/SystemInformation/RaspberryPI\" -I\"scriptInterpreters/yPython27/platformSpecific/linux\" -I\"scriptInterpreters/yPython27/platformSpecific/mac\" -I\"scriptInterpreters/yPython27/platformSpecific/windows\" -I\"server/tools\" -I\"server/tools/windows\" -I\"server/tools/mac\" -I\"server/tools/linux\" -I\"external-libs/SQLite/sqlite-amalgamation-3130000\">../projects/cppcheck_out.txt 2> ../projects/cppcheck_missing_includes.xml")

print 'Normal check...'
os.system("cppcheck --enable=all . --std=c++11 --xml -v -i\"external-libs\" -i\"plugins/ZWave/OpenZWave\" -i\"plugins/Piface2/libmcp23s17\" -i\"plugins/Piface2/libpifacedigital\" --suppress=missingIncludeSystem --suppress=unreadVariable:\"server/database/sqlite/SQLiteDatabaseTables.h\" --suppress=unreadVariable:\"server/database/sqlite/SQLiteSystemTables.h\" --suppress=*:\"scriptInterpreters/yPython27/yScriptApiWrapper/swigpyrun.h\" --suppress=noExplicitConstructor:\"shared/shared/enumeration/EnumHelpers.hpp\" -I\"shared\" -I\"shared/windows\" -I\"shared/linux\" -I\"shared/mac\" -I\"server\" -I\"plugins/OneWire\" -I\"plugins/SystemInformation/CommonLinux\" -I\"plugins/SystemInformation/Linux\" -I\"plugins/SystemInformation/Mac\" -I\"plugins/SystemInformation/Windows\" -I\"plugins/SystemInformation/RaspberryPI\" -I\"scriptInterpreters/yPython27/platformSpecific/linux\" -I\"scriptInterpreters/yPython27/platformSpecific/mac\" -I\"scriptInterpreters/yPython27/platformSpecific/windows\" -I\"server/tools\" -I\"server/tools/windows\" -I\"server/tools/mac\" -I\"server/tools/linux\" >../projects/cppcheck_out.txt 2> ../projects/cppcheck_result.xml")

print 'Generate HMTL report...'
os.system("python ../analysis/htmlreport/cppcheck-htmlreport --source-encoding=\"iso8859-1\" --title=\"Yadoms\" --source-dir=. --report-dir=../projects/cppcheck_results --file=../projects/cppcheck_result.xml")

print 'Finished'

os.chdir('../analysis')
