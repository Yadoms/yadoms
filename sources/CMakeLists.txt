# Project Stuff
cmake_minimum_required (VERSION 2.6)
project (Yadoms)

##################################################################################################
## Debug makefile
##################################################################################################
if(CMAKE_COMPIER_IS_GNUCXX)
	set(CMAKE_VERBOSE_MAKEFILE on)
endif()

##################################################################################################
## Global properties
##################################################################################################

#include precompiled headers
include("PrecompiledHeader.cmake")

#include user specific configuration
include("CMakeListsUserConfig.txt" OPTIONAL)

# Default Options
if(UNIX AND NOT APPLE)
	add_definitions("-std=c++0x")
endif()

#suppress VS solution ZERO_CHECK project
set(CMAKE_SUPPRESS_REGENERATION TRUE)

#define outputs
include_directories(${PROJECT_SOURCE_DIR})
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../builds/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../builds)


##################################################################################################
## Externals libraries
##################################################################################################

## Boost (dynamic linking)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
add_definitions( -DBOOST_WHATEVER_DYN_LINK )
add_definitions( -DBOOST_ALL_NO_LIB )
add_definitions( -DBOOST_ALL_DYN_LINK )
	
if(UNIX AND NOT APPLE)
	set(Boost_USE_STATIC_RUNTIME OFF)
endif()

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
find_package(Boost COMPONENTS ${BOOST_LIBS} REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${BOOST_ROOT})

FIND_PACKAGE(Threads)

## RT Library
if(UNIX AND NOT APPLE)
    set(RT_LIBRARY rt)
endif()

set(LIBS ${LIBS} ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${RT_LIBRARY})

#Sqlite,...
set(USE_SQLITE_DATABASE TRUE)
add_subdirectory(external-libs)

##################################################################################################
## yadoms-tools
##################################################################################################

# Library Sources
set(SHARED_LIB_SRC
	tools/tools/stdafx.cpp
	tools/tools/stdafx.h

	tools/tools/ThreadBase.cpp
	tools/tools/ThreadBase.h

	tools/tools/Exceptions/Exception.h
	tools/tools/Exceptions/Exception.cpp
	tools/tools/Exceptions/NotImplementedException.h
	tools/tools/Exceptions/NullReferenceException.h
	tools/tools/Exceptions/NullReferenceException.cpp
	
	#include both platform files to see them into IDE
	tools/windows/tools/DynamicLibrary.hpp
	tools/windows/tools/Export.h
	tools/windows/tools/StringExtension.hpp
	tools/linux/tools/DynamicLibrary.hpp
	tools/linux/tools/Export.h
	tools/linux/tools/StringExtension.hpp
)

# Define project folders
IF(MSVC)
	source_group(PlatformSpecific\\windows tools/windows/tools/*.*)
	source_group(PlatformSpecific\\linux tools/linux/tools/*.*)
	source_group(Exception tools/tools/Exceptions/*.* )	
ENDIF()

# Define library outputs
add_library(yadoms-tools STATIC  ${SHARED_LIB_SRC})
#add_precompiled_header(yadoms-tools tools/tools/stdafx.h)
target_link_libraries(yadoms-tools ${LIBS} ${CMAKE_DL_LIBS})

# Define include directories (allow easy includes)

IF(WIN32)
	INCLUDE_DIRECTORIES(tools/windows)
ELSE()
	INCLUDE_DIRECTORIES(tools/linux)
	INCLUDE_DIRECTORIES(tools/tools)
ENDIF()
INCLUDE_DIRECTORIES(tools)

##################################################################################################
## Plugins
##################################################################################################

#include Plugin CMake file
add_subdirectory(plugins)


##################################################################################################
## Yadoms
##################################################################################################

INCLUDE_DIRECTORIES(server)

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
	plugins/hardware/IHardwarePlugin.h
	plugins/hardware/HardwarePluginConfiguration.hpp
	plugins/hardware/HardwarePluginConfigurationParameters.hpp
	server/StartupOptionsLoader.h
	server/StartupOptionsLoader.cpp
	server/StartupOptions.h
	server/ApplicationStopHandler.h
	server/ApplicationStopHandler.cpp
	server/Log.h
	
	server/database/IDataProvider.h
	server/database/IHardwareRequester.h
	server/database/entities/Hardware.h
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
		server/database/sqlite/SQLiteDataProvider.h
		server/database/sqlite/SQLiteDataProvider.cpp
		
		server/database/sqlite/adapters/ISQLiteResultAdapter.h
		server/database/sqlite/adapters/HardwareAdapter.h
		server/database/sqlite/adapters/HardwareAdapter.cpp
		server/database/sqlite/adapters/GenericAdapter.h
		server/database/sqlite/adapters/GenericAdapter.cpp
		server/database/sqlite/adapters/SingleValueAdapter.hpp
		server/database/sqlite/adapters/MultipleValueAdapter.hpp
	)
	
	set(LIBS ${LIBS} sqlite)
	
	if(MSVC)
		source_group(Server\\database\\sqlite  server/database/sqlite/*.*)
		source_group(Server\\database\\sqlite\\adapters  server/database/sqlite/adapters/*.*)
	endif()
	
	INCLUDE_DIRECTORIES(${SQLITE_INCLUDE_DIRECTORY})
ENDIF()

# Define executables output
add_executable(yadoms ${YADOMS_SRC})
add_precompiled_header(yadoms stdafx.h)
target_link_libraries(yadoms yadoms-tools ${LIBS} ${CMAKE_DL_LIBS})

##################################################################################################
## IDE configurations
##################################################################################################

if(MSVC)
	#allow solution folders
	SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

	#global disable (for all targets)
	#disable warning C4996   Function call with parameters that may be unsafe 
	add_definitions( "/wd4996" )
	
	add_definitions("/Zm256")
endif()

if(CMAKE_COMPIER_IS_GNUCXX)
	add_definitions("-fpermissive")
endif()
