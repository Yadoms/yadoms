# Make a static library to use to encode/decode IPC messages using Protobuf



set(plugin_IPC_SOURCES
   toYadoms.proto
   toPlugin.proto
   
   pluginInit.proto
   
   plugin_IPC/plugin_IPC.h
)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS toYadoms.proto)
set(plugin_IPC_SOURCES ${plugin_IPC_SOURCES} ${PROTO_SRCS} ${PROTO_HDRS})
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS toPlugin.proto)
set(plugin_IPC_SOURCES ${plugin_IPC_SOURCES} ${PROTO_SRCS} ${PROTO_HDRS})
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS pluginInit.proto)
set(plugin_IPC_SOURCES ${plugin_IPC_SOURCES} ${PROTO_SRCS} ${PROTO_HDRS})

set(plugin_IPC_GENERATED_DIR ${CMAKE_CURRENT_BINARY_DIR} CACHE INTERNAL "Path to generated protobuf files.")

include_directories(${plugin_IPC_GENERATED_DIR} ${PROTOBUF_INCLUDE_DIRS})

add_library(plugin_IPC STATIC ${plugin_IPC_SOURCES})
target_link_libraries(plugin_IPC ${PROTOBUF_LIBRARIES})

IF(MSVC OR XCODE)
   SET_PROPERTY(TARGET plugin_IPC PROPERTY FOLDER "plugin_IPC")
ENDIF()


set(plugin_IPC_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} ${plugin_IPC_GENERATED_DIR} CACHE INTERNAL "plugin_IPC include directories")
set(plugin_IPC_LIBRARY plugin_IPC CACHE INTERNAL "plugin_IPC library")



#Ignore some warnings under VisualStudio for Protobuf generated files
if(MSVC)
add_definitions( "/wd4244 /wd4125" )
endif()
