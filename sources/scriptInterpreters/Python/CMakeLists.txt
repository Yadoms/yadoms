# Check dependencies

set (PYTHON_REQUIRED_VERSION 2.7)
include(findPython.cmake)
  
if(PYTHONLIBS_FOUND)

   # include common script interpreters definitions
   include("../YadomsScriptInterpertersCommon.cmake")

   #########################################
   ## Place here all the source files
   #########################################
   set(PYTHON_SOURCES
      stdafx.h
      stdafx.cpp
      IInitializer.h
      Initializer.h
      Initializer.cpp
      Python.h
      Python.cpp
      PythonLibInclude.h
   )

   #########################################
   ## Place here all additional include directories
   ## (by default yadoms-shared and boost are included)
   #########################################
   set(PYTHON_INCLDIR
      ${PYTHON_INCLUDE_DIRS}
   )

   #########################################
   ## Place here all additional libraries to link with
   ## (by default yadoms-shared and boost are included)
   #########################################
   message("PYTHON_LIBRARIES=${PYTHON_LIBRARIES}")
   set(PYTHON_LINK
      ${PYTHON_LIBRARIES}
   )

   #########################################
   ## Place here all additional compiler flags
   ## i.e. : for raspberry to link with wiringPi : use add_definition("-lwiringPi")
   #########################################
   ##add_definitions("-lwiringPi")

   #########################################
   ## Always leave these lines to configure script interpreter correctly
   ## even if PYTHON_INCLDIR or PYTHON_LINK are empty
   #########################################
   SCRIPT_INTERPRETER_SOURCES(python ${PYTHON_SOURCES})
   SCRIPT_INTERPRETER_INCLDIR(python ${PYTHON_INCLDIR})
   SCRIPT_INTERPRETER_LINK(python ${PYTHON_LINK})

   #########################################
   ## Post-build commands
   ## Copy files to build directory
   #########################################
   #SCRIPT_INTERPRETER_POST_BUILD_COPY_FILE(python fileToCopy)
   #SCRIPT_INTERPRETER_POST_BUILD_COPY_DIRECTORY(python directoryToCopy)
   
else()
   message(WARNING "Python interpreter is not available. Python library is Mssing")
endif()