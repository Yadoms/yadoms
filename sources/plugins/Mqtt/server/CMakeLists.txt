#########################################
## The file allow building Openzwave for anyplatform
##
## Build tested with:
##		- Windows /VisualStudio 2013
##		- MacOS / Xcode
##		- Linux Ubuntu / makefile
##		- Raspberry (cross compiled from Ubuntu) / makefile
##
#########################################

#########################################
## Define the main folder name
#########################################

#When changing openzwave, remeber to enable exceptions, by undefining OPENZWAVE_DISABLE_EXCEPTIONS (cpp/src/Defs.h, line 149 in v1.4.1)
#Include "vers.cpp" to git manually
set(MOSQUITTO_DIR_NAME mosquitto-1.4.15)

set (VERSION 1.4.15)

if (WIN32)
	execute_process(COMMAND cmd /c echo %DATE% %TIME% OUTPUT_VARIABLE TIMESTAMP
			OUTPUT_STRIP_TRAILING_WHITESPACE)
else (WIN32)
	execute_process(COMMAND date "+%F %T%z" OUTPUT_VARIABLE TIMESTAMP
			OUTPUT_STRIP_TRAILING_WHITESPACE)
endif (WIN32)

add_definitions (-DCMAKE -DVERSION=\"${VERSION}\" -DTIMESTAMP=\"${TIMESTAMP}\")

set (MOSQ_SRCS
	${MOSQUITTO_DIR_NAME}/src/conf.c
	${MOSQUITTO_DIR_NAME}/src/context.c
	${MOSQUITTO_DIR_NAME}/src/database.c
	${MOSQUITTO_DIR_NAME}/src/lib_load.h
	${MOSQUITTO_DIR_NAME}/src/logging.c
	${MOSQUITTO_DIR_NAME}/src/loop.c
	${MOSQUITTO_DIR_NAME}/src/../lib/memory_mosq.c 
   ${MOSQUITTO_DIR_NAME}/src/../lib/memory_mosq.h
	mosquitto-main.h
	mosquitto-main.c
	${MOSQUITTO_DIR_NAME}/src/mosquitto_broker.h
	${MOSQUITTO_DIR_NAME}/src/net.c
	${MOSQUITTO_DIR_NAME}/src/../lib/net_mosq.c 
   ${MOSQUITTO_DIR_NAME}/src/../lib/net_mosq.h
	${MOSQUITTO_DIR_NAME}/src/persist.c 
   ${MOSQUITTO_DIR_NAME}/src/persist.h
	${MOSQUITTO_DIR_NAME}/src/read_handle.c 
   ${MOSQUITTO_DIR_NAME}/src/read_handle_client.c 
   ${MOSQUITTO_DIR_NAME}/src/read_handle_server.c
	${MOSQUITTO_DIR_NAME}/src/../lib/read_handle_shared.c 
   ${MOSQUITTO_DIR_NAME}/src/../lib/read_handle.h
	${MOSQUITTO_DIR_NAME}/src/subs.c
	${MOSQUITTO_DIR_NAME}/src/security.c 
   ${MOSQUITTO_DIR_NAME}/src/security_default.c
	${MOSQUITTO_DIR_NAME}/src/../lib/send_client_mosq.c 
   ${MOSQUITTO_DIR_NAME}/src/../lib/send_mosq.h
	${MOSQUITTO_DIR_NAME}/src/../lib/send_mosq.c 
   ${MOSQUITTO_DIR_NAME}/src/../lib/send_mosq.h
	${MOSQUITTO_DIR_NAME}/src/send_server.c
	${MOSQUITTO_DIR_NAME}/src/sys_tree.c
	${MOSQUITTO_DIR_NAME}/src/../lib/time_mosq.c
	${MOSQUITTO_DIR_NAME}/src/../lib/tls_mosq.c
	${MOSQUITTO_DIR_NAME}/src/../lib/util_mosq.c 
   ${MOSQUITTO_DIR_NAME}/src/../lib/util_mosq.h
	${MOSQUITTO_DIR_NAME}/src/websockets.c
	${MOSQUITTO_DIR_NAME}/src/../lib/will_mosq.c 
   ${MOSQUITTO_DIR_NAME}/src/../lib/will_mosq.h)

option(INC_BRIDGE_SUPPORT
	"Include bridge support for connecting to other brokers?" ON)
if (${INC_BRIDGE_SUPPORT} STREQUAL ON)
	set (MOSQ_SRCS ${MOSQ_SRCS} ${MOSQUITTO_DIR_NAME}/src/bridge.c)
	add_definitions("-DWITH_BRIDGE")
endif (${INC_BRIDGE_SUPPORT} STREQUAL ON)


option(USE_LIBWRAP
	"Include tcp-wrappers support?" OFF)

if (${USE_LIBWRAP} STREQUAL ON)
	set (MOSQ_LIBS ${MOSQ_LIBS} wrap)
	add_definitions("-DWITH_WRAP")
endif (${USE_LIBWRAP} STREQUAL ON)

option(INC_DB_UPGRADE
	"Include database upgrade support? (recommended)" ON)

option(INC_MEMTRACK
	"Include memory tracking support?" ON)
if (${INC_MEMTRACK} STREQUAL ON)
	add_definitions("-DWITH_MEMORY_TRACKING")
endif (${INC_MEMTRACK} STREQUAL ON)

option(WITH_PERSISTENCE
	"Include persistence support?" ON)
if (${WITH_PERSISTENCE} STREQUAL ON)
	add_definitions("-DWITH_PERSISTENCE")
endif (${WITH_PERSISTENCE} STREQUAL ON)

option(WITH_SYS_TREE
	"Include $SYS tree support?" ON)
if (${WITH_SYS_TREE} STREQUAL ON)
	add_definitions("-DWITH_SYS_TREE")
endif (${WITH_SYS_TREE} STREQUAL ON)

option(WITH_WEBSOCKETS
	"Include websockets support?" OFF)
if (${WITH_WEBSOCKETS} STREQUAL ON)
	add_definitions("-DWITH_WEBSOCKETS")
endif (${WITH_WEBSOCKETS} STREQUAL ON)

if (WIN32 OR CYGWIN)
	set (MOSQ_SRCS ${MOSQ_SRCS} ${MOSQUITTO_DIR_NAME}/src/service.c)
endif (WIN32 OR CYGWIN)

add_definitions (-DWITH_BROKER)

set (MOSQ_LIBS ${MOSQ_LIBS} ${OPENSSL_LIBRARIES})

# Check for getaddrinfo_a
include(CheckLibraryExists)
check_library_exists(anl getaddrinfo_a  "" HAVE_GETADDRINFO_A)
if (HAVE_GETADDRINFO_A)
	add_definitions(-DHAVE_GETADDRINFO_A)
	set (MOSQ_LIBS ${MOSQ_LIBS} anl)
endif (HAVE_GETADDRINFO_A)



if (UNIX)
	if (APPLE)
		set (MOSQ_LIBS ${MOSQ_LIBS} dl m)
	else (APPLE)
		set (MOSQ_LIBS ${MOSQ_LIBS} dl m)
		find_library(LIBRT rt)
		if (LIBRT)
			set (MOSQ_LIBS ${MOSQ_LIBS} rt)
		endif (LIBRT)
	endif (APPLE)
endif (UNIX)

if (WIN32)
	set (MOSQ_LIBS ${MOSQ_LIBS} ws2_32)
endif (WIN32)

if (${WITH_WEBSOCKETS} STREQUAL ON)
	set (MOSQ_LIBS ${MOSQ_LIBS} websockets)
endif (${WITH_WEBSOCKETS} STREQUAL ON)

# Simple detect libuuid
if(NOT APPLE)
	FIND_PATH(UUID_HEADER uuid/uuid.h)
	if (UUID_HEADER)
		add_definitions(-DWITH_UUID)
		set (MOSQ_LIBS ${MOSQ_LIBS} uuid)
	endif (UUID_HEADER)
endif(NOT APPLE)

add_library ( mosquitto STATIC ${MOSQ_SRCS} )
set_target_properties( mosquitto PROPERTIES OUTPUT_NAME "mosquitto" )
set_target_properties( mosquitto PROPERTIES LIBRARY_OUTPUT_DIRECTORY static )

if (UNIX)
	if (APPLE)
		set_target_properties(mosquitto PROPERTIES LINK_FLAGS "-Wl,-exported_symbols_list -Wl,${mosquitto_SOURCE_DIR}/src/linker-macosx.syms")
	else (APPLE)
		set_target_properties(mosquitto PROPERTIES LINK_FLAGS "-Wl,-dynamic-list=${mosquitto_SOURCE_DIR}/src/linker.syms")
	endif (APPLE)
endif (UNIX)

set_property( TARGET mosquitto PROPERTY INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/${MOSQUITTO_DIR_NAME}/lib" "${CMAKE_CURRENT_SOURCE_DIR}/${MOSQUITTO_DIR_NAME}/src" "${CMAKE_CURRENT_SOURCE_DIR}/${MOSQUITTO_DIR_NAME}")

#if (${WITH_TLS} STREQUAL ON)
#	add_executable(mosquitto_passwd mosquitto_passwd.c)
#	target_link_libraries(mosquitto_passwd "${OPENSSL_LIBRARIES}")
#	install(TARGETS mosquitto_passwd RUNTIME DESTINATION "${BINDIR}" LIBRARY DESTINATION "${LIBDIR}")
#endif (${WITH_TLS} STREQUAL ON)

if (UNIX)
	install(CODE "EXEC_PROGRAM(/sbin/ldconfig)")
endif (UNIX)

#########################################
## Share include directories for project which use openzwave
#########################################
set(MOSQUITTO_INCLUDE_DIR2 "${CMAKE_CURRENT_SOURCE_DIR}" )
set(MOSQUITTO_INCLUDE_DIRECTORY 
	${MOSQUITTO_INCLUDE_DIR2}
	${MOSQUITTO_DIR_NAME}/lib
	${MOSQUITTO_DIR_NAME}/src
	${MOSQUITTO_DIR_NAME}
	PARENT_SCOPE
)




