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
   set(PYTHON27_SOURCES
      stdafx.h
      stdafx.cpp
      ContextAccessor.h
      ContextAccessor.cpp
      IContextAccessor.h
      IPythonExecutable.h
      IScriptFile.h
      Messages.h
      Python27.h
      Python27.cpp
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
      set(PYTHON27_SOURCES	
         ${PYTHON27_SOURCES}
         platformSpecific/windows/PythonExecutablePath.h
         platformSpecific/windows/PythonExecutablePath.cpp
      )	
      set(PLATFORM_SPECIFIC_INCL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/platformSpecific/windows)

   ELSEIF(APPLE)
      set(PYTHON27_SOURCES	
         ${PYTHON27_SOURCES}
         platformSpecific/mac/PythonExecutablePath.h
         platformSpecific/mac/PythonExecutablePath.cpp
      )	
      set(PLATFORM_SPECIFIC_INCL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/platformSpecific/mac)

   ELSE()
      set(PYTHON27_SOURCES	
         ${PYTHON27_SOURCES}
         platformSpecific/linux/PythonExecutablePath.h
         platformSpecific/linux/PythonExecutablePath.cpp
      )	
      set(PLATFORM_SPECIFIC_INCL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/platformSpecific/linux)

   ENDIF()   
   
   
   #########################################
   ## Always leave these lines to configure script interpreter correctly
   ## even if PYTHON27_INCLDIR or PYTHON27_LINK are empty
   #########################################
   SCRIPT_INTERPRETER_SOURCES(python27 ${PYTHON27_SOURCES})
   SCRIPT_INTERPRETER_INCLDIR(python27 ${PYTHON27_INCLDIR} ${PLATFORM_SPECIFIC_INCL_DIR} ${PROTOBUF_INCLUDE_DIRS} ${messages_GENERATED_DIR})
   SCRIPT_INTERPRETER_LINK(python27 ${PYTHON27_LINK} messages)

   #########################################
   ## Post-build commands
   ## Copy files to build directory
   #########################################
   SCRIPT_INTERPRETER_POST_BUILD_COPY_FILE(python27 package.json)
   SCRIPT_INTERPRETER_POST_BUILD_COPY_FILE(python27 icon.png)
   SCRIPT_INTERPRETER_POST_BUILD_COPY_FILE(python27 scriptCaller.py)
   SCRIPT_INTERPRETER_POST_BUILD_COPY_FILE(python27 scriptUtilities.py)
   SCRIPT_INTERPRETER_POST_BUILD_COPY_FILE(python27 template.py)
   SCRIPT_INTERPRETER_POST_BUILD_COPY_DIRECTORY(python27 locales)
	  
   #########################################
   ## Generate the associated yScriptApiwrapper
   #########################################
   add_subdirectory(yScriptApiWrapper)
   
else()
   message(WARNING "Python27 interpreter is not available. Python library is Mssing")
endif()