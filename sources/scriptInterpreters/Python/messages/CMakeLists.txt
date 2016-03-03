# Make a static library to use to encode/decode IPC messages using Protobuf



set(messages_SOURCES   
   pbRequest.proto
   pbAnswer.proto
)

# Request
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS pbRequest.proto)
set(messages_SOURCES ${messages_SOURCES} ${PROTO_SRCS} ${PROTO_HDRS})

# Answer
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS pbAnswer.proto)
set(messages_SOURCES ${messages_SOURCES} ${PROTO_SRCS} ${PROTO_HDRS})

set(messages_GENERATED_DIR ${CMAKE_CURRENT_BINARY_DIR} CACHE INTERNAL "Path to generated protobuf files.")

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${PROTOBUF_INCLUDE_DIRS})

add_library(messages STATIC ${messages_SOURCES})
target_link_libraries(messages ${PROTOBUF_LIBRARIES})

IF(MSVC OR XCODE)
   SET_PROPERTY(TARGET messages PROPERTY FOLDER "scriptInterpreters/python")
ENDIF()



#Ignore some warnings under VisualStudio for Protobuf generated files
if(MSVC)
add_definitions( "/wd4244 /wd4125" )
endif()
