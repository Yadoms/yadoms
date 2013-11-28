# Project Stuff
cmake_minimum_required (VERSION 2.6)
project (Yadoms)

##################################################################################################
## Global properties
##################################################################################################

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

## Boost
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)

set(BOOST_LIBS 
	thread 
	date_time 
	system 
	chrono 
	log 
	filesystem 
	log_setup 
	program_options
)
find_package(Boost COMPONENTS ${BOOST_LIBS} REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

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
	tools/tools/ThreadBase.cpp
	tools/tools/ThreadBase.h

	tools/tools/Exceptions/Exception.h
	tools/tools/Exceptions/Exception.cpp
	tools/tools/Exceptions/NotImplementedException.h
	tools/tools/Exceptions/NullReferenceException.h
	tools/tools/Exceptions/NullReferenceException.cpp
	tools/tools/StringExtension.hpp
	
	#include both platform files to see them into IDE
	tools/windows/tools/DynamicLibrary.hpp
	tools/windows/tools/Export.h
	tools/linux/tools/DynamicLibrary.hpp
	tools/linux/tools/Export.h
)

# Define project folders
IF(MSVC)
	source_group(PlatformSpecific\\windows tools/windows/tools/*.*)
	source_group(PlatformSpecific\\linux tools/linux/tools/*.*)
	source_group(Exception tools/tools/Exceptions/*.* )	
ENDIF()

# Define library outputs
add_library(yadoms-tools STATIC  ${SHARED_LIB_SRC})
target_link_libraries(yadoms-tools ${LIBS} ${CMAKE_DL_LIBS})

# Define include directories (allow easy includes)
INCLUDE_DIRECTORIES(tools)
IF(WIN32)
	INCLUDE_DIRECTORIES(tools/windows)
ELSE()
	INCLUDE_DIRECTORIES(tools/linux)
ENDIF()


##################################################################################################
## Plugins
##################################################################################################

#include Plugin CMake file
add_subdirectory(plugins)


##################################################################################################
## Yadoms
##################################################################################################

# Executable sources
set(YADOMS_SRC
	server/main.cpp
	server/Supervisor.cpp
	server/Supervisor.h
	server/pluginSystem/HardwarePluginManager.h
	server/pluginSystem/HardwarePluginManager.cpp
	server/pluginSystem/HardwarePluginInstance.h
	server/pluginSystem/HardwarePluginInstance.cpp
	server/pluginSystem/HardwarePluginFactory.h
	server/pluginSystem/HardwarePluginFactory.cpp
	server/pluginSystem/HardwarePluginConfigurationProviderImplementation.h
	server/pluginSystem/HardwarePluginConfigurationProviderImplementation.cpp
	plugins/hardware/IHardwarePlugin.h
	server/StartupOptionsLoader.h
	server/StartupOptionsLoader.cpp
	server/StartupOptions.h
	server/ApplicationStopHandler.h
	server/ApplicationStopHandler.cpp
	
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
endif()

