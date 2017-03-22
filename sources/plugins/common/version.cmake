# This macro extracts the Plugin version from "package.json"
file (STRINGS "${PACKAGE_JSON_FILE}" PLUGIN_VERSION REGEX "\"version\"[ ]*:[ ]*\"\(.*\)\"")
STRING(REGEX MATCH "\"version\"[ ]*:[ ]*\"\(.*\)\"" PLUGIN_VERSION ${PLUGIN_VERSION})
set (PLUGIN_VERSION ${CMAKE_MATCH_1})
STRING(REGEX MATCH "(0|[1-9]\\d*)\\.(0|[1-9]\\d*)\\.(0|[1-9]\\d*)(-[0-9a-zA-Z-\\.]*)?(\\+[0-9a-zA-Z-\\.]*)?" PLUGIN_VERSION ${PLUGIN_VERSION})

set (PLUGIN_VERSION_MAJOR ${CMAKE_MATCH_1})
set (PLUGIN_VERSION_MINOR ${CMAKE_MATCH_2})
set (PLUGIN_VERSION_PATCH ${CMAKE_MATCH_3})
set (PLUGIN_VERSION_PRERELEASE ${CMAKE_MATCH_4})
set (PLUGIN_VERSION_BUILD ${CMAKE_MATCH_5})
