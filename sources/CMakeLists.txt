##################################################################################################
## CMakeList.txt for YADOMS
##
## This file generates projects/makefiles for the compete Yadoms software
##
## This file actually works for
##   Windows 7 x64, MSVC 12
##   Ubuntu 12.04, g++ 4.7
##   Raspberry Raspbian Wheezy, g++ 4.7
##   MacOSx Snow Leopard 10.8, g++ 4.8
##################################################################################################
project (Yadoms)

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
## Global CMake configuration
##################################################################################################

#suppress VS solution ZERO_CHECK project
if(MSVC)
	set(CMAKE_SUPPRESS_REGENERATION TRUE)
endif()


##################################################################################################
## IDE configurations
##################################################################################################

if(MSVC)
	#allow solution folders
	SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

	#global disable (for all targets)
	#disable warning C4996   Function call with parameters that may be unsafe 
	add_definitions( "/wd4996" )
	
	#to allow BOOST in precompiled header (specify Precompiled Header Memory Allocation Limit) 
	add_definitions("/Zm256")

	#add WIN32_WINNT preprocessor deifinition
	get_WIN32_WINNT(CURRENT_WINNT_VERSION)
	add_definitions(-D_WIN32_WINNT=${CURRENT_WINNT_VERSION})
endif()

if(CMAKE_COMPIER_IS_GNUCXX)
	add_definitions("-fpermissive")
	add_definitions("-std=c++0x")
endif()


##################################################################################################
## Project output configuration
##################################################################################################

#define outputs
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../builds/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../builds)


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
## Internal libraries (SQlite, Webem) comletly build from sources
##################################################################################################

#Use Sqlite database system
set(USE_SQLITE_DATABASE TRUE)
#Use Webem web server
set(USE_WEBEM_WEBSERVER TRUE)

#include libs (have their own CMakeList.txt)
add_subdirectory(external-libs)


##################################################################################################
## Yadoms
##################################################################################################


# Executable sources
set(YADOMS_SRC
	server/stdafx.cpp
	server/stdafx.h
	server/main.cpp
	server/Supervisor.cpp
	server/Supervisor.h
	server/pluginSystem/HardwarePluginManager.h
	server/pluginSystem/HardwarePluginManager.cpp
	server/pluginSystem/HardwarePluginInstance.h
	server/pluginSystem/HardwarePluginInstance.cpp
	server/pluginSystem/HardwarePluginFactory.h
	server/pluginSystem/HardwarePluginFactory.cpp
	server/pluginSystem/DirectoryChangeListener.h
	server/pluginSystem/DirectoryChangeListener.cpp
	plugins/hardware/IHardwarePlugin.h
	plugins/hardware/HardwarePluginConfiguration.hpp
	plugins/hardware/HardwarePluginConfigurationParameters.hpp
	server/StartupOptionsLoader.h
	server/StartupOptionsLoader.cpp
	server/StartupOptions.h
	server/ApplicationStopHandler.h
	server/ApplicationStopHandler.cpp
	
	server/database/IDataProvider.h
	server/database/IHardwareRequester.h
	server/database/IConfigurationRequester.h
	server/database/entities/Hardware.h
	server/database/entities/Configuration.h
	)

# Project folders
if(MSVC)
	source_group(Server server/*.*)
	source_group(Server\\pluginSystem server/pluginSystem/*.* )
	source_group(Server\\pluginSystem\\external plugins/hardware/IHardwarePlugin.h )
	source_group(Server\\database  server/database/*.*)
	source_group(Server\\database\\entities  server/database/entities/*.*)
endif()

#Database
IF(USE_SQLITE_DATABASE)
	set(YADOMS_SRC ${YADOMS_SRC}
		server/database/sqlite/SQLiteHardwareRequester.h
		server/database/sqlite/SQLiteHardwareRequester.cpp
		server/database/sqlite/SQLiteConfigurationRequester.h
		server/database/sqlite/SQLiteConfigurationRequester.cpp
		server/database/sqlite/SQLiteDataProvider.h
		server/database/sqlite/SQLiteDataProvider.cpp
		server/database/sqlite/SQLiteRequester.h
		server/database/sqlite/SQLiteRequester.cpp

		server/database/sqlite/adapters/ISQLiteResultAdapter.h
		server/database/sqlite/adapters/HardwareAdapter.h
		server/database/sqlite/adapters/HardwareAdapter.cpp
		server/database/sqlite/adapters/ConfigurationAdapter.h
		server/database/sqlite/adapters/ConfigurationAdapter.cpp
		server/database/sqlite/adapters/GenericAdapter.h
		server/database/sqlite/adapters/GenericAdapter.cpp
		server/database/sqlite/adapters/SingleValueAdapter.hpp
		server/database/sqlite/adapters/MultipleValueAdapter.hpp
		
		server/database/sqlite/versioning/ISQLiteVersionUpgrade.h
		server/database/sqlite/versioning/SQLiteVersion1.h
		server/database/sqlite/versioning/SQLiteVersion1.cpp
		server/database/sqlite/versioning/SQLiteVersionUpgraderFactory.h
		server/database/sqlite/versioning/SQLiteVersionUpgraderFactory.cpp
		server/database/sqlite/versioning/SQLiteVersionException.h
	)
	
	if(MSVC)
		source_group(Server\\database\\sqlite  server/database/sqlite/*.*)
		source_group(Server\\database\\sqlite\\adapters  server/database/sqlite/adapters/*.*)
		source_group(Server\\database\\sqlite\\versioning  server/database/sqlite/versioning/*.*)
	endif()
	
	set(LIBS ${LIBS} sqlite)
	set(YADOMS_INCL_DIR ${YADOMS_INCL_DIR} ${SQLITE_INCLUDE_DIRECTORY})
ENDIF()

#Web server
IF(USE_WEBEM_WEBSERVER)
	set(LIBS ${LIBS} webem)
	set(YADOMS_INCL_DIR ${YADOMS_INCL_DIR} ${WEBEM_INCLUDE_DIRECTORY})
ENDIF()

#define YADOMS output
add_executable(yadoms ${YADOMS_SRC})
add_precompiled_header(yadoms stdafx.h)


##################################################################################################
## yadoms-tools
##################################################################################################

# Library Sources
set(SHARED_LIB_SRC
	tools/tools/stdafx.cpp
	tools/tools/stdafx.h

	tools/tools/ThreadBase.cpp
	tools/tools/ThreadBase.h

	tools/tools/Log.h
	tools/tools/Log.cpp
	tools/tools/StringExtension.hpp	
	tools/tools/Exceptions/Exception.h
	tools/tools/Exceptions/Exception.cpp
	tools/tools/Exceptions/NotImplementedException.h
	tools/tools/Exceptions/NullReferenceException.h
	tools/tools/Exceptions/NullReferenceException.cpp

	tools/tools/dir_monitor/dir_monitor.hpp
	tools/tools/dir_monitor/basic_dir_monitor.hpp
	
	#include both platform files to see them into IDE
	tools/windows/tools/DynamicLibrary.hpp
	tools/windows/tools/Export.h
	tools/windows/tools/dir_monitor/basic_dir_monitor_service.hpp
	tools/windows/tools/dir_monitor/dir_monitor_impl.hpp

	tools/linux/tools/DynamicLibrary.hpp
	tools/linux/tools/Export.h
	tools/linux/tools/dir_monitor/basic_dir_monitor_service.hpp
	tools/linux/tools/dir_monitor/dir_monitor_impl.hpp
)

# Define project folders
IF(MSVC)
	source_group(PlatformSpecific\\windows tools/windows/tools/*.*)
	source_group(PlatformSpecific\\linux tools/linux/tools/*.*)
	source_group(Exception tools/tools/Exceptions/*.* )	
ENDIF()

# Define library outputs
add_library(yadoms-tools STATIC  ${SHARED_LIB_SRC})
add_precompiled_header(yadoms-tools stdafx.h)
target_link_libraries(yadoms-tools ${LIBS} ${CMAKE_DL_LIBS})

# Define include directories (allow easy includes)
set(TOOLS_INCL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tools)
IF(WIN32)
	set(TOOLS_INCL_DIR ${TOOLS_INCL_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/tools/windows)
ELSE()
	set(TOOLS_INCL_DIR ${TOOLS_INCL_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/tools/linux)
	set(TOOLS_INCL_DIR ${TOOLS_INCL_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/tools/tools)
ENDIF()

set_property( TARGET yadoms-tools PROPERTY INCLUDE_DIRECTORIES ${TOOLS_INCL_DIR} ${BOOST_INCL_DIR})




##################################################################################################
## Plugins
##################################################################################################

#include Plugin CMake file
add_subdirectory(plugins)


##################################################################################################
## Include directories
##################################################################################################
set(YADOMS_INCL_DIR
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/server
	${YADOMS_INCL_DIR}
	${TOOLS_INCL_DIR}
)
set_property( TARGET yadoms PROPERTY INCLUDE_DIRECTORIES ${YADOMS_INCL_DIR} ${BOOST_INCL_DIR})

##################################################################################################
## Link
##################################################################################################
target_link_libraries(yadoms yadoms-tools ${LIBS} ${CMAKE_DL_LIBS})


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
			add_custom_command(TARGET yadoms POST_BUILD        	# Adds a post-build event to yadoms
				COMMAND ${CMAKE_COMMAND} -E copy_if_different  	# which executes "cmake - E copy_if_different..."
					"${LIBFILE}"     							# <--this is in-file
					$<TARGET_FILE_DIR:yadoms>)   				# <--this is out-file path
		else()
			MESSAGE("    Library file not found : ${LIBFILE}")
		endif()
	ENDFOREACH(BOOSTLIBRARY)
endif()		
		

