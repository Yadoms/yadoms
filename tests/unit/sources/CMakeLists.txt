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
project (YadomsTests)

# Check the minimum CMake version
cmake_minimum_required (VERSION 3.2.0)

# The main Yadoms path
set(YADOMS_PATH ../../../sources)

include("${CMAKE_SOURCE_DIR}/../../../projects/conanbuildinfo_multi.cmake")
conan_basic_setup(TARGETS)

##################################################################################################
## Include other cmake scripts
##################################################################################################

# First checks (compilers versions...)
include(${YADOMS_PATH}/cmake/preChecks.cmake)

# User specific configuration
include("CMakeListsUserConfig.txt" OPTIONAL)

# Precompiled headers
include(${YADOMS_PATH}/cmake/PrecompiledHeader.cmake)

# Windows specific macro
if(WIN32)
	include(${YADOMS_PATH}/cmake/windows.cmake)
endif()

if(MSVC)
   set (LINK_WITH_STATIC_RUNTIME 1)
   include(${YADOMS_PATH}/cmake/MSVCRuntime.cmake)
endif()

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
add_definitions(-DBOOST_SPIRIT_THREADSAFE)

## thread library (pthread)
FIND_PACKAGE(Threads)

## RT Library
if(UNIX AND NOT APPLE)
    set(RT_LIBRARY rt)
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


# define libraries to link with
set(LIBS ${LIBS} ${CMAKE_THREAD_LIBS_INIT} CONAN_PKG::boost CONAN_PKG::Poco CONAN_PKG::OpenSSL CONAN_PKG::protobuf)

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

set(YADOMS_INCL_DIR)

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
source_group(shared\\plugin shared/plugin/*.*)
source_group(shared\\plugin\\configuration shared/plugin/configuration/*.*)
source_group(shared\\plugin\\information shared/plugin/information/*.*)
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
include_directories(${BOOST_INCL_DIR} . mock ${YADOMS_PATH}/server ${YADOMS_PATH}/shared ${YADOMS_PATH}/server/tools ${Poco_INCLUDE_DIRS})

IF(WIN32)
   include_directories(${YADOMS_PATH}/server/tools/windows)
   include_directories(${YADOMS_PATH}/shared/windows)
ELSEIF(APPLE)
   include_directories(${YADOMS_PATH}/server/tools/mac)
   include_directories(${YADOMS_PATH}/shared/mac)
ELSE()
   include_directories(${YADOMS_PATH}/server/tools/linux)
   include_directories(${YADOMS_PATH}/shared/linux)
ENDIF()

include_directories(${YADOMS_INCL_DIR})

##################################################################################################
## Link
##################################################################################################
target_link_libraries(yadomsTests ${LIBS} ${CMAKE_DL_LIBS})

#due to conan architecture, debug pdb are not always available
set_target_properties(yadomsTests PROPERTIES LINK_FLAGS "/ignore:4099")
