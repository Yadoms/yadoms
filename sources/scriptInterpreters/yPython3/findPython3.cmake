# Python 3.x library Finder
# This module finds if Python is installed and determines where the
# include files and libraries are. It also determines what the name of
# the library is. This code sets the following variables:
#
#  PYTHONLIBS3_FOUND           - have the Python libs been found
#  PYTHON3_LIBRARIES           - path to the python library
#  PYTHON3_INCLUDE_DIRS        - path to where Python.h is found
#
# Use PYTHON3_ROOT if defined (from environment variables or CMakeListsUserConfig.txt), use it
# Otherwise, use the cmake-provided find_package
#

include (SelectLibraryConfigurations)

string(REPLACE "." "" PYTHON_REQUIRED_VERSION_NO_DOT ${PYTHON3_REQUIRED_VERSION})

# find_package seems to not work correctly under Windows, so use CMakeListsUserConfig.txt to get Python path
if (PYTHON3_ROOT)
   find_path(PYTHON3_INCLUDE_DIRS NAMES Python.h 
      PATHS 
         ${PYTHON3_ROOT}/include
         ${PYTHON3_ROOT}
      NO_DEFAULT_PATH
      )

      
   if(WIN32)
      if(PYTHON3_USE_SOURCES)
   
      find_library(PYTHON_LIBRARY_DEBUG
         NAMES python${PYTHON_REQUIRED_VERSION_NO_DOT}_d
         PATHS
         ${PYTHON3_ROOT}/PCbuild
         )
      find_library(PYTHON_LIBRARY_RELEASE
         NAMES python${PYTHON_REQUIRED_VERSION_NO_DOT}
         PATHS
         ${PYTHON3_ROOT}/PCbuild
         )
      select_library_configurations(PYTHON)
         
      else(PYTHON3_USE_SOURCES)

         find_library(PYTHON3_LIBRARIES NAMES python${PYTHON_REQUIRED_VERSION_NO_DOT}.lib PATHS ${PYTHON3_ROOT}/libs)
      
      endif(PYTHON3_USE_SOURCES)
      
   else(WIN32)
      if(PYTHON3_USE_SOURCES)
         #include also root path
         SET(PYTHON3_INCLUDE_DIRS
            ${PYTHON3_INCLUDE_DIRS}
            ${PYTHON3_ROOT}
         )
         
         #define libraroies and executable
         SET(PYTHON3_LIBRARIES ${PYTHON3_ROOT}/libpython${PYTHON3_REQUIRED_VERSION}.a)
         SET(PYTHON3_EXECUTABLE ${PYTHON3_ROOT}/python)
            
      else(PYTHON3_USE_SOURCES)
         find_library(PYTHON3_LIBRARIES 
			NAMES libpython${PYTHON3_REQUIRED_VERSION}.a 
			PATHS 
			${PYTHON3_ROOT}/libs
			${PYTHON3_ROOT}
			${PYTHON3_ADDITIONAL_LIB_PATH}
		  )
      endif(PYTHON3_USE_SOURCES)
   endif(WIN32)

   # handle the QUIETLY and REQUIRED arguments and set PYTHONLIBS3_FOUND to TRUE if 
   # all listed variables are TRUE
   include(FindPackageHandleStandardArgs)
   find_package_handle_standard_args(PythonLibs
                                     REQUIRED_VARS PYTHON3_LIBRARIES PYTHON3_INCLUDE_DIRS)
                                     
   set(PYTHONLIBS3_FOUND ${PYTHONLIBS_FOUND})
   unset(PYTHONLIBS_FOUND)
                                  
else()
   find_package(PythonLibs ${PYTHON3_REQUIRED_VERSION} REQUIRED)
endif()


