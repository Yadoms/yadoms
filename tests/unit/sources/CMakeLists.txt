##################################################################################################
## CMakeList.txt for tests for YADOMS project
##
## This file generates projects/makefiles for the compete Yadoms tests software
##
## This file actually works for
##   Windows 7 x64, MSVC 12
##   Ubuntu 12.04, g++ 4.7
##   Raspberry Raspbian Wheezy, g++ 4.7
##   MacOSx Snow Leopard 10.8, g++ 4.8
##################################################################################################
cmake_minimum_required (VERSION 3.25.0)
project (YadomsTests)

# Global CMake policies
cmake_policy(SET CMP0074 NEW)
cmake_policy(SET CMP0077 NEW)
cmake_policy(SET CMP0078 NEW)
cmake_policy(SET CMP0086 NEW)
cmake_policy(SET CMP0111 NEW)
cmake_policy(SET CMP0144 NEW)
cmake_policy(SET CMP0167 NEW)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# The main Yadoms path
set(YADOMS_PATH ../../../sources)


##################################################################################################
## Include other cmake scripts
##################################################################################################

# First checks (compilers versions...)
include(${YADOMS_PATH}/cmake/preChecks.cmake)

# User specific configuration
include(CMakeListsUserConfig.txt OPTIONAL)

# Precompiled headers
include(${YADOMS_PATH}/cmake/PrecompiledHeader.cmake)

# Compilers/IDE configurations
include(${YADOMS_PATH}/cmake/compilers.cmake)

##################################################################################################
## Project output configuration
##################################################################################################

#define outputs
set(YADOMS_UNITTEST_OUTPUT_DIR ${PROJECT_SOURCE_DIR}/../builds)

# First for the generic no-config case (e.g. with gcc)
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${YADOMS_UNITTEST_OUTPUT_DIR} )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${YADOMS_UNITTEST_OUTPUT_DIR}/plugins )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${YADOMS_UNITTEST_OUTPUT_DIR} )
# Second, for multi-config builds (e.g. msvc)
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${YADOMS_UNITTEST_OUTPUT_DIR}/${OUTPUTCONFIG} )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${YADOMS_UNITTEST_OUTPUT_DIR}/${OUTPUTCONFIG}/plugins )
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${YADOMS_UNITTEST_OUTPUT_DIR}/${OUTPUTCONFIG} )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )


##################################################################################################
## Externals libraries (Boost, Thread, RT) only linked with
##################################################################################################

# Boost (dynamic linking)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME ON)
add_definitions(-DBOOST_SPIRIT_THREADSAFE)
add_definitions(-DBOOST_TEST_DETECT_MEMORY_LEAK)
add_definitions(-DBOOST_TEST_DETECT_FP_EXCEPTIONS)


# define all needed boost libraries
set(BOOST_LIBS 
   atomic
   chrono
   date_time
   filesystem
   process
   regex
   thread
   unit_test_framework 
)

# search boost libs
find_package(Boost 1.89.0 CONFIG REQUIRED COMPONENTS ${BOOST_LIBS})
message("-- Found Boost: ${BOOST_LIBS}")

## thread library (pthread)
FIND_PACKAGE(Threads)

## RT Library
if(UNIX AND NOT APPLE)
    set(RT_LIBRARY rt)
endif()

## Poco dependencies
if(WIN32)
   set(LIBS ${LIBS} Iphlpapi.lib)
endif()

## IOKIT (needed for Mac)
if(APPLE)
	include(${YADOMS_PATH}/cmake/mac.cmake)
	
	if(NOT IOKIT_FOUND)
		message(SEND_ERROR "Unable to find the required IOKit libraries")
	endif()

	if(NOT CARBON_FOUND)
		message(SEND_ERROR "Unable to find the required Carbon libraries")
	endif()

	set(LIBS ${LIBS} ${IOKIT_LIBRARY} ${CARBON_LIBRARY})
endif()

###############################################
# OpenSSL
#
###############################################
SET(OPENSSL_USE_STATIC_LIBS TRUE)
SET(OPENSSL_MSVC_STATIC_RT TRUE)
FIND_PACKAGE(OpenSSL REQUIRED)

if(NOT ${OPENSSL_FOUND})
   message(FATAL_ERROR "Cannot find opensll, all plugins which requires SSL features will not be loaded")
   set(OPENSSL_INCLUDE_DIR "")
endif()
#Do not add OPENSSL_SSL_LIBRARY and OPENSSL_CRYPTO_LIBRARY here; it will be later
#if done here, this is too early, and for gcc the libraries appears in bad order regarding Poco for example

###############################################
# POCO
#
# POCO_LIBS : define all required poco libraries
###############################################

SET(POCO_COMPONENTS
   Util
   NetSSL
   Crypto
   Zip
   XML
   Net
   JSON
   Foundation
)

set(POCO_REQUIRE_MINIMUM_VERSION "1.7.3")
set(POCO_USE_STATIC_RUNTIME 1)
add_definitions(-DPOCO_STATIC)
add_definitions(-DPOCO_NO_AUTOMATIC_LIBS)
if(MSVC)
	#fix for linking with external OpenSSL
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}  /NODEFAULTLIB:libssl /NODEFAULTLIB:libcrypto")
endif(MSVC)

add_definitions(-DPOCO_EXTERNAL_OPENSSL=42) #force an unknown value (empty value do not work correctly)

find_package(Poco CONFIG REQUIRED COMPONENTS ${POCO_COMPONENTS})
message("-- Found Poco: ${POCO_COMPONENTS}")
set(Poco_TARGETS)
foreach(c IN LISTS POCO_COMPONENTS)
  list(APPEND Poco_TARGETS Poco::${c})
endforeach()



###############################################
# define libraries to link with
#
###############################################
set(LIBS
   ${Poco_TARGETS}
   ${OPENSSL_SSL_LIBRARY}
   ${OPENSSL_CRYPTO_LIBRARY}
   ${CMAKE_THREAD_LIBS_INIT}
   ${Boost_LIBRARIES}
   ${LIBS}
)

##################################################################################################
## Sources
##################################################################################################

# Yadoms Tests sources
set(YADOMS_TESTS_SRC
   stdafx.cpp
   stdafx.h

   testMain.cpp
   
   testCommon/fileSystem.h
   testCommon/fileSystem.cpp

   testCommon/serialTeleInfoMessage.h
   testCommon/serialTeleInfoMessage.cpp
)

##################################################################################################
## Tests selection to build
##################################################################################################
include(addSources.cmake)

add_subdirectory(external-libs)
add_subdirectory(mock)
add_subdirectory(plugins)
add_subdirectory(shared)
add_subdirectory(server)

source_group(plugins plugins/*.*)
source_group(plugins\\FakePlugin plugins/dev-FakePlugin/*.*)
source_group(plugins\\Rfxcom plugins/Rfxcom/*.*)
source_group(plugins\\EnOcean plugins/EnOcean/*.*)
source_group(plugins\\TeleInfo plugins/TeleInfo/*.*)
source_group(plugins\\Linky plugins/Linky/*.*)
source_group(plugins\\OrangeBusiness plugins/OrangeBusiness/*.*)

source_group(server server/*.*)
source_group(server\\automation server/automation/*.*)
source_group(server\\startupOptions server/startupOptions/*.*)
source_group(server\\tools server/tools/*.*)

source_group(shared shared/*.*)
source_group(shared\\event shared/event/*.*)
source_group(shared\\communication shared/communication/*.*)
source_group(shared\\exception shared/exception/*.*)
source_group(shared\\http shared/http/*.*)
source_group(shared\\http\\ssdp shared/http/ssdp/*.*)
source_group(shared\\plugin shared/plugin/*.*)
source_group(shared\\plugin\\configuration shared/plugin/configuration/*.*)
source_group(shared\\plugin\\information shared/plugin/information/*.*)
source_group(shared\\plugin\\yPluginApi shared/plugin/yPluginApi/*.*)
source_group(shared\\plugin\\yPluginApi\\historization shared/plugin/yPluginApi/historization/*.*)
source_group(shared\\serialization shared/serialization/*.*)
source_group(shared\\xpl shared/xpl/*.*)

source_group(mock mock/*.*)
source_group(mock\\server mock/server/*.*)
source_group(mock\\server\\pluginSystem mock/server/pluginSystem/*.*)
source_group(mock\\server\\pluginSystem\\information mock/server/pluginSystem/information/*.*)
source_group(mock\\shared mock/shared/*.*)
source_group(mock\\shared\\currentTime mock/shared/currentTime/*.*)

source_group(testCommon testCommon/*.*)

##################################################################################################
## Output
##################################################################################################
add_executable(yadomsTests ${YADOMS_TESTS_SRC})
add_precompiled_header(yadomsTests stdafx.h)

##################################################################################################
## Include directories
##################################################################################################

set(YADOMS_INCL_DIR
   ${YADOMS_INCL_DIR}
   ${YADOMS_PATH}/server
   ${YADOMS_PATH}/shared
   ${YADOMS_PATH}/server/tools
)

IF(WIN32)
   set(YADOMS_INCL_DIR ${YADOMS_INCL_DIR} ${YADOMS_PATH}/server/tools/windows)
   set(YADOMS_INCL_DIR ${YADOMS_INCL_DIR} ${YADOMS_PATH}/shared/windows)
ELSEIF(APPLE)
   set(YADOMS_INCL_DIR ${YADOMS_INCL_DIR} ${YADOMS_PATH}/server/tools/mac)
   set(YADOMS_INCL_DIR ${YADOMS_INCL_DIR} ${YADOMS_PATH}/shared/mac)
ELSE()
   set(YADOMS_INCL_DIR ${YADOMS_INCL_DIR} ${YADOMS_PATH}/server/tools/linux)
   set(YADOMS_INCL_DIR ${YADOMS_INCL_DIR} ${YADOMS_PATH}/shared/linux)
ENDIF()
# include OPENSSL_INCLUDE_DIR before YADOMS_INCL_DIR because YADOMS_INCL_DIR include postgresql which may embed its own openssl files (not used)
# but this may infer in some compilation cases
include_directories(
   ${BOOST_INCL_DIR}
   ${OPENSSL_INCLUDE_DIR}
   ${Poco_INCLUDE_DIRS}
   ${YADOMS_INCL_DIR}
   .
   mock
)

##################################################################################################
## Link
##################################################################################################
target_link_libraries(yadomsTests ${LIBS} ${CMAKE_DL_LIBS})