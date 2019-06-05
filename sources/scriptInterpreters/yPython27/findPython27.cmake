# Python 2.7.x library Finder
# This module finds if Python is installed and determines where the
# include files and libraries are. It also determines what the name of
# the library is. This code sets the following variables:
#
#  PYTHONLIBS27_FOUND           - have the Python libs been found
#  PYTHON27_LIBRARIES           - path to the python library
#  PYTHON27_INCLUDE_DIRS        - path to where Python.h is found
#
# Use PYTHON27_ROOT if defined (from environment variables or CMakeListsUserConfig.txt), use it
# Otherwise, use the cmake-provided find_package
#

include (SelectLibraryConfigurations)

string(REPLACE "." "" PYTHON_REQUIRED_VERSION_NO_DOT ${PYTHON27_REQUIRED_VERSION})

# find_package seems to not work correctly under Windows, so use CMakeListsUserConfig.txt to get Python path
if (PYTHON27_ROOT)
   find_path(PYTHON27_INCLUDE_DIRS NAMES Python.h 
      PATHS 
         ${PYTHON27_ROOT}/include
         ${PYTHON27_ROOT}
      NO_DEFAULT_PATH
      )

      
   if(WIN32)
      if(PYTHON27_USE_SOURCES)
   
      find_library(PYTHON_LIBRARY_DEBUG
         NAMES python${PYTHON_REQUIRED_VERSION_NO_DOT}_d
         PATHS
         ${PYTHON27_ROOT}/PCbuild
         )
      find_library(PYTHON_LIBRARY_RELEASE
         NAMES python${PYTHON_REQUIRED_VERSION_NO_DOT}
         PATHS
         ${PYTHON27_ROOT}/PCbuild
         )
      select_library_configurations(PYTHON)
         
      else(PYTHON27_USE_SOURCES)

         find_library(PYTHON27_LIBRARIES NAMES python${PYTHON_REQUIRED_VERSION_NO_DOT}.lib PATHS ${PYTHON27_ROOT}/libs)
      
      endif(PYTHON27_USE_SOURCES)
      
   else(WIN32)
      if(PYTHON27_USE_SOURCES)
         #include also root path
         SET(PYTHON27_INCLUDE_DIRS
            ${PYTHON27_INCLUDE_DIRS}
            ${PYTHON27_ROOT}
         )
         
         #define libraroies and executable
         SET(PYTHON27_LIBRARIES ${PYTHON27_ROOT}/libpython${PYTHON27_REQUIRED_VERSION}.a)
         SET(PYTHON27_EXECUTABLE ${PYTHON27_ROOT}/python)
            
      else(PYTHON27_USE_SOURCES)
         find_library(PYTHON27_LIBRARIES NAMES libpython${PYTHON27_REQUIRED_VERSION}.a PATHS ${PYTHON27_ROOT}/libs)
      endif(PYTHON27_USE_SOURCES)
   endif(WIN32)

   # handle the QUIETLY and REQUIRED arguments and set PYTHONLIBS27_FOUND to TRUE if 
   # all listed variables are TRUE
   include(FindPackageHandleStandardArgs)
   find_package_handle_standard_args(PythonLibs
                                     REQUIRED_VARS PYTHON27_LIBRARIES PYTHON27_INCLUDE_DIRS)
                                     
   set(PYTHONLIBS27_FOUND ${PYTHONLIBS_FOUND})
   unset(PYTHONLIBS_FOUND)                            
                                  
else()
   find_package(PythonLibs ${PYTHON27_REQUIRED_VERSION} REQUIRED)
endif()


