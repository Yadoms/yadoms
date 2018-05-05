import os

# os.chdir('../sources')

print 'Find missing includes...'

SOURCE_PATH = '../sources'

EXCLUDE_DIRECTORIES = \
    '-i"external-libs" ' + \
    '-i"plugins/ZWave/OpenZWave" ' + \
    '-i"plugins/Piface2/libmcp23s17" ' + \
    '-i"plugins/Piface2/libpifacedigital" ' + \
    '-i"plugins/TeleInfo/ftdi" ' + \
    '-i"plugins/SomfySituo/hardware" '

INCLUDE_DIRECTORIES = \
    '-I' + SOURCE_PATH + '"/shared" ' + \
    '-I' + SOURCE_PATH + '"/shared/windows" ' + \
    '-I' + SOURCE_PATH + '"/shared/linux" ' + \
    '-I' + SOURCE_PATH + '"/shared/mac" ' + \
    '-I' + SOURCE_PATH + '"/shared/shared" ' + \
    '-I' + SOURCE_PATH + '"/plugins/EnOcean/profiles" ' + \
    '-I' + SOURCE_PATH + '"/plugins/EnOcean/specificHistorizers" ' + \
    '-I' + SOURCE_PATH + '"/plugins/OneWire" ' + \
    '-I' + SOURCE_PATH + '"/plugins/SystemInformation/CommonLinux" ' + \
    '-I' + SOURCE_PATH + '"/plugins/SystemInformation/Linux" ' + \
    '-I' + SOURCE_PATH + '"/plugins/SystemInformation/Mac" ' + \
    '-I' + SOURCE_PATH + '"/plugins/SystemInformation/Windows" ' + \
    '-I' + SOURCE_PATH + '"/plugins/SystemInformation/RaspberryPI" ' + \
    '-I' + SOURCE_PATH + '"/plugins/SystemInformation/Helpers" ' + \
    '-I' + SOURCE_PATH + '"/plugins/Piface2/libmcp23s17" ' + \
    '-I' + SOURCE_PATH + '"/plugins/Piface2/libpifacedigital" ' + \
    '-I' + SOURCE_PATH + '"/plugins/TeleInfo/Windows" ' + \
    '-I' + SOURCE_PATH + '"/plugins/TeleInfo/Linux" ' + \
    '-I' + SOURCE_PATH + '"/scriptInterpreters/yPython27/platformSpecific/linux" ' + \
    '-I' + SOURCE_PATH + '"/scriptInterpreters/yPython27/platformSpecific/mac" ' + \
    '-I' + SOURCE_PATH + '"/scriptInterpreters/yPython27/platformSpecific/windows" ' + \
    '-I' + SOURCE_PATH + '"/server" ' + \
    '-I' + SOURCE_PATH + '"/server/tools" ' + \
    '-I' + SOURCE_PATH + '"/server/tools/windows" ' + \
    '-I' + SOURCE_PATH + '"/server/tools/mac" ' + \
    '-I' + SOURCE_PATH + '"/server/tools/linux" ' + \
    '-I' + SOURCE_PATH + '"/external-libs/SQLite/sqlite-amalgamation-3230000" ' + \
    '-I' + SOURCE_PATH + '"/script_cpp_api" ' + \
    '-I"../projects/plugins/EnOcean/generated" ' + \
    '-I"../projects/plugins/EnOcean/generated/profiles" ' + \
    '-I"../projects/plugin_IPC" ' + \
    '-I"../projects/interpreter_IPC" ' + \
    '-I"../projects/script_IPC" '

EXCLUDE_CONFIGURATIONS = \
    '-USQLITE_INT64_TYPE '

os.system('cppcheck --check-config --enable=all ' +
          SOURCE_PATH +
          ' --std=c++11 --xml' +
          ' --suppressions-list=cppcheck.suppressions ' +
          ' --suppress=missingIncludeSystem ' +
          EXCLUDE_DIRECTORIES +
          INCLUDE_DIRECTORIES +
          EXCLUDE_CONFIGURATIONS +
          ' > ../projects/cppcheck_out.txt 2>../projects/cppcheck_missing_includes.xml')

print 'Normal check...'
os.system('cppcheck --enable=all ' +
          SOURCE_PATH +
          ' --std=c++11 --xml -v --suppressions-list=cppcheck.suppressions --suppress=missingIncludeSystem --suppress=unreadVariable:\"server/database/sqlite/SQLiteDatabaseTables.h\" --suppress=unreadVariable:\"server/database/sqlite/SQLiteSystemTables.h\" --suppress=*:\"scriptInterpreters/yPython27/yScriptApiWrapper/swigpyrun.h\" --suppress=noExplicitConstructor:\"shared/shared/enumeration/EnumHelpers.hpp\" ' +
          EXCLUDE_DIRECTORIES +
          INCLUDE_DIRECTORIES +
          EXCLUDE_CONFIGURATIONS +
          ' >../projects/cppcheck_out.txt 2>../projects/cppcheck_result.xml')

print 'Generate HMTL report...'
os.system("python ../analysis/htmlreport/cppcheck-htmlreport --source-encoding=\"iso8859-1\" --title=\"Yadoms\" --source-dir=. --report-dir=../projects/cppcheck_results --file=../projects/cppcheck_result.xml")

print 'Finished'

os.chdir('../analysis')
