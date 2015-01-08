# Python library Finder
# This module finds if Python is installed and determines where the
# include files and libraries are. It also determines what the name of
# the library is. This code sets the following variables:
#
#  PYTHONLIBS_FOUND           - have the Python libs been found
#  PYTHON_LIBRARIES           - path to the python library
#  PYTHON_INCLUDE_DIRS        - path to where Python.h is found
#
# Use PYTHON_ROOT is defined (from environment variables or CMakeListsUserConfig.txt), use it
# Otherwise, use the cmake-provided find_package
#

include (CMakeListsUserConfig.txt OPTIONAL)

string(REPLACE "." "" PYTHON_REQUIRED_VERSION_NO_DOT ${PYTHON_REQUIRED_VERSION})

# find_package seems to not work correctly under Windows, so use CMakeListsUserConfig.txt to get Python path
if (PYTHON_ROOT)

   find_path(PYTHON_INCLUDE_DIRS NAMES python.h PATHS ${PYTHON_ROOT}/include)
   find_library(PYTHON_LIBRARIES NAMES python${PYTHON_REQUIRED_VERSION_NO_DOT}.lib PATHS ${PYTHON_ROOT}/libs)

   # handle the QUIETLY and REQUIRED arguments and set PYTHONLIBS_FOUND to TRUE if 
   # all listed variables are TRUE
   include(FindPackageHandleStandardArgs)
   find_package_handle_standard_args(PythonLibs
                                     REQUIRED_VARS PYTHON_LIBRARIES PYTHON_INCLUDE_DIRS
                                     VERSION_VAR PYTHONLIBS_VERSION_STRING)
                                  
else()

   find_package(PythonLibs ${PYTHON_REQUIRED_VERSION})

endif()


