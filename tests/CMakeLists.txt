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

#check eclispe debug options
if(DEBUG_WITH_ECLIPSE)
	add_definitions(-DDEBUG)
	add_definitions("-g")
endif()

##################################################################################################
## User specific Defines
##################################################################################################
if(DEV_ACTIVATE_DATABASE_TESTS)
	add_definitions(-DDEV_ACTIVATE_DATABASE_TESTS)
endif()

if(DEV_ACTIVATE_HARDWARE_PLUGIN_MANAGER_TESTS)
	add_definitions(-DDEV_ACTIVATE_HARDWARE_PLUGIN_MANAGER_TESTS)
endif()

if(DEV_ACTIVATE_SERIAL_PORTS_GETTER_TESTS)
	add_definitions(-DDEV_ACTIVATE_SERIAL_PORTS_GETTER_TESTS)
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
set(USE_SQLITE_DATABASE FALSE)
#Use Webem web server
set(USE_WEBEM_WEBSERVER FALSE)
#Use Tests
set(USE_TESTS_START_OPTIONS_LOADER TRUE)
set(USE_TESTS_HARDWARE_PLUGIN_MANAGER FALSE)

#include libs (have their own CMakeList.txt)
include_directories(../sources/external-libs)


##################################################################################################
## Yadoms
##################################################################################################


# Executable sources
set(YADOMS_SRC
#	../sources/server/stdafx.cpp
#	../sources/server/stdafx.h
#	../sources/server/Supervisor.cpp
#	../sources/server/Supervisor.h
#	../sources/server/pluginSystem/HardwarePluginManager.h
#	../sources/server/pluginSystem/HardwarePluginManager.cpp
#	../sources/server/pluginSyst#em/HardwarePluginInstance.h
#	../sources/server/pluginSystem/HardwarePluginInstance.cpp
#	../sources/server/pluginSystem/HardwarePluginFactory.h
#	../sources/server/pluginSystem/HardwarePluginFactory.cpp
#	../sources/plugins/hardware/IHardwarePlugin.h
#	../sources/plugins/hardware/HardwarePluginConfiguration.hpp
#	../sources/plugins/hardware/HardwarePluginConfigurationParameters.hpp
#	../sources/server/StartupOptionsLoader.h
#	../sources/server/StartupOptionsLoader.cpp
#	../sources/server/StartupOptions.h
#	../sources/server/ApplicationStopHandler.h
#	../sources/server/ApplicationStopHandler.cpp
	
#	../sources/server/database/IDataProvider.h
#	../sources/server/database/IHardwareRequester.h
#	../sources/server/database/IConfigurationRequester.h
#	../sources/server/database/entities/EntityHelpers.hpp
#	../sources/server/database/entities/Hardware.h
#	../sources/server/database/entities/Configuration.h
	)

# Project folders
if(MSVC)
	source_group(Server server/*.*)
	source_group(Server\\pluginSystem server/pluginSystem/*.* )
	source_group(Server\\pluginSystem\\external plugins/hardware/IHardwarePlugin.h )
	source_group(Server\\database  server/database/*.*)
	source_group(Server\\database\\entities  server/database/entities/*.*)
endif()

IF(USE_SQLITE_DATABASE)
	set(YADOMS_SRC ${YADOMS_SRC}
		../sources/server/database/sqlite/Query.h
		../sources/server/database/sqlite/Query.cpp
		../sources/server/database/sqlite/QueryValue.h
		../sources/server/database/sqlite/QueryValue.cpp
		../sources/server/database/sqlite/SQLiteDatabaseTables.h
		../sources/server/database/sqlite/SQLiteDatabaseTables.cpp
		../sources/server/database/sqlite/SQLiteDatabaseTablesHelpers.hpp
		../sources/server/database/sqlite/SQLiteHardwareRequester.h
		../sources/server/database/sqlite/SQLiteHardwareRequester.cpp
		../sources/server/database/sqlite/SQLiteConfigurationRequester.h
		../sources/server/database/sqlite/SQLiteConfigurationRequester.cpp
		../sources/server/database/sqlite/SQLiteKeywordRequester.h
		../sources/server/database/sqlite/SQLiteKeywordRequester.cpp
		../sources/server/database/sqlite/SQLiteAcquisitionRequester.h
		../sources/server/database/sqlite/SQLiteAcquisitionRequester.cpp		
		../sources/server/database/sqlite/SQLiteDeviceRequester.h
		../sources/server/database/sqlite/SQLiteDeviceRequester.cpp		
		../sources/server/database/sqlite/SQLiteDataProvider.h
		../sources/server/database/sqlite/SQLiteDataProvider.cpp
		../sources/server/database/sqlite/SQLiteRequester.h
		../sources/server/database/sqlite/SQLiteRequester.cpp
		
		../sources/server/database/sqlite/adapters/SQLiteDatabaseAdapters.h
		../sources/server/database/sqlite/adapters/AdapterHelpers.hpp
		../sources/server/database/sqlite/adapters/ISQLiteResultAdapter.h
		../sources/server/database/sqlite/adapters/GenericAdapter.h
		../sources/server/database/sqlite/adapters/GenericAdapter.cpp
		../sources/server/database/sqlite/adapters/SingleValueAdapter.hpp
		../sources/server/database/sqlite/adapters/MultipleValueAdapter.hpp
		
		../sources/server/database/sqlite/versioning/ISQLiteVersionUpgrade.h
		../sources/server/database/sqlite/versioning/SQLiteVersion1.h
		../sources/server/database/sqlite/versioning/SQLiteVersion1.cpp
		../sources/server/database/sqlite/versioning/SQLiteVersionUpgraderFactory.h
		../sources/server/database/sqlite/versioning/SQLiteVersionUpgraderFactory.cpp
		../sources/server/database/sqlite/versioning/SQLiteVersionException.h
		../sources/server/database/sqlite/versioning/SQLiteSystemTables.h
		../sources/server/database/sqlite/versioning/SQLiteSystemTables.cpp
	)
	
	if(MSVC)
		source_group(Server\\database\\sqlite  server/database/sqlite/*.*)
		source_group(Server\\database\\sqlite\\adapters  server/database/sqlite/adapters/*.*)
		source_group(Server\\database\\sqlite\\versioning  server/database/sqlite/versioning/*.*)
	endif()
	
	set(LIBS ${LIBS} sqlite)
	set(YADOMS_INCL_DIR ${YADOMS_INCL_DIR} ${SQLITE_INCLUDE_DIRECTORY})
ENDIF()

#If use tests
IF (USE_TESTS_START_OPTIONS_LOADER)
        set(YADOMS_TESTS_STARTUP_OPTIONS_LOADER_SRC

        ../sources/server/stdafx.cpp
        ../sources/server/stdafx.h
  
	../sources/server/StartupOptionsLoader.h
	../sources/server/StartupOptionsLoader.cpp

        ../tests/server/MainTestStartOptionsLoader.cpp

        )

# Project folders
	IF(MSVC)
		#Tests definition for server
	        source_group(Tests server/MainTestStartOptionsLoader.cpp)
                #source_group(Server\\pluginSystem server/pluginSystem/*.* )
	endif()

ENDIF()


IF (USE_TESTS_HARDWARE_PLUGIN_MANAGER)
set(YADOMS_TESTS_HARDWARE_PLUGIN_MANAGER

        ../sources/server/stdafx.cpp
        ../sources/server/stdafx.h
  
	../sources/server/pluginSystem/HardwarePluginManager.h
	../sources/server/pluginSystem/HardwarePluginManager.cpp
        ../sources/server/pluginSystem/HardwarePluginFactory.h
        ../sources/server/pluginSystem/HardwarePluginFactory.cpp
        ../sources/plugins/hardware/IHardwarePlugin.h
        ../sources/server/database/entities/Hardware.h

	../sources/server/StartupOptionsLoader.h
	../sources/server/StartupOptionsLoader.cpp

        ../tests/server/pluginSystem/MainTestHardwarePluginManager.cpp

        )
ENDIF()

#Web server
#IF(USE_WEBEM_WEBSERVER)
#	set(LIBS ${LIBS} webem)
#	set(YADOMS_INCL_DIR ${YADOMS_INCL_DIR} ${WEBEM_INCLUDE_DIRECTORY})
#ENDIF()

#define YADOMS tests executable outputs

IF (USE_TESTS_START_OPTIONS_LOADER)
   add_executable(MainTestStartOptionsLoader ${YADOMS_TESTS_STARTUP_OPTIONS_LOADER_SRC})
   add_precompiled_header(MainTestStartOptionsLoader stdafx.h)
ENDIF()

if (USE_TESTS_HARDWARE_PLUGIN_MANAGER)
   add_executable(MainTestHardwarePluginManager ${YADOMS_TESTS_HARDWARE_PLUGIN_MANAGER} ${YADOMS_SRC})
   add_precompiled_header(MainTestHardwarePluginManager stdafx.h)
ENDIF()

##################################################################################################
## yadoms-tools
##################################################################################################

# Library Sources
set(SHARED_LIB_SRC
	../sources/tools/tools/stdafx.cpp
	../sources/tools/tools/stdafx.h

	../sources/tools/tools/ThreadBase.cpp
	../sources/tools/tools/ThreadBase.h

	../sources/tools/tools/Log.h
	../sources/tools/tools/Log.cpp
	../sources/tools/tools/StringExtension.hpp	
	../sources/tools/tools/Exceptions/Exception.hpp
#	../sources/tools/tools/Exceptions/Exception.cpp
	../sources/tools/tools/Exceptions/NotImplementedException.hpp
	../sources/tools/tools/Exceptions/NullReferenceException.hpp
#	../sources/tools/tools/Exceptions/NullReferenceException.cpp
	../sources/tools/tools/Exceptions/EmptyResultException.hpp

	../sources/tools/tools/dir_monitor/dir_monitor.hpp
	../sources/tools/tools/dir_monitor/basic_dir_monitor.hpp
	../sources/tools/tools/DirectoryChangeListener.h
	../sources/tools/tools/DirectoryChangeListener.cpp
	
	#include both platform files to see them into IDE
	../sources/tools/windows/tools/DynamicLibrary.hpp
	../sources/tools/windows/tools/Export.h
	../sources/tools/windows/tools/dir_monitor/basic_dir_monitor_service.hpp
	../sources/tools/windows/tools/dir_monitor/dir_monitor_impl.hpp

	../sources/tools/linux/tools/DynamicLibrary.hpp
	../sources/tools/linux/tools/Export.h
	../sources/tools/linux/tools/dir_monitor/basic_dir_monitor_service.hpp
	../sources/tools/linux/tools/dir_monitor/dir_monitor_impl.hpp
)

 #Define project folders
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
set(TOOLS_INCL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../sources/tools)
IF(WIN32)
	set(TOOLS_INCL_DIR ${TOOLS_INCL_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../sources/tools/windows)
ELSE()
	set(TOOLS_INCL_DIR ${TOOLS_INCL_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../sources/tools/linux)
	set(TOOLS_INCL_DIR ${TOOLS_INCL_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../sources/tools/tools)
ENDIF()

set_property( TARGET yadoms-tools PROPERTY INCLUDE_DIRECTORIES ${TOOLS_INCL_DIR} ${BOOST_INCL_DIR})

##################################################################################################
## Plugins
##################################################################################################

#include Plugin CMake file
include_directories(../sources/plugins)

##################################################################################################
## Include directories
##################################################################################################
set(YADOMS_INCL_DIR
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../sources/server
        ${CMAKE_CURRENT_SOURCE_DIR}/../sources
	${YADOMS_INCL_DIR}
	${TOOLS_INCL_DIR}
)

IF (USE_TESTS_START_OPTIONS_LOADER)
        set_property( TARGET MainTestStartOptionsLoader PROPERTY INCLUDE_DIRECTORIES ${YADOMS_INCL_DIR} ${BOOST_INCL_DIR})
ENDIF()

IF (USE_TESTS_HARDWARE_PLUGIN_MANAGER)
        set_property( TARGET MainTestHardwarePluginManager PROPERTY INCLUDE_DIRECTORIES ${YADOMS_INCL_DIR} ${BOOST_INCL_DIR})
ENDIF()

##################################################################################################
## Link
##################################################################################################

IF (USE_TESTS_START_OPTIONS_LOADER)
        target_link_libraries(MainTestStartOptionsLoader yadoms-tools ${LIBS} ${CMAKE_DL_LIBS})
ENDIF()

IF (USE_TESTS_HARDWARE_PLUGIN_MANAGER)
        target_link_libraries(MainTestHardwarePluginManager yadoms-tools ${LIBS} ${CMAKE_DL_LIBS})
ENDIF()

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
		else()
			MESSAGE("    Library file not found : ${LIBFILE}")
		endif()
	ENDFOREACH(BOOSTLIBRARY)
endif()		
		
#copy www directory into build directory
#add_custom_command(TARGET yadoms POST_BUILD
#                   COMMAND ${CMAKE_COMMAND} -E copy_directory
#                       ${CMAKE_SOURCE_DIR}/www $<TARGET_FILE_DIR:yadoms>/www/)

