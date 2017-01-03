# Make a static library to link with C++ scripts wrappers

set(script_cpp_api_SOURCES
   stdafx.cpp
   stdafx.h

   ApiImplementation.cpp
   ApiImplementation.h

   script_cpp_api/ImplementationHelper.h
)

add_library(script_cpp_api STATIC ${script_cpp_api_SOURCES})

if(COTIRE_USE)
	#nothing to do
else()
	add_precompiled_header(script_cpp_api stdafx.h)
endif()


#define the list of all include dirs
set(script_cpp_api_ALL_INCLUDE_DIRS
   ${SHARED_INCL_DIR}
   ${BOOST_INCL_DIR}
   ${Poco_INCLUDE_DIRS}
   ${PROTOBUF_INCLUDE_DIRS}
   ${script_IPC_INCLUDE_DIRS}
   ${CMAKE_CURRENT_SOURCE_DIR}/script_cpp_api
   )

set_property(TARGET script_cpp_api PROPERTY INCLUDE_DIRECTORIES ${script_cpp_api_ALL_INCLUDE_DIRS})


target_link_libraries(script_cpp_api
   yadoms-shared
   ${LIBS}
   ${CMAKE_DL_LIBS}
   ${script_IPC_LIBRARY}
   )


IF(MSVC OR XCODE)
   SET_PROPERTY(TARGET script_cpp_api PROPERTY FOLDER "script_cpp_api")
ENDIF()

set(script_cpp_api_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)
