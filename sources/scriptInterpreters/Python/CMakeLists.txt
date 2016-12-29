# Check dependencies

set (PYTHON_REQUIRED_VERSION 2.7)
include(findPython.cmake)
  
if(PYTHONLIBS_FOUND)

   # include common script interpreters definitions
   include("../YadomsScriptInterpertersCommon.cmake")   
	  
   #########################################
   ## Messages static library
   #########################################
   add_subdirectory(messages)

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
      Messages.h
      Python.h
      Python.cpp
      PythonCommandLine.h
      PythonCommandLine.cpp
      PythonExecutable.h
      PythonExecutable.cpp
      ScriptFile.h
      ScriptFile.cpp
      ScriptProcess.h
      ScriptProcess.cpp
      
      scriptCaller.py
      scriptUtilities.py
   )
   
   #Append platform specific files
   IF(WIN32)
      set(PYTHON_SOURCES	
         ${PYTHON_SOURCES}
         platformSpecific/windows/PythonExecutablePath.h
         platformSpecific/windows/PythonExecutablePath.cpp
      )	
      set(PLATFORM_SPECIFIC_INCL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/platformSpecific/windows)

   ELSEIF(APPLE)
      set(PYTHON_SOURCES	
         ${PYTHON_SOURCES}
         platformSpecific/mac/PythonExecutablePath.h
         platformSpecific/mac/PythonExecutablePath.cpp
      )	
      set(PLATFORM_SPECIFIC_INCL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/platformSpecific/mac)

   ELSE()
      set(PYTHON_SOURCES	
         ${PYTHON_SOURCES}
         platformSpecific/linux/PythonExecutablePath.h
         platformSpecific/linux/PythonExecutablePath.cpp
      )	
      set(PLATFORM_SPECIFIC_INCL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/platformSpecific/linux)

   ENDIF()   
   
   
   #########################################
   ## Always leave these lines to configure script interpreter correctly
   ## even if PYTHON_INCLDIR or PYTHON_LINK are empty
   #########################################
   SCRIPT_INTERPRETER_SOURCES(python ${PYTHON_SOURCES})
   SCRIPT_INTERPRETER_INCLDIR(python ${PYTHON_INCLDIR} ${PLATFORM_SPECIFIC_INCL_DIR} ${PROTOBUF_INCLUDE_DIRS} ${messages_GENERATED_DIR})
   SCRIPT_INTERPRETER_LINK(python ${PYTHON_LINK} messages)

   #########################################
   ## Post-build commands
   ## Copy files to build directory
   #########################################
   SCRIPT_INTERPRETER_POST_BUILD_COPY_FILE(python package.json)
   SCRIPT_INTERPRETER_POST_BUILD_COPY_FILE(python icon.png)
   SCRIPT_INTERPRETER_POST_BUILD_COPY_FILE(python scriptCaller.py)
   SCRIPT_INTERPRETER_POST_BUILD_COPY_FILE(python scriptUtilities.py)
   SCRIPT_INTERPRETER_POST_BUILD_COPY_FILE(python template.py)
   SCRIPT_INTERPRETER_POST_BUILD_COPY_DIRECTORY(python locales)
	  
   #########################################
   ## Generate the associated yScriptApiwrapper
   #########################################
   add_subdirectory(yScriptApiWrapper)
   
else()
   message(WARNING "Python interpreter is not available. Python library is Mssing")
endif()