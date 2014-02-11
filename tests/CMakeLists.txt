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

##################################################################################################
## Check for minimum version
##################################################################################################

# Check the minimum CMake version
cmake_minimum_required (VERSION 2.8.9)

# Check for minimum GCC version
MESSAGE(STATUS "Compiler = ${CMAKE_CXX_COMPILER_ID}")
MESSAGE(STATUS "Compiler Version = ${CMAKE_CXX_COMPILER_VERSION}")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # require at least gcc 4.5 (for boost::log)
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.5)
        message(FATAL_ERROR "GCC version must be at least 4.5!")
    endif()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    # require at least clang 3.2
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 3.2)
        message(FATAL_ERROR "Clang version must be at least 3.2!")
    endif()
endif()


##################################################################################################
## Debug makefile
##################################################################################################
if(CMAKE_COMPIER_IS_GNUCXX)
	set(CMAKE_VERBOSE_MAKEFILE on)
endif()

##################################################################################################
## Include other cmake scripts
##################################################################################################

#include precompiled headers
include("PrecompiledHeader.cmake")

#include user specific configuration
include("CMakeListsUserConfig.txt" OPTIONAL)

#include Windows specific macro
if(MSVC)
	include("windows.cmake")
endif()

##################################################################################################
## IDE configurations
##################################################################################################

if(MSVC)
	#allow solution folders
	SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

	#global disable (for all targets)
	#disable warning C4996   Function call with parameters that may be unsafe 
	add_definitions( "/wd4996 /wd4251" )

	#######################################################
	## Activate warning level4, and disable useless ones ##
	#######################################################
	add_definitions("/W4")
	
	# A lot of level 4 warnings are involved by boost.
	# Use the next link to see how to remove warning 
	#https://svn.boost.org/trac/boost/wiki/Guidelines/WarningsGuidelines
	
	#disable warning C4512: l'opérateur d'assignation n'a pas pu être généré
	add_definitions( "/wd4512" )
	
	#disable warning C4610: struct '' ne peut jamais être instancié(e) : résolution : la même que pour C4512
	add_definitions( "/wd4610" )
	
	#disable warning C4127: l'expression conditionnelle est une constante
	add_definitions( "/wd4127" )

	#disable warning C4100: unreferenced formal parameter
	add_definitions( "/wd4100" )
	
	#disable warning C4714: fonction '' marquée comme __forceinline non inline
	add_definitions( "/wd4714" )
	
	#disable warning C4510: le constructeur par défaut n'a pas pu être généré
	add_definitions( "/wd4510" )

	#disable warning C4505: la fonction locale non référencée a été supprimée : résolution : supprimer le code mort
	add_definitions( "/wd4505" )
	
	#to allow BOOST in precompiled header (specify Precompiled Header Memory Allocation Limit) 
	add_definitions("/Zm256")

	#add WIN32_WINNT preprocessor deifinition
	get_WIN32_WINNT(CURRENT_WINNT_VERSION)
	add_definitions(-D_WIN32_WINNT=${CURRENT_WINNT_VERSION})
endif()

if(CMAKE_COMPIER_IS_GNUCXX)
	add_definitions("-Wall -Wextra -pedantic")
	add_definitions("-fpermissive")
	add_definitions("-std=c++0x")
	
	# In case unix is 64bits, append -fPIC to gcc
	IF(UNIX AND NOT WIN32)
	   FIND_PROGRAM(CMAKE_UNAME uname /bin /usr/bin /usr/local/bin )
	   IF(CMAKE_UNAME)
		 EXEC_PROGRAM(uname ARGS -m OUTPUT_VARIABLE CMAKE_SYSTEM_PROCESSOR)
		 SET(CMAKE_SYSTEM_PROCESSOR ${CMAKE_SYSTEM_PROCESSOR} CACHE INTERNAL "processor type (i386 and x86_64)")
		 IF(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
		   add_definitions(-fPIC)
		 ENDIF(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
	   ENDIF(CMAKE_UNAME)
	ENDIF(UNIX AND NOT WIN32)
endif()


if (CMAKE_COMPIER_IS_CLANG)
	add_definitions("-Wall -Wextra -pedantic")
	add_definitions("-std=c++0x")
   add_definitions("-Woverloaded-virtual")
   add_definitions("-Wunused-variable")
endif()

#check eclispe debug options
if(DEBUG_WITH_ECLIPSE)
	add_definitions(-DDEBUG)
	add_definitions("-g")
endif()





##################################################################################################
## Project output configuration
##################################################################################################

#define outputs
set(youroutputdirectory ${PROJECT_SOURCE_DIR}/builds)

# First for the generic no-config case (e.g. with gcc)
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${youroutputdirectory} )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${youroutputdirectory}/plugins/hardware )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${youroutputdirectory} )
# Second, for multi-config builds (e.g. msvc)
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${youroutputdirectory}/${OUTPUTCONFIG} )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${youroutputdirectory}/${OUTPUTCONFIG}/plugins/hardware )
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
set(BOOST_INCL_DIR ${YADOMS_INCL_DIR} ${Boost_INCLUDE_DIR} ${BOOST_ROOT})

## thread library (pthread)
FIND_PACKAGE(Threads)

## RT Library
if(UNIX AND NOT APPLE)
    set(RT_LIBRARY rt)
endif()

# define libraries to link with
set(LIBS ${LIBS} ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${RT_LIBRARY})

##################################################################################################
## Tests selection to build
##################################################################################################

# TODO déplacer dans le CMakeListsUserConfig.txt (ou plutôt permettre la désactivation à partir de ce fichier)
set(USE_TESTS_START_OPTIONS_LOADER TRUE)
set(USE_TESTS_HARDWARE_PLUGIN_MANAGER FALSE)
set(USE_TESTS_SHARED_EVENT TRUE)


##################################################################################################
## Include directories
##################################################################################################

set(SHARED_INCL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../sources/shared)
IF(WIN32)
	set(SHARED_INCL_DIR ${SHARED_INCL_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../sources/shared/windows)
ELSEIF(APPLE)
	set(SHARED_INCL_DIR ${SHARED_INCL_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../sources/shared/mac)
ELSE()
	set(SHARED_INCL_DIR ${SHARED_INCL_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../sources/shared/linux)
ENDIF()


set(YADOMS_INCL_DIR
	${CMAKE_CURRENT_SOURCE_DIR}/../sources/server
	${CMAKE_CURRENT_SOURCE_DIR}/../sources/tools
	${SHARED_INCL_DIR}
)


##################################################################################################
## Tests on Yadoms classes
##################################################################################################

#TODO : mettre en commun des trucs par macro/fonction

source_group(Yadoms server/*.*)


IF (USE_TESTS_START_OPTIONS_LOADER)
   set(YADOMS_TESTS_STARTUP_OPTIONS_LOADER_SRC
   
   ../sources/server/stdafx.cpp
   ../sources/server/stdafx.h
  
	../sources/server/StartupOptions/Loader.h
	../sources/server/StartupOptions/Loader.cpp
	../sources/server/StartupOptions/InvalidOptionException.hpp
	../sources/server/StartupOptions/LoaderCustomValidators.hpp
	../sources/server/StartupOptions/LoaderException.hpp
	../sources/server/StartupOptions/MustExistPathOption.hpp
	../sources/server/StartupOptions/StartupOptions.h
	../sources/server/StartupOptions/ValidIpAddressOption.hpp

   ../tests/server/MainTestStartOptionsLoader.cpp
   
   )
   add_executable(MainTestStartOptionsLoader ${YADOMS_TESTS_STARTUP_OPTIONS_LOADER_SRC})
   add_precompiled_header(MainTestStartOptionsLoader stdafx.h)
   set_property( TARGET MainTestStartOptionsLoader PROPERTY INCLUDE_DIRECTORIES ${YADOMS_INCL_DIR} ${SHARED_INCL_DIR} ${BOOST_INCL_DIR})
   target_link_libraries(MainTestStartOptionsLoader ${LIBS} ${CMAKE_DL_LIBS})
ENDIF()

IF (USE_TESTS_HARDWARE_PLUGIN_MANAGER)
   set(YADOMS_TESTS_HARDWARE_PLUGIN_MANAGER

   ../sources/server/stdafx.cpp
   ../sources/server/stdafx.h
  
#	../sources/server/pluginSystem/HardwarePluginManager.h
#	../sources/server/pluginSystem/HardwarePluginManager.cpp
#       ../sources/server/pluginSystem/HardwarePluginFactory.h
#        ../sources/server/pluginSystem/HardwarePluginFactory.cpp
#        ../sources/plugins/hardware/IHardwarePlugin.h
#        ../sources/server/database/entities/Hardware.h

#	../sources/server/StartupOptions/Loader.h
#	../sources/server/StartupOptions/Loader.cpp
#	../sources/server/StartupOptions/InvalidOptionException.hpp
#	../sources/server/StartupOptions/LoaderCustomValidators.hpp
#	../sources/server/StartupOptions/LoaderException.hpp
#	../sources/server/StartupOptions/MustExistPathOption.hpp
#	../sources/server/StartupOptions/StartupOptions.h
#	../sources/server/StartupOptions/ValidIpAddressOption.hpp

#        ../tests/server/pluginSystem/MainTestHardwarePluginManager.cpp

   )
   add_executable(MainTestHardwarePluginManager ${YADOMS_TESTS_HARDWARE_PLUGIN_MANAGER} ${YADOMS_SRC})
   add_precompiled_header(MainTestHardwarePluginManager stdafx.h)
   set_property( TARGET MainTestHardwarePluginManager PROPERTY INCLUDE_DIRECTORIES ${YADOMS_INCL_DIR} ${SHARED_INCL_DIR} ${BOOST_INCL_DIR})
   target_link_libraries(MainTestHardwarePluginManager ${LIBS} ${CMAKE_DL_LIBS})
ENDIF()



##################################################################################################
## Tests on yadoms-shared classes
##################################################################################################

if(MSVC)
  # Define yadoms_shared_EXPORTS variable to simulate shared library building
  add_definitions(-Dyadoms_shared_EXPORTS)
endif()

source_group(yadoms-shared shared/*.*)

IF (USE_TESTS_SHARED_EVENT)
   set(YADOMS_TESTS_SHARED_EVENT

   ../sources/shared/stdafx.cpp
   ../sources/shared/stdafx.h
   
   
   ../sources/shared/shared/StringExtension.h
   ../sources/shared/shared/StringExtension.cpp
   
   ../sources/shared/shared/Exceptions/BadConversionException.hpp
   
   ../sources/shared/shared/Event/Event.hpp
   ../sources/shared/shared/Event/EventBase.hpp
   ../sources/shared/shared/Event/EventHandler.hpp
   
   ../sources/shared/shared/Exceptions/BadConversionException.hpp
   ../sources/shared/shared/Exceptions/EmptyResultException.hpp
   ../sources/shared/shared/Exceptions/Exception.hpp
   ../sources/shared/shared/Exceptions/InvalidParameterException.hpp

	shared/MainTestEvent.cpp

	)
   add_executable(MainTestEvent ${YADOMS_TESTS_SHARED_EVENT} ${YADOMS_SRC})
   add_precompiled_header(MainTestEvent stdafx.h)
   set_property( TARGET MainTestEvent PROPERTY INCLUDE_DIRECTORIES ${SHARED_INCL_DIR} ${BOOST_INCL_DIR})
	target_link_libraries(MainTestEvent ${LIBS} ${CMAKE_DL_LIBS})
ENDIF()

	
##################################################################################################
## Tests on plugins classes
##################################################################################################

# Not test on plugins for the moment


##################################################################################################
## Post build commands
##################################################################################################

#create lib lists, one for debug libs and another for optimized (release)
# Boost_LIBRARIES is a list whchi contains something like this:
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
if(NOT Boost_USE_STATIC_LIBS)
	MESSAGE(STATUS "Add boost libraries to be copied as postbuild")
	#todo : choose debug/optimized mode
	FOREACH (BOOSTLIBRARY ${BOOST_LIBS_DEBUG})

		#construct the dll/so fil path
		get_filename_component(UTF_BASE_NAME ${BOOSTLIBRARY} NAME_WE)
		get_filename_component(UTF_PATH ${BOOSTLIBRARY} PATH)
	
		if(MSVC)
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
			add_custom_command(TARGET MainTestStartOptionsLoader POST_BUILD        	# Adds a post-build event to yadoms
				COMMAND ${CMAKE_COMMAND} -E copy_if_different  	# which executes "cmake - E copy_if_different..."
					"${LIBFILE}"     							# <--this is in-file
					$<TARGET_FILE_DIR:MainTestStartOptionsLoader>)   				# <--this is out-file path
			add_custom_command(TARGET MainTestHardwarePluginManager POST_BUILD        	# Adds a post-build event to yadoms
				COMMAND ${CMAKE_COMMAND} -E copy_if_different  	# which executes "cmake - E copy_if_different..."
					"${LIBFILE}"     							# <--this is in-file
					$<TARGET_FILE_DIR:MainTestHardwarePluginManager>)   

			add_custom_command(TARGET MainTestEvent POST_BUILD        	# Adds a post-build event to yadoms
				COMMAND ${CMAKE_COMMAND} -E copy_if_different  	# which executes "cmake - E copy_if_different..."
					"${LIBFILE}"     							# <--this is in-file
					$<TARGET_FILE_DIR:MainTestEvent>)   
		else()
			MESSAGE("    Library file not found : ${LIBFILE}")
		endif()
	ENDFOREACH(BOOSTLIBRARY)
endif()		


