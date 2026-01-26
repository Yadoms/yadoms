# Make a static library to use to encode/decode IPC messages using Protobuf



set(interpreter_IPC_PROTOS
   ${CMAKE_CURRENT_SOURCE_DIR}/interpreterToYadoms.proto
   ${CMAKE_CURRENT_SOURCE_DIR}/yadomsToInterpreter.proto

   ${CMAKE_CURRENT_SOURCE_DIR}/interpreterInit.proto
)

protobuf_generate(
   LANGUAGE cpp
   OUT_VAR interpreter_IPC_SOURCES
   PROTOS ${interpreter_IPC_PROTOS}
)

set(interpreter_IPC_GENERATED_DIR ${CMAKE_CURRENT_BINARY_DIR} CACHE INTERNAL "Path to generated protobuf files.")

add_library(interpreter_IPC STATIC ${interpreter_IPC_SOURCES})
target_link_libraries(interpreter_IPC protobuf::libprotobuf)
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
   # this target is for protobuf and precompiled header has no real sense
   set_target_properties(interpreter_IPC PROPERTIES COTIRE_ENABLE_PRECOMPILED_HEADER FALSE)
   set_target_properties(interpreter_IPC PROPERTIES COTIRE_ADD_UNITY_BUILD FALSE)

   cotire(interpreter_IPC)
endif()



if(MSVC)
   #Ignore some warnings under VisualStudio for Protobuf generated files
   add_definitions( "/wd4244 /wd4125" )
endif()
