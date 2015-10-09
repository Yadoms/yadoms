# Libquickmail library Finder
# This module defines
#  LIBQUICKMAIL_FOUND, true if Libquickmail was found
#  LIBQUICKMAIL_INCLUDE_DIRS, where to find Libquickmail.h
#  LIBQUICKMAIL_LIBRARIES, where to find libraries needed to build with Libquickmail

include (SelectLibraryConfigurations)

if(LIBQUICKMAIL_INCLUDE_DIRS AND LIBQUICKMAIL_LIBRARIES)
    # Already in cache
    set(LIBQUICKMAIL_FIND_QUIETLY TRUE)
endif(LIBQUICKMAIL_INCLUDE_DIRS AND LIBQUICKMAIL_LIBRARIES)

# Use LIBQUICKMAIL_ROOT} defined in CMakeListsUserConfig.txt
set(LIBQUICKMAIL_INCLUDE_DIRS ${LIBQUICKMAIL_ROOT})
set(LIBQUICKMAIL_LIBRARY_DIRS ${LIBQUICKMAIL_ROOT}/.libs)
set(LIBQUICKMAIL_LIBRARIES libquickmail.so)

find_library(LIBQUICKMAIL_LIBRARIES NAMES ${LIBQUICKMAIL_LIBRARIES} PATHS ${LIBQUICKMAIL_LIBRARY_DIRS} NO_DEFAULT_PATH)

find_path(LIBQUICKMAIL_INCLUDE_DIRS NAMES quickmail.h
   PATHS
   ${LIBQUICKMAIL_INCLUDE_DIRS})

# handle the QUIETLY and REQUIRED arguments and set LIBQUICKMAIL_FOUND to TRUE if 
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LIBQUICKMAIL DEFAULT_MSG LIBQUICKMAIL_INCLUDE_DIRS LIBQUICKMAIL_LIBRARIES)
