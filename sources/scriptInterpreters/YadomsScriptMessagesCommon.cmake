# Macros for setting up a script messages for yScriptApi
#
# Note that we use Protobuf to generate messages.

# 
MACRO(MAKE_SCRIPT_API_MESSAGES_LIBRARY target proto_file ide_folder)

   set(MESSAGES_LIBRARY_SOURCES   
      Messages.proto
   )

   protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${proto_file})
   set(MESSAGES_LIBRARY_SOURCES ${MESSAGES_LIBRARY_SOURCES} ${PROTO_SRCS} ${PROTO_HDRS})
   set(${target}_GENERATED_DIR ${CMAKE_CURRENT_BINARY_DIR} CACHE INTERNAL "Path to generated protobuf files.")

   include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${PROTOBUF_INCLUDE_DIRS})

   add_library(${target} STATIC ${MESSAGES_LIBRARY_SOURCES})
   target_link_libraries(${target} ${PROTOBUF_LIBRARIES})

   IF(MSVC OR XCODE)
      SET_PROPERTY(TARGET ${target} PROPERTY FOLDER ${ide_folder})
   ENDIF()



   #Ignore some warnings under VisualStudio for Protobuf generated files
   if(MSVC)
   add_definitions( "/wd4244 /wd4125" )
   endif()

ENDMACRO()