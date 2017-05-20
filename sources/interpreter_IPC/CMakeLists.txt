# Make a static library to use to encode/decode IPC messages using Protobuf



set(interpreter_IPC_SOURCES
   interpreterToYadoms.proto
   yadomsToInterpreter.proto

   interpreterInit.proto
)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS interpreterToYadoms.proto)
set(interpreter_IPC_SOURCES ${interpreter_IPC_SOURCES} ${PROTO_SRCS} ${PROTO_HDRS})
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS yadomsToInterpreter.proto)
set(interpreter_IPC_SOURCES ${interpreter_IPC_SOURCES} ${PROTO_SRCS} ${PROTO_HDRS})
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS interpreterInit.proto)
set(interpreter_IPC_SOURCES ${interpreter_IPC_SOURCES} ${PROTO_SRCS} ${PROTO_HDRS})

set(interpreter_IPC_GENERATED_DIR ${CMAKE_CURRENT_BINARY_DIR} CACHE INTERNAL "Path to generated protobuf files.")

add_library(interpreter_IPC STATIC ${interpreter_IPC_SOURCES})
target_link_libraries(interpreter_IPC ${PROTOBUF_LIBRARIES})

set_property(TARGET interpreter_IPC PROPERTY INCLUDE_DIRECTORIES ${interpreter_IPC_GENERATED_DIR} ${PROTOBUF_INCLUDE_DIRS})

IF(MSVC OR XCODE)
   SET_PROPERTY(TARGET interpreter_IPC PROPERTY FOLDER "interpreter_IPC")
ENDIF()


set(interpreter_IPC_INCLUDE_DIRS ${interpreter_IPC_GENERATED_DIR}/.. CACHE INTERNAL "interpreter_IPC include directories")
set(interpreter_IPC_LIBRARY interpreter_IPC CACHE INTERNAL "interpreter_IPC library")

##################################################################################################
## COTIRE
##################################################################################################
if(COTIRE_USE)
   if(COTIRE_USE_UNITY)
      set_target_properties(interpreter_IPC PROPERTIES COTIRE_ADD_UNITY_BUILD TRUE)
   else()
      set_target_properties(interpreter_IPC PROPERTIES COTIRE_ADD_UNITY_BUILD FALSE)
   endif()

	cotire(interpreter_IPC)
   
	if(COTIRE_USE_UNITY)
		target_link_libraries(interpreter_IPC_unity ${PROTOBUF_LIBRARIES})
		set_property( TARGET interpreter_IPC_unity PROPERTY INCLUDE_DIRECTORIES ${interpreter_IPC_GENERATED_DIR} ${PROTOBUF_INCLUDE_DIRS})
	endif()	
endif()
      
      

#Ignore some warnings under VisualStudio for Protobuf generated files
if(MSVC)
add_definitions( "/wd4244 /wd4125" )
endif()
