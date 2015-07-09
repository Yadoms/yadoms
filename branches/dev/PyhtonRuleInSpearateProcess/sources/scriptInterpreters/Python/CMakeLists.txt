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
      ContextAccessor.h
      ContextAccessor.cpp
      IContextAccessor.h
      IPythonExecutable.h
      IScriptFile.h
      Messages.hpp
      MessageQueueRemover.hpp
      Python.h
      Python.cpp
      PythonException.hpp
      PythonExecutable.h
      PythonExecutable.cpp
      Runner.h
      Runner.cpp
      ScriptFile.h
      ScriptFile.cpp
   )

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
   ## Generate the associated yScriptApi wrapper
   #########################################
   add_subdirectory(yScriptApiWrapper)
   
else()
   message(WARNING "Python interpreter is not available. Python library is Mssing")
endif()