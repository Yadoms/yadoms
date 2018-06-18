
# This macro extracts a package version from its "changelog.md" file
MACRO(GET_PACKAGE_VERSION _changelogFile PACKAGE_VERSION)

   file (STRINGS "${_changelogFile}" NON_EMPTY_LINES REGEX "^.+$")
   list(GET NON_EMPTY_LINES 0 FIRST_NON_EMPTY_LINE)
   STRING(REGEX MATCH "### ((0|[1-9]+)\.(0|[1-9]+)\.(0|[1-9]+)(-[a-zA-Z]+\\.(0|[1-9]+))*)" PACKAGE_VERSION_LINE ${FIRST_NON_EMPTY_LINE})

   set (${PACKAGE_VERSION} ${CMAKE_MATCH_1})
   
ENDMACRO()


MACRO(EXTRACT_VERSION VERSION_IN MAJOR MINOR PATCH)

   STRING(REGEX MATCH "(0|[1-9]+)\\.(0|[1-9]+)\\.(0|[1-9]+)(-[0-9a-zA-Z-\\.]*)?(\\+[0-9a-zA-Z-\\.]*)?" VERSION_IN ${VERSION_IN})

   set (${MAJOR} ${CMAKE_MATCH_1})
   set (${MINOR} ${CMAKE_MATCH_2})
   set (${PATCH} ${CMAKE_MATCH_3})

ENDMACRO()


MACRO(EXTRACT_VERSION_2 VERSION_IN MAJOR MINOR PATCH PRERELEASE BUILD)

   STRING(REGEX MATCH "(0|[1-9]\\d*)\\.(0|[1-9]\\d*)\\.(0|[1-9]\\d*)(-[0-9a-zA-Z-\\.]*)?(\\+[0-9a-zA-Z-\\.]*)?" VERSION_IN ${VERSION_IN})

   set (${MAJOR} ${CMAKE_MATCH_1})
   set (${MINOR} ${CMAKE_MATCH_2})
   set (${PATCH} ${CMAKE_MATCH_3})
   set (${PRERELEASE} ${CMAKE_MATCH_4})
   set (${BUILD} ${CMAKE_MATCH_5})

ENDMACRO()
