# Make a static library to use to encode/decode IPC messages using Protobuf



set(script_IPC_SOURCES
   scriptToYadoms.proto
   yadomsToScript.proto
)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS scriptToYadoms.proto)
set(script_IPC_SOURCES ${script_IPC_SOURCES} ${PROTO_SRCS} ${PROTO_HDRS})
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS yadomsToScript.proto)
set(script_IPC_SOURCES ${script_IPC_SOURCES} ${PROTO_SRCS} ${PROTO_HDRS})

set(script_IPC_GENERATED_DIR ${CMAKE_CURRENT_BINARY_DIR} CACHE INTERNAL "Path to generated protobuf files.")

add_library(script_IPC STATIC ${script_IPC_SOURCES})
target_link_libraries(script_IPC ${PROTOBUF_LIBRARIES})

ADD_VS_NATVIS(script_IPC)

set_property(TARGET script_IPC PROPERTY INCLUDE_DIRECTORIES ${script_IPC_GENERATED_DIR} ${PROTOBUF_INCLUDE_DIRS})

IF(MSVC OR XCODE)
   SET_PROPERTY(TARGET script_IPC PROPERTY FOLDER "script_IPC")
ENDIF()


set(script_IPC_INCLUDE_DIRS ${script_IPC_GENERATED_DIR}/.. CACHE INTERNAL "script_IPC include directories")
set(script_IPC_LIBRARY script_IPC CACHE INTERNAL "script_IPC library")


if(COTIRE_USE)
   if(COTIRE_USE_UNITY)
      set_target_properties(script_IPC PROPERTIES COTIRE_ADD_UNITY_BUILD TRUE)
   else()
      set_target_properties(script_IPC PROPERTIES COTIRE_ADD_UNITY_BUILD FALSE)
   endif()
   
   # this target is for protobuf and precompiled header has no real sense
   set_target_properties(script_IPC PROPERTIES COTIRE_ENABLE_PRECOMPILED_HEADER FALSE)

   cotire(script_IPC)
   
   if(COTIRE_USE_UNITY)
      target_link_libraries(script_IPC_unity ${PROTOBUF_LIBRARIES})
      ADD_VS_NATVIS(script_IPC_unity)
   endif()	
endif()


#Ignore some warnings under VisualStudio for Protobuf generated files
if(MSVC)
add_definitions( "/wd4244 /wd4125" )
endif()
