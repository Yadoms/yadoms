# Check dependencies

set(Python3_USE_STATIC_LIBS FALSE)

set(PYTHON_INCLUDE_DIR PYTHON_INCLUDE_DIR-NOTFOUND)
set(PYTHON_LIBRARY PYTHON_LIBRARY-NOTFOUND)

if(${Python3_ManualSetup})
	if(EXISTS ${Python3_EXECUTABLE})
		set(Python3_Interpreter_FOUND TRUE)
		if(EXISTS ${Python3_LIBRARIES})
				if(EXISTS ${Python3_INCLUDE_DIRS})
					set(Python3_FOUND TRUE)
				else(EXISTS ${Python3_INCLUDE_DIRS})
					message(FATAL_ERROR "The specified Python3_INCLUDE_DIRS is not found : ${Python3_INCLUDE_DIRS}")
				endif(EXISTS ${Python3_INCLUDE_DIRS})

		else(EXISTS ${Python3_LIBRARIES})
			message(FATAL_ERROR "The specified Python3_LIBRARIES is not found : ${Python3_LIBRARIES}")
		endif(EXISTS ${Python3_LIBRARIES})
	else(EXISTS ${Python3_EXECUTABLE})
		message(FATAL_ERROR "The specified Python3 executable is not found : ${Python3_EXECUTABLE}")
	endif(EXISTS ${Python3_EXECUTABLE})
else(${Python3_ManualSetup})
	find_package (Python3 COMPONENTS Interpreter Development)
endif(${Python3_ManualSetup})


if(${Python3_FOUND})

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
   message("==================================================================")
   message("Python3_ROOT: ${Python3_ROOT}")
   message("Python3_FOUND: ${Python3_FOUND}")
   message("Python3_INCLUDE_DIRS: ${Python3_INCLUDE_DIRS}")
   message("Python3_LIBRARIES: ${Python3_LIBRARIES}")
   message("Python3_Interpreter_FOUND: ${Python3_Interpreter_FOUND}")
   message("Python3_EXECUTABLE: ${Python3_EXECUTABLE}")

   message(SEND_ERROR "yPython3 interpreter is not available. Python 3.x library is Mssing")
endif()
