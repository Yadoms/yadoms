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
      ConsoleRedirector.h
      ConsoleRedirector.cpp
      IInitializer.h
      Initializer.h
      Initializer.cpp
      IScriptFile.h
      IScriptLoader.h
      ISubInterpreter.h
      Python.h
      Python.cpp
      PythonBorrowedObject.h
      PythonBorrowedObject.cpp
      PythonException.hpp
      PythonObject.h
      PythonObject.cpp
      PythonLibInclude.h
      Runner.h
      Runner.cpp
      ScriptFile.h
      ScriptFile.cpp
      ScriptLoader.h
      ScriptLoader.cpp
      SubInterpreter.h
      SubInterpreter.cpp
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
   set(PYTHON_LINK
      ${PYTHON_LIBRARIES}
   )

   #########################################
   ## Place here all additional compiler flags
   ## i.e. : for raspberry to link with wiringPi : use add_definition("-lwiringPi")
   #########################################
   if(PYTHON_USE_SOURCES)
      add_definitions(-DPYTHON_USE_SOURCES)
   endif()

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
   SCRIPT_INTERPRETER_POST_BUILD_COPY_FILE(python package.json)
   SCRIPT_INTERPRETER_POST_BUILD_COPY_FILE(python icon.png)
   SCRIPT_INTERPRETER_POST_BUILD_COPY_DIRECTORY(python locales)
   
   #########################################
   ## Add pre-build step to generate swigpyrun.h file
   #########################################
   add_custom_command(TARGET python PRE_BUILD
      COMMAND ${SWIG_EXECUTABLE} -python -external-runtime ${CMAKE_CURRENT_SOURCE_DIR}/swigpyrun.h)
   
   #########################################
   ## Generate the associated yScriptApi wrapper
   #########################################
   add_subdirectory(yScriptApiWrapper)
   
else()
   message(WARNING "Python interpreter is not available. Python library is Mssing")
endif()