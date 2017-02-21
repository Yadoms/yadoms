# Check dependencies

set (PYTHON_REQUIRED_VERSION 2.7)
include(findPython.cmake)
  
if(PYTHONLIBS_FOUND)

   # include common script interpreters definitions
   include("../YadomsScriptInterpertersCommon.cmake")   
	  
   #########################################
   ## Place here all the source files
   #########################################
   set(YPYTHON27_SOURCES
      stdafx.h
      stdafx.cpp
      EventScriptStopped.h
      EventScriptStopped.cpp
      Factory.h
      Factory.cpp
      IEventScriptStopped.h
      IPythonExecutable.h
      IFactory.h
      IScriptFile.h
      ProcessObserver.h
      ProcessObserver.cpp
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
      set(YPYTHON27_SOURCES	
         ${YPYTHON27_SOURCES}
         platformSpecific/windows/PythonExecutablePath.h
         platformSpecific/windows/PythonExecutablePath.cpp
      )	
      set(PLATFORM_SPECIFIC_INCL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/platformSpecific/windows)

   ELSEIF(APPLE)
      set(YPYTHON27_SOURCES	
         ${YPYTHON27_SOURCES}
         platformSpecific/mac/PythonExecutablePath.h
         platformSpecific/mac/PythonExecutablePath.cpp
      )	
      set(PLATFORM_SPECIFIC_INCL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/platformSpecific/mac)

   ELSE()
      set(YPYTHON27_SOURCES	
         ${YPYTHON27_SOURCES}
         platformSpecific/linux/PythonExecutablePath.h
         platformSpecific/linux/PythonExecutablePath.cpp
      )	
      set(PLATFORM_SPECIFIC_INCL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/platformSpecific/linux)

   ENDIF()   
   
   
   #########################################
   ## Always leave these lines to configure script interpreter correctly
   ## even if YPYTHON27_INCLDIR or YPYTHON27_LINK are empty
   #########################################
   SCRIPT_INTERPRETER_SOURCES(yPython27 ${YPYTHON27_SOURCES})
   SCRIPT_INTERPRETER_INCLDIR(yPython27 ${YPYTHON27_INCLDIR} ${PLATFORM_SPECIFIC_INCL_DIR} ${PROTOBUF_INCLUDE_DIRS})
   SCRIPT_INTERPRETER_LINK(yPython27 ${YPYTHON27_LINK})

   #########################################
   ## Post-build commands
   ## Copy files to build directory
   #########################################
   SCRIPT_INTERPRETER_POST_BUILD_COPY_FILE(yPython27 package.json)
   SCRIPT_INTERPRETER_POST_BUILD_COPY_FILE(yPython27 icon.png)
   SCRIPT_INTERPRETER_POST_BUILD_COPY_FILE(yPython27 scriptCaller.py)
   SCRIPT_INTERPRETER_POST_BUILD_COPY_FILE(yPython27 scriptUtilities.py)
   SCRIPT_INTERPRETER_POST_BUILD_COPY_FILE(yPython27 template.py)
   SCRIPT_INTERPRETER_POST_BUILD_COPY_DIRECTORY(yPython27 locales)
	  
   #########################################
   ## Generate the associated yScriptApiwrapper
   #########################################
   add_subdirectory(yScriptApiWrapper)
   
else()
   message(WARNING "yPython27 interpreter is not available. Python library is Mssing")
endif()