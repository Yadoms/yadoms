
# This macro extracts the Yadoms version from "Version.h"

file (STRINGS "${CMAKE_SOURCE_DIR}/server/Version.h" YADOMS_VERSION REGEX "YadomsVersion\(.*\)")
STRING(REGEX MATCH "YadomsVersion\\(\"(.*)\"\\)" YADOMS_VERSION ${YADOMS_VERSION})

set (YADOMS_VERSION ${CMAKE_MATCH_1})


STRING(REGEX MATCH "(0|[1-9]\\d*)\\.(0|[1-9]\\d*)\\.(0|[1-9]\\d*)(-[0-9a-zA-Z-\\.]*)?(\\+[0-9a-zA-Z-\\.]*)?" YADOMS_VERSION ${YADOMS_VERSION})

set (YADOMS_VERSION_MAJOR ${CMAKE_MATCH_1})
set (YADOMS_VERSION_MINOR ${CMAKE_MATCH_2})
set (YADOMS_VERSION_PATCH ${CMAKE_MATCH_3})
set (YADOMS_VERSION_PRERELEASE ${CMAKE_MATCH_4})
set (YADOMS_VERSION_BUILD ${CMAKE_MATCH_5})
