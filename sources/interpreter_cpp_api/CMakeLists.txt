# Make a static library to link with C++ interpreters

set(interpreter_cpp_api_SOURCES
   stdafx.cpp
   stdafx.h

   ICommandLine.h
   IInterpreterContext.h

   ApiImplementation.h
   ApiImplementation.cpp
   CommandLine.h
   CommandLine.cpp

   AvalaibleRequest.h
   AvalaibleRequest.cpp
   Information.h
   Information.cpp
   LoadScriptContentRequest.h
   LoadScriptContentRequest.cpp
   LogConfiguration.h
   LogConfiguration.cpp
   SaveScriptContentRequest.h
   SaveScriptContentRequest.cpp
   ScriptLogger.h
   ScriptLogger.cpp
   StartScript.h
   StartScript.cpp
   StopScript.h
   StopScript.cpp

   InterpreterContext.h
   InterpreterContext.cpp

   InterpreterMain.cpp

   interpreter_cpp_api/ImplementationHelper.h
   interpreter_cpp_api/IInterpreter.h
)

add_library(interpreter_cpp_api STATIC ${interpreter_cpp_api_SOURCES})

#define the list of all include dirs
set(interpreter_cpp_api_ALL_INCLUDE_DIRS
   ${SHARED_INCL_DIR}
   ${BOOST_INCL_DIR}
   ${Poco_INCLUDE_DIRS}
   ${PROTOBUF_INCLUDE_DIRS}
   ${interpreter_IPC_INCLUDE_DIRS}
   ${CMAKE_CURRENT_SOURCE_DIR}/interpreter_cpp_api
   )

set_property(TARGET interpreter_cpp_api PROPERTY INCLUDE_DIRECTORIES ${interpreter_cpp_api_ALL_INCLUDE_DIRS})


target_link_libraries(interpreter_cpp_api
   yadoms-shared
   ${LIBS}
   ${CMAKE_DL_LIBS}
   ${interpreter_IPC_LIBRARY}
   )

##################################################################################################
## COTIRE
##################################################################################################
if(COTIRE_USE)
	set_target_properties(interpreter_IPC PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "stdafx.h")
	
	if(COTIRE_USE_UNITY)
		set_target_properties(interpreter_IPC PROPERTIES COTIRE_ADD_UNITY_BUILD TRUE)
	else()
		set_target_properties(interpreter_IPC PROPERTIES COTIRE_ADD_UNITY_BUILD FALSE)
	endif()

   
	cotire(interpreter_cpp_api)
	if(COTIRE_USE_UNITY)
		target_link_libraries(interpreter_cpp_api_unity yadoms-shared_unity ${LIBS} ${CMAKE_DL_LIBS} ${interpreter_IPC_LIBRARY}_unity)
		set_property( TARGET interpreter_cpp_api_unity PROPERTY INCLUDE_DIRECTORIES ${interpreter_cpp_api_ALL_INCLUDE_DIRS})
	endif()	
else()
	add_precompiled_header(interpreter_cpp_api stdafx.h)
endif()

IF(MSVC OR XCODE)
   SET_PROPERTY(TARGET interpreter_cpp_api PROPERTY FOLDER "interpreter_cpp_api")
ENDIF()

set(interpreter_cpp_api_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)
