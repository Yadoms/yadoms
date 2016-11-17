#This macro extract the Yadoms version from "Version.h"
file (STRINGS "${CMAKE_SOURCE_DIR}/server/Version.h" YADOMS_VERSION REGEX "[0-9]\.[0-9]\.[0-9]\.[0-9]")
STRING(REGEX MATCH "[0-9]\.[0-9]\.[0-9]\.[0-9]" YADOMS_VERSION ${YADOMS_VERSION})
