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
set(youroutputdirectory ${PROJECT_SOURCE_DIR}/../builds)

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
	server/pluginSystem/InvalidPluginException.hpp
  
	server/StartupOptions/MustExistPathOption.hpp
	server/StartupOptions/ValidIpAddressOption.hpp
	server/StartupOptions/LoaderCustomValidators.hpp
	server/StartupOptions/Loader.h
	server/StartupOptions/Loader.cpp
	server/StartupOptions/LoaderException.hpp
	server/StartupOptions/StartupOptions.h
	server/StartupOptions/InvalidOptionException.hpp
  
	server/ApplicationStopHandler.h
	server/ApplicationStopHandler.cpp
	server/XplLogger.h
	server/XplLogger.cpp
	
	server/database/IDataProvider.h
	server/database/IHardwareRequester.h
	server/database/IConfigurationRequester.h
	server/database/IDeviceRequester.h
	server/database/IAcquisitionRequester.h
	server/database/IKeywordRequester.h
	server/database/IPageRequester.h
	server/database/IWidgetRequester.h
	server/database/DatabaseException.hpp

	server/database/entities/EntityHelpers.hpp
	server/database/entities/Hardware.h
	server/database/entities/Configuration.h
	server/database/entities/Acquisition.h
	server/database/entities/Device.h
	server/database/entities/Keyword.h
	server/database/entities/Page.h
	server/database/entities/Widget.h
	
	server/web/IWebServer.h
	server/web/IRestHandler.h
	server/web/WebServerManager.h
	server/web/WebServerManager.cpp
	
	server/web/rest/IRestService.h
	server/web/rest/json/json.h
	server/web/rest/json/JsonCollectionSerializer.h
	server/web/rest/json/IEntitySerializer.h
	server/web/rest/json/IEntityDeserializer.h

	server/web/rest/RestDispatcher.h
	server/web/rest/RestDispatcher.cpp
	server/web/rest/RestDispatcherHelpers.hpp
	
	server/web/rest/json/JsonSerializers.h
	server/web/rest/json/JsonSerializers.cpp
	server/web/rest/json/JsonSerializersHelpers.hpp
	
	server/web/rest/HardwareRestService.h
	server/web/rest/HardwareRestService.cpp

	server/web/rest/DeviceRestService.h
	server/web/rest/DeviceRestService.cpp
	
	server/web/rest/PageRestService.h
	server/web/rest/PageRestService.cpp

	server/web/rest/WidgetRestService.h
	server/web/rest/WidgetRestService.cpp

	server/web/rest/AcquisitionRestService.h
	server/web/rest/AcquisitionRestService.cpp
	server/web/rest/ConfigurationRestService.h
	server/web/rest/ConfigurationRestService.cpp
	)

# Project folders
source_group(Server server/*.*)
source_group(Server\\pluginSystem server/pluginSystem/*.* )
source_group(Server\\pluginSystem\\external plugins/hardware/IHardwarePlugin.h )
source_group(Server\\database  server/database/*.*)
source_group(Server\\StartupOptions  server/StartupOptions/*.*)
source_group(Server\\database\\entities  server/database/entities/*.*)
source_group(Server\\web  server/web/*.*)
source_group(Server\\web\\REST  server/web/rest/*.*)
source_group(Server\\web\\REST\\Json  server/web/rest/json/*.*)

#Database
IF(USE_SQLITE_DATABASE)
	set(YADOMS_SRC ${YADOMS_SRC}
		server/database/sqlite/Query.h
		server/database/sqlite/Query.cpp
		server/database/sqlite/QueryValue.h
		server/database/sqlite/QueryValue.cpp
		server/database/sqlite/SQLiteDatabaseTables.h
		server/database/sqlite/SQLiteDatabaseTables.cpp
		server/database/sqlite/SQLiteSystemTables.h
		server/database/sqlite/SQLiteSystemTables.cpp
		server/database/sqlite/SQLiteDatabaseTablesHelpers.hpp
		server/database/sqlite/SQLiteHardwareRequester.h
		server/database/sqlite/SQLiteHardwareRequester.cpp
		server/database/sqlite/SQLiteConfigurationRequester.h
		server/database/sqlite/SQLiteConfigurationRequester.cpp
		server/database/sqlite/SQLiteKeywordRequester.h
		server/database/sqlite/SQLiteKeywordRequester.cpp
		server/database/sqlite/SQLiteAcquisitionRequester.h
		server/database/sqlite/SQLiteAcquisitionRequester.cpp		
		server/database/sqlite/SQLiteDeviceRequester.h
		server/database/sqlite/SQLiteDeviceRequester.cpp		
		server/database/sqlite/SQLiteDataProvider.h
		server/database/sqlite/SQLiteDataProvider.cpp
		server/database/sqlite/SQLiteRequester.h
		server/database/sqlite/SQLiteRequester.cpp
		server/database/sqlite/SQLitePageRequester.h
		server/database/sqlite/SQLitePageRequester.cpp		
		server/database/sqlite/SQLiteWidgetRequester.h
		server/database/sqlite/SQLiteWidgetRequester.cpp		
		
		server/database/sqlite/adapters/SQLite3Extension.hpp
		server/database/sqlite/adapters/SQLiteDatabaseAdapters.h
		server/database/sqlite/adapters/SQLiteDatabaseAdapters.cpp
		server/database/sqlite/adapters/AdapterHelpers.hpp
		server/database/sqlite/adapters/ISQLiteResultAdapter.h
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
		server/database/sqlite/versioning/SQLiteTablesScripts.cpp
	)
	
	source_group(Server\\database\\sqlite  server/database/sqlite/*.*)
	source_group(Server\\database\\sqlite\\adapters  server/database/sqlite/adapters/*.*)
	source_group(Server\\database\\sqlite\\versioning  server/database/sqlite/versioning/*.*)
	
	set(LIBS ${LIBS} sqlite)
	set(YADOMS_INCL_DIR ${YADOMS_INCL_DIR} ${SQLITE_INCLUDE_DIRECTORY})
ENDIF()

#Web server
IF(USE_WEBEM_WEBSERVER)
	set(YADOMS_SRC ${YADOMS_SRC}
		server/web/webem/WebServer.h
		server/web/webem/WebServer.cpp
		server/web/webem/RestHandler.h
		server/web/webem/RestHandler.cpp
	)
	
	source_group(Server\\web\\webem  server/web/webem/*.*)

	add_definitions( -DWEBSERVER_DONT_USE_ZIP )
	
	set(LIBS ${LIBS} webem)
	set(YADOMS_INCL_DIR ${YADOMS_INCL_DIR} ${WEBEM_INCLUDE_DIRECTORY})
ENDIF()

#define YADOMS output
add_executable(yadoms ${YADOMS_SRC})
add_precompiled_header(yadoms stdafx.h)

##################################################################################################
## yadoms-shared
##################################################################################################

add_subdirectory(shared)

set(SHARED_INCL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shared)
IF(WIN32)
	set(SHARED_INCL_DIR ${SHARED_INCL_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/shared/windows)
ELSEIF(APPLE)
	set(SHARED_INCL_DIR ${SHARED_INCL_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/shared/mac)
ELSE()
	set(SHARED_INCL_DIR ${SHARED_INCL_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/shared/linux)
ENDIF()


##################################################################################################
## yadoms-tools
##################################################################################################

# Library Sources
set(TOOLS_LIB_SRC
	tools/tools/stdafx.cpp
	tools/tools/stdafx.h

	tools/tools/dir_monitor/dir_monitor.hpp
	tools/tools/dir_monitor/basic_dir_monitor.hpp
	tools/tools/DirectoryChangeListener.h
	tools/tools/DirectoryChangeListener.cpp
	

	tools/tools/Version.h
	tools/tools/Version.cpp
)

#Append platform specific files
IF(MSVC)
set(TOOLS_LIB_SRC	
	${TOOLS_LIB_SRC}
	tools/windows/tools/dir_monitor/basic_dir_monitor_service.hpp
	tools/windows/tools/dir_monitor/dir_monitor_impl.hpp
)	

ELSEIF(APPLE)
set(TOOLS_LIB_SRC	
	${TOOLS_LIB_SRC}
	tools/mac/tools/dir_monitor/basic_dir_monitor_service.hpp
	tools/mac/tools/dir_monitor/dir_monitor_impl.hpp
)	
ELSE()
set(TOOLS_LIB_SRC	
	${TOOLS_LIB_SRC}
	tools/linux/tools/dir_monitor/basic_dir_monitor_service.hpp
	tools/linux/tools/dir_monitor/dir_monitor_impl.hpp
)	
ENDIF()

# Define project folders
source_group(PlatformSpecific\\windows tools/windows/tools/*.*)
source_group(PlatformSpecific\\linux tools/linux/tools/*.*)
source_group(PlatformSpecific\\mac tools/mac/tools/*.*)
source_group(Exception tools/tools/Exceptions/*.* )	
source_group(Xpl tools/tools/Xpl/*.* )	

# Define library outputs
add_library(yadoms-tools STATIC  ${TOOLS_LIB_SRC})
add_precompiled_header(yadoms-tools stdafx.h)
target_link_libraries(yadoms-tools ${LIBS} ${CMAKE_DL_LIBS})

# Define include directories (allow easy includes)
set(TOOLS_INCL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tools ${CMAKE_CURRENT_SOURCE_DIR}/tools/tools)

IF(WIN32)
	set(TOOLS_INCL_DIR ${TOOLS_INCL_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/tools/windows)
ELSEIF(APPLE)
	set(TOOLS_INCL_DIR ${TOOLS_INCL_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/tools/mac)
ELSE()
	set(TOOLS_INCL_DIR ${TOOLS_INCL_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/tools/linux)
ENDIF()

set_property( TARGET yadoms-tools PROPERTY INCLUDE_DIRECTORIES ${TOOLS_INCL_DIR} ${SHARED_INCL_DIR} ${BOOST_INCL_DIR})

##################################################################################################
## Plugins
##################################################################################################

#include Plugins CMake file
add_subdirectory(plugins/hardware)


##################################################################################################
## Include directories
##################################################################################################
set(YADOMS_INCL_DIR
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/server
	${YADOMS_INCL_DIR}
	${TOOLS_INCL_DIR}
	${SHARED_INCL_DIR}
)
set_property( TARGET yadoms PROPERTY INCLUDE_DIRECTORIES ${YADOMS_INCL_DIR} ${BOOST_INCL_DIR})

##################################################################################################
## Link
##################################################################################################
target_link_libraries(yadoms yadoms-tools yadoms-shared ${LIBS} ${CMAKE_DL_LIBS})

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

#copy www directory into build directory
add_custom_command(TARGET yadoms POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/www $<TARGET_FILE_DIR:yadoms>/www/)
		

