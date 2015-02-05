# Swig library Finder
# This module finds if Swig is installed and determines where the
# needed files are. This code sets the following variables:
#
#  SWIG_FOUND              - set to true if SWIG is found
#  SWIG_DIR                - the directory where swig is installed
#  SWIG_EXECUTABLE         - the path to the swig executable
#  SWIG_VERSION            - the version number of the swig executable
#
# Use SWIG_ROOT if defined (from environment variables or CMakeListsUserConfig.txt), use it
# Otherwise, use the path
#

include (CMakeListsUserConfig.txt OPTIONAL)
include (SelectLibraryConfigurations)

if (SWIG_ROOT)
   if(WIN32)
      set(SWIG_EXECUTABLE "${SWIG_ROOT}/swig.exe")
   else()
      set(SWIG_EXECUTABLE "${SWIG_ROOT}/swig")
   endif()
endif()

FIND_PACKAGE(SWIG)


