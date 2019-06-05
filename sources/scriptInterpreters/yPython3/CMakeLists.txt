# Check dependencies

set (PYTHON3_REQUIRED_VERSION 3.7)
include(findPython3.cmake)
  
if(PYTHONLIBS3_FOUND)

   # include common script interpreters definitions
   include("../YadomsScriptInterpertersCommon.cmake")   
	  
   #########################################
   ## Place here all the source files
   #########################################
   set(YPYTHON3_SOURCES
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
      Python3.h
      Python3.cpp
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
      set(YPYTHON3_SOURCES	
         ${YPYTHON3_SOURCES}
         platformSpecific/windows/PythonExecutablePath.h
         platformSpecific/windows/PythonExecutablePath.cpp
      )	
      set(PLATFORM_SPECIFIC_INCL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/platformSpecific/windows)

   ELSEIF(APPLE)
      set(YPYTHON3_SOURCES	
         ${YPYTHON3_SOURCES}
         platformSpecific/mac/PythonExecutablePath.h
         platformSpecific/mac/PythonExecutablePath.cpp
      )	
      set(PLATFORM_SPECIFIC_INCL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/platformSpecific/mac)

   ELSE()
      set(YPYTHON3_SOURCES	
         ${YPYTHON3_SOURCES}
         platformSpecific/linux/PythonExecutablePath.h
         platformSpecific/linux/PythonExecutablePath.cpp
      )	
      set(PLATFORM_SPECIFIC_INCL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/platformSpecific/linux)

   ENDIF()   
   
   
   #########################################
   ## Always leave these lines to configure script interpreter correctly
   ## even if YPYTHON3_INCLDIR or YPYTHON3_LINK are empty
   #########################################
   SCRIPT_INTERPRETER_SOURCES(yPython3 ${YPYTHON3_SOURCES})
   SCRIPT_INTERPRETER_INCLDIR(yPython3 ${YPYTHON3_INCLDIR} ${PLATFORM_SPECIFIC_INCL_DIR} ${PROTOBUF_INCLUDE_DIRS})
   SCRIPT_INTERPRETER_LINK(yPython3 ${YPYTHON3_LINK})

   #########################################
   ## Post-build commands
   ## Copy files to build directory
   #########################################
   SCRIPT_INTERPRETER_POST_BUILD_COPY_FILE(yPython3 scriptCaller.py)
   SCRIPT_INTERPRETER_POST_BUILD_COPY_FILE(yPython3 scriptUtilities.py)
   SCRIPT_INTERPRETER_POST_BUILD_COPY_FILE(yPython3 template.py)
	  
   #########################################
   ## Generate the associated yScriptApiwrapper
   #########################################
   add_subdirectory(yScriptApiWrapper)
   
else()
   message(SEND_ERROR "yPython3 interpreter is not available. Python 3.x library is Mssing")
endif()