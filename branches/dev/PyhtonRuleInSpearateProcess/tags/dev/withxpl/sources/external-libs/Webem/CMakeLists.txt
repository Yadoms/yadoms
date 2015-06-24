#
# Static and shared library version of sqlite3.
# libgpkg is not linked against this version of sqlite3, it is only used to:
# - provide the sqlite3 and sqlite3ext headers
# - provide a sqlite3 binary with a known configuration for the unit tests
set(WEBEM_SRC
	cWebem.h
	cWebem.cpp
	Base64.cpp
	Base64.h
	connection.cpp
	connection.hpp
	connection_manager.cpp
	connection_manager.hpp
	header.hpp
	mime_types.cpp
	mime_types.hpp
	reply.cpp
	reply.hpp
	request.hpp
	request_handler.cpp
	request_handler.hpp
	request_parser.cpp
	request_parser.hpp
	server.cpp
	server.hpp
	Helper.cpp
	Helper.h
	localtime_r.h
	localtime_r.cpp
	)


add_library ( webem STATIC ${WEBEM_SRC} )
set_target_properties( webem PROPERTIES OUTPUT_NAME "webem" )
set_target_properties( webem PROPERTIES LIBRARY_OUTPUT_DIRECTORY static )

add_definitions(-DWEBSERVER_DONT_USE_ZIP)

set_property( TARGET webem PROPERTY INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR} ${BOOST_ROOT})

#
# Strip 'lib' prefix when compiling for windows
if(WIN32)
  set_target_properties ( webem PROPERTIES PREFIX "" )
endif()

IF(WIN32)
	SET_PROPERTY(TARGET webem PROPERTY FOLDER "External Libs")
ENDIF()


set(WEBEM_INCLUDE_DIR2 "${CMAKE_CURRENT_SOURCE_DIR}" )
set(WEBEM_INCLUDE_DIRECTORY ${WEBEM_INCLUDE_DIR2} PARENT_SCOPE)

