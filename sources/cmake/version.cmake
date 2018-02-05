
# This macro extracts the Yadoms version from "changelog.md"

file (STRINGS "${CMAKE_SOURCE_DIR}/server/changelog.md" NON_EMPTY_LINES REGEX "^.+$")
list(GET NON_EMPTY_LINES 0 FIRST_NON_EMPTY_LINE)
STRING(REGEX MATCH "### ((0|[1-9]+)\.(0|[1-9]+)\.(0|[1-9]+)(-[a-zA-Z]+\\.(0|[1-9]+))*)" YADOMS_VERSION_LINE ${FIRST_NON_EMPTY_LINE})

set (YADOMS_VERSION ${CMAKE_MATCH_1})


STRING(REGEX MATCH "(0|[1-9]+)\\.(0|[1-9]+)\\.(0|[1-9]+)(-[0-9a-zA-Z-\\.]*)?(\\+[0-9a-zA-Z-\\.]*)?" YADOMS_VERSION ${YADOMS_VERSION})

set (YADOMS_VERSION_MAJOR ${CMAKE_MATCH_1})
set (YADOMS_VERSION_MINOR ${CMAKE_MATCH_2})
set (YADOMS_VERSION_PATCH ${CMAKE_MATCH_3})

