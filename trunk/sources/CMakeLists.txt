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

# Check the minimum CMake version
cmake_minimum_required (VERSION 2.8.9)

##################################################################################################
## Include other cmake scripts
##################################################################################################

# First checks (compilers versions...)
include(preChecks.cmake)

# Precompiled headers
include(PrecompiledHeader.cmake)

# User specific configuration
include(CMakeListsUserConfig.txt OPTIONAL)

# Windows specific macro
if(WIN32)
	include(windows.cmake)
endif()

# Compilers/IDE configurations
include(compilers.cmake)

# Boost built libraries copy helper
include(copyBoost.cmake)


##################################################################################################
## Project output configuration
##################################################################################################

#define outputs
set(youroutputdirectory ${PROJECT_SOURCE_DIR}/../builds)

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
## Internal libraries (SQlite, Webem) completly build from sources
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
	server/System.cpp
	server/System.h
  
   server/pluginSystem/DeviceCommand.cpp
   server/pluginSystem/DeviceCommand.h
   server/pluginSystem/ManuallyDeviceCreationData.cpp
   server/pluginSystem/ManuallyDeviceCreationData.h
   server/pluginSystem/ManuallyDeviceCreationTestData.cpp
   server/pluginSystem/ManuallyDeviceCreationTestData.h
	server/pluginSystem/DummyQualifier.h
	server/pluginSystem/DummyQualifier.cpp
	server/pluginSystem/IdentityForQualifier.cpp
	server/pluginSystem/IdentityForQualifier.h
	server/pluginSystem/Information.cpp
	server/pluginSystem/Information.h
	server/pluginSystem/Qualifier.h
	server/pluginSystem/Qualifier.cpp
	server/pluginSystem/Manager.h
	server/pluginSystem/Manager.cpp
	server/pluginSystem/ManagerEvent.h
	server/pluginSystem/Instance.h
	server/pluginSystem/Instance.cpp
	server/pluginSystem/Factory.h
	server/pluginSystem/Factory.cpp
	server/pluginSystem/IQualifier.h
	server/pluginSystem/InvalidPluginException.hpp
   server/pluginSystem/YadomsApiImplementation.h
   server/pluginSystem/YadomsApiImplementation.cpp
  
	server/startupOptions/MustExistPathOption.hpp
	server/startupOptions/ValidIpAddressOption.hpp
	server/startupOptions/LoaderCustomValidators.hpp
	server/startupOptions/Loader.h
	server/startupOptions/Loader.cpp
	server/startupOptions/LoaderException.hpp
	server/startupOptions/IStartupOptions.h
	server/startupOptions/InvalidOptionException.hpp
  
	server/ApplicationStopHandler.h
	server/ApplicationStopHandler.cpp
	
	server/database/IDataProvider.h
	server/database/IDataBackup.h
	server/database/ITransactionalProvider.h
	server/database/IPluginRequester.h
	server/database/IConfigurationRequester.h
	server/database/IDeviceRequester.h
	server/database/IKeywordRequester.h
	server/database/IPageRequester.h
	server/database/IWidgetRequester.h
	server/database/IPluginEventLoggerRequester.h
	server/database/IEventLoggerRequester.h
	server/database/IAcquisitionRequester.h
	server/database/DatabaseException.hpp

	server/database/entities/EntityHelpers.hpp
	server/database/entities/Entities.h
	server/database/entities/Entities.cpp
	server/database/entities/Field.hpp
	
	server/web/IWebServer.h
	server/web/IRestHandler.h
	server/web/WebServerManager.h
	server/web/WebServerManager.cpp
	
	server/web/rest/RestDispatcher.h
	server/web/rest/RestDispatcher.cpp
	server/web/rest/RestDispatcherHelpers.hpp

	server/web/rest/json/Json.h
	server/web/rest/json/JsonUtility.h
	server/web/rest/json/JsonUtility.cpp
	server/web/rest/json/JsonCollectionSerializer.h
	server/web/rest/json/IEntitySerializer.h
	server/web/rest/json/IEntityDeserializer.h
	server/web/rest/json/JsonResult.h
	server/web/rest/json/JsonResult.cpp	
	server/web/rest/json/JsonDate.h
	server/web/rest/json/JsonDate.cpp
	server/web/rest/json/JsonSerializers.h
	server/web/rest/json/JsonSerializers.cpp
	server/web/rest/json/JsonGenericSerializer.h
	server/web/rest/json/JsonGenericSerializer.cpp
	server/web/rest/json/JsonSerializersHelpers.hpp

	server/web/rest/service/IRestService.h
	server/web/rest/service/Plugin.h
	server/web/rest/service/Plugin.cpp
	server/web/rest/service/Device.h
	server/web/rest/service/Device.cpp
	server/web/rest/service/Page.h
	server/web/rest/service/Page.cpp
	server/web/rest/service/Widget.h
	server/web/rest/service/Widget.cpp
	server/web/rest/service/Configuration.h
	server/web/rest/service/Configuration.cpp
	server/web/rest/service/PluginEventLogger.h
	server/web/rest/service/PluginEventLogger.cpp
	server/web/rest/service/EventLogger.h
	server/web/rest/service/EventLogger.cpp
	server/web/rest/service/General.h
	server/web/rest/service/General.cpp
	server/web/rest/service/Acquisition.h
	server/web/rest/service/Acquisition.cpp
	
	server/task/Scheduler.h
	server/task/Scheduler.cpp
	server/task/Instance.h
	server/task/Instance.cpp
	server/task/ITask.h
	server/task/update/Plugin.h
	server/task/update/Plugin.cpp
	server/task/update/Yadoms.h
	server/task/update/Yadoms.cpp
	server/task/backup/Database.h
	server/task/backup/Database.cpp

 	server/communication/PluginGateway.h
 	server/communication/PluginGateway.cpp	
	server/communication/ISendMessageAsync.h
	server/communication/command/Result.h
	server/communication/command/Result.cpp	
	server/communication/command/Callback.h
	server/communication/command/Callback.cpp	

)

# Project folders
source_group(server server/*.*)
source_group(server\\pluginSystem server/pluginSystem/*.* )
source_group(server\\database  server/database/*.*)
source_group(server\\startupOptions  server/startupOptions/*.*)
source_group(server\\database\\entities  server/database/entities/*.*)
source_group(server\\web  server/web/*.*)
source_group(server\\web\\rest  server/web/rest/*.*)
source_group(server\\web\\rest\\json  server/web/rest/json/*.*)
source_group(server\\web\\rest\\service  server/web/rest/service/*.*)
source_group(server\\task  server/task/*.*)
source_group(server\\task\\update  server/task/update/*.*)
source_group(server\\task\\backup  server/task/backup/*.*)
source_group(server\\communication  server/communication/*.*)
source_group(server\\communication\\command  server/communication/command/*.*)

# Tools
set(YADOMS_SRC ${YADOMS_SRC}
	server/tools/tools/Version.h
	server/tools/tools/Version.cpp
	server/tools/tools/web/Uri.h
	server/tools/tools/web/Uri.cpp
	server/tools/tools/web/FileDownloader.h
	server/tools/tools/web/FileDownloader.cpp
)

IF(WIN32)
set(YADOMS_SRC ${YADOMS_SRC}
	# Add here server/tools/windows/tools/ sources
	server/tools/windows/tools/OperatingSystem.h
	server/tools/windows/tools/OperatingSystem.cpp
)	
ELSEIF(APPLE)
set(YADOMS_SRC ${YADOMS_SRC}
	# Add here server/tools/mac/tools/ sources
	server/tools/windows/tools/OperatingSystem.h
	server/tools/windows/tools/OperatingSystem.cpp
)	
ELSE()
set(YADOMS_SRC ${YADOMS_SRC}
	# Add here server/tools/linux/tools/ sources
	server/tools/windows/tools/OperatingSystem.h
	server/tools/windows/tools/OperatingSystem.cpp
)	
ENDIF()

# Define include directories (allow easy includes)
set(YADOMS_INCL_DIR ${YADOMS_INCL_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/server/tools)

IF(WIN32)
	set(YADOMS_INCL_DIR ${YADOMS_INCL_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/server/tools/windows)
ELSEIF(APPLE)
	set(YADOMS_INCL_DIR ${YADOMS_INCL_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/server/tools/mac)
ELSE()
	set(YADOMS_INCL_DIR ${YADOMS_INCL_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/server/tools/linux)
ENDIF()

source_group(server\\tools tools/*.*)
source_group(server\\tools\\web tools/web/*.*)
source_group(server\\tools\\platformSpecific\\windows server/tools/windows/tools/*.*)
source_group(server\\tools\\platformSpecific\\linux server/tools/linux/tools/*.*)
source_group(server\\tools\\platformSpecific\\mac server/tools/mac/tools/*.*)

#Database
IF(USE_SQLITE_DATABASE)
	set(YADOMS_SRC ${YADOMS_SRC}
		server/database/sqlite/Query.h
		server/database/sqlite/Query.cpp
		server/database/sqlite/QueryValue.h
		server/database/sqlite/QueryValue.cpp
		server/database/sqlite/SQLiteDataProvider.h
		server/database/sqlite/SQLiteDataProvider.cpp
		server/database/sqlite/SQLiteRequester.h
		server/database/sqlite/SQLiteRequester.cpp
		server/database/sqlite/SQLiteDatabaseTables.h
		server/database/sqlite/SQLiteDatabaseTables.cpp
		server/database/sqlite/SQLiteSystemTables.h
		server/database/sqlite/SQLiteSystemTables.cpp
		server/database/sqlite/SQLiteDatabaseTablesHelpers.hpp
		server/database/sqlite/SQLiteTablesScripts.cpp
		server/database/sqlite/requesters/Plugin.h
		server/database/sqlite/requesters/Plugin.cpp
		server/database/sqlite/requesters/Configuration.h
		server/database/sqlite/requesters/Configuration.cpp
		server/database/sqlite/requesters/Keyword.h
		server/database/sqlite/requesters/Keyword.cpp
		server/database/sqlite/requesters/Device.h
		server/database/sqlite/requesters/Device.cpp		
		server/database/sqlite/requesters/Page.h
		server/database/sqlite/requesters/Page.cpp		
		server/database/sqlite/requesters/Widget.h
		server/database/sqlite/requesters/Widget.cpp		
		server/database/sqlite/requesters/PluginEventLogger.h
		server/database/sqlite/requesters/PluginEventLogger.cpp
		server/database/sqlite/requesters/EventLogger.h
		server/database/sqlite/requesters/EventLogger.cpp
		server/database/sqlite/requesters/Acquisition.h
		server/database/sqlite/requesters/Acquisition.cpp
		
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
		
	)
	
	source_group(server\\database\\sqlite  server/database/sqlite/*.*)
	source_group(server\\database\\sqlite\\adapters  server/database/sqlite/adapters/*.*)
	source_group(server\\database\\sqlite\\requesters  server/database/sqlite/requesters/*.*)
	source_group(server\\database\\sqlite\\versioning  server/database/sqlite/versioning/*.*)
	
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
	
	source_group(server\\web\\webem  server/web/webem/*.*)

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
## Plugins
##################################################################################################

#include Plugins CMake file
add_subdirectory(plugins)


##################################################################################################
## Include directories
##################################################################################################
set(YADOMS_INCL_DIR
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/server
	${YADOMS_INCL_DIR}
	${SHARED_INCL_DIR}
)
set_property( TARGET yadoms PROPERTY INCLUDE_DIRECTORIES ${YADOMS_INCL_DIR} ${BOOST_INCL_DIR})

##################################################################################################
## Link
##################################################################################################
target_link_libraries(yadoms yadoms-shared ${LIBS} ${CMAKE_DL_LIBS})

##################################################################################################
## Post build commands
##################################################################################################

POST_BUILD_COPY_BOOST_LIBRARIES(yadoms)

#copy www directory into build directory
add_custom_command(TARGET yadoms POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/www $<TARGET_FILE_DIR:yadoms>/www/)
		

