#
# Static and shared library version of sqlite3.
# libgpkg is not linked against this version of sqlite3, it is only used to:
# - provide the sqlite3 and sqlite3ext headers
# - provide a sqlite3 binary with a known configuration for the unit tests
set(WEBEM_SRC
	cWebem.h
	cWebem.cpp
	cWebemTable.h
	cWebemTable.cpp
	server/connection.cpp
	server/connection.hpp
	server/connection_manager.cpp
	server/connection_manager.hpp
	server/header.hpp
	server/mime_types.cpp
	server/mime_types.hpp
	server/reply.cpp
	server/reply.hpp
	server/request.hpp
	server/request_handler.cpp
	server/request_handler.hpp
	server/request_parser.cpp
	server/request_parser.hpp
	server/server.cpp
	server/server.hpp)


add_library ( webem STATIC ${WEBEM_SRC} )
set_target_properties( webem PROPERTIES OUTPUT_NAME "webem" )
set_target_properties( webem PROPERTIES LIBRARY_OUTPUT_DIRECTORY static )


set_property( TARGET webem PROPERTY INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR} ${BOOST_ROOT})

#
# Strip 'lib' prefix when compiling for windows
if ( WIN32 )
  set_target_properties ( webem PROPERTIES PREFIX "" )
endif()

IF(MSVC)
	SET_PROPERTY(TARGET webem PROPERTY FOLDER "External Libs")
ENDIF()


set(WEBEM_INCLUDE_DIR2 "${CMAKE_CURRENT_SOURCE_DIR}" )
set(WEBEM_INCLUDE_DIRECTORY ${WEBEM_INCLUDE_DIR2} PARENT_SCOPE)

