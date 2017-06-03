#
# Required CMake modules
include(CheckIncludeFile)
include(CheckFunctionExists)


set(SQLITE_DIR_NAME sqlite-amalgamation-3180000)
#
# Static and shared library version of sqlite3.
# libgpkg is not linked against this version of sqlite3, it is only used to:
# - provide the sqlite3 and sqlite3ext headers
# - provide a sqlite3 binary with a known configuration for the unit tests

add_library ( sqlite STATIC ${SQLITE_DIR_NAME}/sqlite3.c )
set_target_properties( sqlite PROPERTIES OUTPUT_NAME "sqlite3" )
set_target_properties( sqlite PROPERTIES LIBRARY_OUTPUT_DIRECTORY static )

#add_definitions ( -D_HAVE_SQLITE_CONFIG_H=1 )

check_include_file(stdint.h HAVE_STDINT_H)
check_include_file(inttype.h HAVE_INTTYPES_H)
check_include_file(utime.h HAVE_UTIME_H)

check_function_exists(gmtime_r HAVE_GMTIME_R)
check_function_exists(localtime_r HAVE_LOCALTIME_R)
check_function_exists(posix_fallocate HAVE_POSIX_FALLOCATE)
check_function_exists(usleep HAVE_USLEEP)

set ( SQLITE_ENABLE_RTREE 1 )

find_package ( Threads )
if ( CMAKE_USE_PTHREADS_INIT )
  set( SQLITE_THREADSAFE 2 )
  target_link_libraries( sqlite "${CMAKE_THREAD_LIBS_INIT}" )
elseif ( CMAKE_USE_WIN32_THREADS_INIT )
  set( SQLITE_THREADSAFE 2 )
else()
  set( SQLITE_THREADSAFE 1 )
endif()

if ( UNIX )
  target_link_libraries( sqlite "${CMAKE_DL_LIBS}" )
endif()

configure_file ( "${CMAKE_CURRENT_SOURCE_DIR}/config.h.in" "${CMAKE_CURRENT_BINARY_DIR}/config.h" )


#
# Strip 'lib' prefix when compiling for windows
if ( WIN32 )
  set_target_properties ( sqlite PROPERTIES PREFIX "" )
endif()

IF(WIN32)
	SET_PROPERTY(TARGET sqlite PROPERTY FOLDER "External Libs")
	
	#set warning level3 only for this project
	add_definitions("/W3")
ENDIF()

set(SQLITE_INCLUDE_DIR2 "${CMAKE_CURRENT_SOURCE_DIR}/${SQLITE_DIR_NAME}" )
set(SQLITE_INCLUDE_DIRECTORY ${SQLITE_INCLUDE_DIR2} PARENT_SCOPE)


