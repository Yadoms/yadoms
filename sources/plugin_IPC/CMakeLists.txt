# Make a static library to use to encode/decode IPC messages using Protobuf



set(plugin_IPC_SOURCES
   pluginToYadoms.proto
   yadomsToPlugin.proto
   
   pluginInit.proto
)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS pluginToYadoms.proto)
set(plugin_IPC_SOURCES ${plugin_IPC_SOURCES} ${PROTO_SRCS} ${PROTO_HDRS})
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS yadomsToPlugin.proto)
set(plugin_IPC_SOURCES ${plugin_IPC_SOURCES} ${PROTO_SRCS} ${PROTO_HDRS})
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS pluginInit.proto)
set(plugin_IPC_SOURCES ${plugin_IPC_SOURCES} ${PROTO_SRCS} ${PROTO_HDRS})

set(plugin_IPC_GENERATED_DIR ${CMAKE_CURRENT_BINARY_DIR} CACHE INTERNAL "Path to generated protobuf files.")

add_library(plugin_IPC STATIC ${plugin_IPC_SOURCES})
target_link_libraries(plugin_IPC ${PROTOBUF_LIBRARIES})

set_property(TARGET plugin_IPC PROPERTY INCLUDE_DIRECTORIES ${plugin_IPC_GENERATED_DIR} ${PROTOBUF_INCLUDE_DIRS})

if(COTIRE_USE)
   # this target is for protobuf and precompiled header has no real sense
   set_target_properties(plugin_IPC PROPERTIES COTIRE_ENABLE_PRECOMPILED_HEADER FALSE)
   set_target_properties(plugin_IPC PROPERTIES COTIRE_ADD_UNITY_BUILD FALSE)

   cotire(plugin_IPC)
endif()


IF(MSVC OR XCODE)
   SET_PROPERTY(TARGET plugin_IPC PROPERTY FOLDER "plugin_IPC")
ENDIF()


set(plugin_IPC_INCLUDE_DIRS ${plugin_IPC_GENERATED_DIR}/.. CACHE INTERNAL "plugin_IPC include directories")
set(plugin_IPC_LIBRARY plugin_IPC CACHE INTERNAL "plugin_IPC library")



if(MSVC)
   #Ignore some warnings under VisualStudio for Protobuf generated files
   add_definitions( "/wd4244 /wd4125" )
endif()
