# Check dependencies

find_package (Python2 COMPONENTS Interpreter Development)

#message("==================================================================")
#message("Python2_ROOT: ${Python2_ROOT}")
#message("Python2_FOUND: ${Python2_FOUND}")
#message("Python2_INCLUDE_DIRS: ${Python2_INCLUDE_DIRS}")
#message("Python2_LIBRARIES: ${Python2_LIBRARIES}")
#message("Python2_Interpreter_FOUND: ${Python2_Interpreter_FOUND}")
#message("Python2_EXECUTABLE: ${Python2_EXECUTABLE}")

if(${Python2_FOUND})

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
   ## even if YPYTHON3_INCLDIR or YPYTHON3_LINK are empty
   #########################################
   SCRIPT_INTERPRETER_SOURCES(yPython27 ${YPYTHON27_SOURCES})
   SCRIPT_INTERPRETER_INCLDIR(yPython27 ${YPYTHON2_INCLDIR} ${PLATFORM_SPECIFIC_INCL_DIR} ${PROTOBUF_INCLUDE_DIRS})
   SCRIPT_INTERPRETER_LINK(yPython27 ${YPYTHON2_LINK})

   #########################################
   ## Post-build commands
   ## Copy files to build directory
   #########################################
   SCRIPT_INTERPRETER_POST_BUILD_COPY_FILE(yPython27 scriptCaller.py)
   SCRIPT_INTERPRETER_POST_BUILD_COPY_FILE(yPython27 scriptUtilities.py)
   SCRIPT_INTERPRETER_POST_BUILD_COPY_FILE(yPython27 template.py)
	  
   #########################################
   ## Generate the associated yScriptApiwrapper
   #########################################
   add_subdirectory(yScriptApiWrapper)
   
else()
   message(SEND_ERROR "yPython27 interpreter is not available. Python 2.7.x library is Mssing")
endif()
