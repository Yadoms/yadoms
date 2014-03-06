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
cmake_minimum_required (VERSION 2.8.9)

##################################################################################################
## Include other cmake scripts
##################################################################################################

# First checks (compilers versions...)
include("../sources/preChecks.cmake")

# Precompiled headers
include("PrecompiledHeader.cmake")

# User specific configuration
include("CMakeListsUserConfig.txt" OPTIONAL)

# Tests Common Definitions
include("YadomsTestsCommon.cmake")

# Windows specific macro
if(WIN32)
	include("windows.cmake")
endif()

# Compilers/IDE configurations
include("../sources/compilers.cmake")


add_definitions( -DBOOST_AUTO_TEST_MAIN )



##################################################################################################
## Project output configuration
##################################################################################################

#TODO : Changer la destination de chaque test, puis les exécuter à l'endroit où ils sont. Cela permettra d'avoir des tests avec les mêmes noms à des endroits différents.

#define outputs
set(youroutputdirectory ${PROJECT_SOURCE_DIR}/builds)

# First for the generic no-config case (e.g. with gcc)
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${youroutputdirectory} )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${youroutputdirectory}/plugins )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${youroutputdirectory} )
# Second, for multi-config builds (e.g. msvc)
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${youroutputdirectory}/${OUTPUTCONFIG} )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${youroutputdirectory}/${OUTPUTCONFIG}/plugins )
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${youroutputdirectory}/${OUTPUTCONFIG} )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

##################################################################################################
## Externals libraries (Boost, Thread, RT) only linked with
##################################################################################################

# Boost (dynamic linking)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
add_definitions( -DBOOST_WHATEVER_DYN_LINK )
add_definitions( -DBOOST_ALL_NO_LIB )
add_definitions( -DBOOST_ALL_DYN_LINK )
add_definitions( -DBOOST_SPIRIT_THREADSAFE )

# Boost tests units definitions
add_definitions( -DBOOST_AUTO_TEST_MAIN )

# define all needed boost libraries
set(BOOST_LIBS 
	system 
	filesystem 
	thread 
	date_time 
	chrono 
	log 
	log_setup 
	program_options
  unit_test_framework system
	regex
)

# search boost libs
find_package(Boost COMPONENTS ${BOOST_LIBS} REQUIRED)
set(BOOST_INCL_DIR ${Boost_INCLUDE_DIR} ${BOOST_ROOT})

## thread library (pthread)
FIND_PACKAGE(Threads)

## RT Library
if(UNIX AND NOT APPLE)
    set(RT_LIBRARY rt)
endif()

## IOKIT (needed for Mac)
if(APPLE)
	include("mac.cmake")
	
	if(NOT IOKIT_FOUND)
		message(SEND_ERROR "Unable to find the requested IOKit libraries")
	endif()
	set(LIBS ${LIBS} ${IOKIT_LIBRARY})
endif()

# define libraries to link with
set(LIBS ${LIBS} ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${RT_LIBRARY})


##################################################################################################
## Include directories
##################################################################################################

set(SHARED_INCL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../sources/shared)
IF(WIN32)
	set(SHARED_PF_INCL_DIR ${SHARED_INCL_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../sources/shared/windows)
ELSEIF(APPLE)
	set(SHARED_PF_INCL_DIR ${SHARED_INCL_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../sources/shared/mac)
ELSE()
	set(SHARED_PF_INCL_DIR ${SHARED_INCL_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../sources/shared/linux)
ENDIF()

set(TOOLS_INCL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../sources/server/tools)
IF(WIN32)
	set(TOOLS_PF_INCL_DIR ${TOOLS_INCL_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../sources/server/tools/windows)
ELSEIF(APPLE)
	set(TOOLS_PF_INCL_DIR ${TOOLS_INCL_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../sources/server/tools/mac)
ELSE()
	set(TOOLS_PF_INCL_DIR ${TOOLS_INCL_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../sources/server/tools/linux)
ENDIF()


##################################################################################################
## Mocks
##################################################################################################

set(MOCK_SHARED_INCL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/mock/shared)
set(MOCK_INCL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/mock)


##################################################################################################
## Post build commands
##################################################################################################

#create lib lists, one for debug libs and another for optimized (release)
# Boost_LIBRARIES is a list which contains something like this:
#
#	debug							(<- define that next list entry if for debug mode)
#	path/to/debug/boost-...gd.lib)
#	optimize                        (<- define that next list entry if for release mode)
#	path/to/debug/boost-...lib)
#	...
#
# In some configurations, the list contains only lib file path without (debug/optimize)
# So if debug and optimize or not found, then just add library to debug or optimized list
#
set(TYPEISKNOWN OFF)
set(TYPEISDEBUG OFF)
FOREACH (BOOSTLIBRARY ${Boost_LIBRARIES})
	if(TYPEISKNOWN)
		if(TYPEISDEBUG)
			list(APPEND BOOST_LIBS_DEBUG ${BOOSTLIBRARY})
		else()
			list(APPEND BOOST_LIBS_OPTIMIZED ${BOOSTLIBRARY})
		endif()
		set(TYPEISKNOWN OFF)
	else()
		if(${BOOSTLIBRARY} MATCHES "debug") 
			set(TYPEISKNOWN ON)
			set(TYPEISDEBUG ON)
		elseif(${BOOSTLIBRARY} MATCHES "optimized") 
			set(TYPEISKNOWN ON)
			set(TYPEISDEBUG OFF)
		else()
			list(APPEND BOOST_LIBS_DEBUG ${BOOSTLIBRARY})
		endif()
	endif()
ENDFOREACH(BOOSTLIBRARY)


#Take each boost lib file, find its dll and add it to a post build command
MACRO(COPY_BOOST_LIBS _targetName)
   if(NOT Boost_USE_STATIC_LIBS)
      MESSAGE(STATUS "Add boost libraries to be copied as postbuild")
      #todo : choose debug/optimized mode
      FOREACH (BOOSTLIBRARY ${BOOST_LIBS_DEBUG})

         #construct the dll/so fil path
         get_filename_component(UTF_BASE_NAME ${BOOSTLIBRARY} NAME_WE)
         get_filename_component(UTF_PATH ${BOOSTLIBRARY} PATH)
      
         if(WIN32)
            #windows
            set(LIBFILE ${UTF_PATH}/${UTF_BASE_NAME}.dll)
         elseif(APPLE)
            #mac
            set(LIBFILE ${UTF_PATH}/${UTF_BASE_NAME}.dylib)
         else()
            #linux
            set(LIBFILE ${UTF_PATH}/${UTF_BASE_NAME}.so)
         endif()

         #check existance
         if(EXISTS ${LIBFILE})
            MESSAGE(STATUS "    Library file : ${LIBFILE}")
               
            #create post build command
            add_custom_command(TARGET ${_targetName} POST_BUILD        	# Adds a post-build event to target
               COMMAND ${CMAKE_COMMAND} -E copy_if_different  	# which executes "cmake - E copy_if_different..."
                  "${LIBFILE}"     							# <--this is in-file
                  $<TARGET_FILE_DIR:${_targetName}>)   				# <--this is out-file path
         else()
            MESSAGE("    Library file not found : ${LIBFILE}")
         endif()		
      ENDFOREACH(BOOSTLIBRARY)
   endif()		
ENDMACRO()

##################################################################################################
## Tests selection to build
##################################################################################################

add_subdirectory(server)
add_subdirectory(shared)


