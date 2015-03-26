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

# User specific configuration
include(CMakeListsUserConfig.txt OPTIONAL)

# Cotire 
if(COTIRE_USE)
	include(cotire.cmake)
else()
	include(PrecompiledHeader.cmake)
endif()


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
    atomic
	system 
	filesystem 
	thread 
	date_time 
	chrono 
	program_options
	regex
)

# search boost libs
find_package(Boost COMPONENTS ${BOOST_LIBS} REQUIRED)
set(BOOST_INCL_DIR ${Boost_INCLUDE_DIR} ${BOOST_ROOT})


## Thread Library
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

	if(NOT CARBON_FOUND)
		message(SEND_ERROR "Unable to find the requested Carbon libraries")
	endif()
	set(LIBS ${LIBS} ${IOKIT_LIBRARY} ${CARBON_LIBRARY})
endif()



###############################################
# POCO
#
# POCO_LIBS : define all required poco libraries
###############################################

SET ( POCO_LIBS
	PocoFoundation
   PocoJSON
	PocoNet
	PocoUtil
   PocoXml
   PocoZip
)

include(FindPoco.cmake)

IF(NOT ${Poco_FOUND})
	message(SEND_ERROR "Unable to find the requested POCO libraries")
ENDIF()


# define libraries to link with
set(LIBS ${LIBS} ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${RT_LIBRARY} ${Poco_FOUND_LIBS})

##################################################################################################
## Internal libraries (SQlite, Webem) completly build from sources
##################################################################################################

#Use Sqlite database system
set(USE_SQLITE_DATABASE TRUE)
#Use Poco web server
set(USE_POCO_WEBSERVER TRUE)

#include libs (have their own CMakeList.txt)
add_subdirectory(external-libs)



##################################################################################################
## Yadoms
##################################################################################################


# Executable sources
set(YADOMS_SRC
	server/stdafx.cpp
	server/stdafx.h
	
	server/Application.h
	server/Application.cpp
	server/ApplicationStopHandler.h
	server/ApplicationStopHandler.cpp
	server/ErrorHandler.h
	server/ErrorHandler.cpp
	
	server/IRunningInformation.h
	server/RunningInformation.cpp
	server/RunningInformation.h
	server/Supervisor.cpp
	server/Supervisor.h
  
   server/logConfiguration/LogConfiguration.h
   server/logConfiguration/LogConfiguration.cpp   
   server/logConfiguration/LogConfigurationImpl.h
   server/logConfiguration/LogConfigurationImpl.cpp
   server/logConfiguration/LogConfigurationException.h
   
   server/pluginSystem/CommandHistorizer.cpp
   server/pluginSystem/CommandHistorizer.h
   server/pluginSystem/DeviceCommand.cpp
   server/pluginSystem/DeviceCommand.h
	server/pluginSystem/DummyQualifier.h
	server/pluginSystem/DummyQualifier.cpp
	server/pluginSystem/IdentityForQualifier.cpp
	server/pluginSystem/IdentityForQualifier.h
	server/pluginSystem/Information.cpp
	server/pluginSystem/Information.h
	server/pluginSystem/Instance.h
	server/pluginSystem/Instance.cpp
	server/pluginSystem/IQualifier.h
	server/pluginSystem/InvalidPluginException.hpp
	server/pluginSystem/Factory.h
	server/pluginSystem/InternalPluginFactory.h
	server/pluginSystem/InternalPluginFactory.cpp
	server/pluginSystem/ExternalPluginFactory.h
	server/pluginSystem/ExternalPluginFactory.cpp
	server/pluginSystem/Manager.h
	server/pluginSystem/Manager.cpp
	server/pluginSystem/ManagerEvent.h
   server/pluginSystem/ManuallyDeviceCreationData.cpp
   server/pluginSystem/ManuallyDeviceCreationData.h
   server/pluginSystem/BindingQueryData.cpp
   server/pluginSystem/BindingQueryData.h
   server/pluginSystem/BindingQueryRequest.cpp
   server/pluginSystem/BindingQueryRequest.h
   server/pluginSystem/ManuallyDeviceCreationTestData.cpp
   server/pluginSystem/ManuallyDeviceCreationTestData.h
	server/pluginSystem/Qualifier.h
	server/pluginSystem/Qualifier.cpp
   server/pluginSystem/yPluginApiImplementation.h
   server/pluginSystem/yPluginApiImplementation.cpp
	server/pluginSystem/internalPlugin/System.h
	server/pluginSystem/internalPlugin/System.cpp
	server/pluginSystem/internalPlugin/Information.h
	server/pluginSystem/internalPlugin/Information.cpp
	server/pluginSystem/ManuallyDeviceCreationRequest.h
	server/pluginSystem/ManuallyDeviceCreationRequest.cpp
  
	server/startupOptions/IStartupOptions.h
	server/startupOptions/Loader.h
	server/startupOptions/Loader.cpp
	server/startupOptions/MustExistPathValidator.h
	server/startupOptions/MustExistPathValidator.cpp
   
	server/notifications/NewAcquisitionNotification.h
	server/notifications/NewDeviceNotification.h
	server/notifications/ConfigurationUpdatedNotification.h
	
	server/dataAccessLayer/DataAccessLayer.h
	server/dataAccessLayer/DataAccessLayer.cpp
	server/dataAccessLayer/IDataAccessLayer.h
	server/dataAccessLayer/AcquisitionHistorizer.h
	server/dataAccessLayer/AcquisitionHistorizer.cpp
	server/dataAccessLayer/IAcquisitionHistorizer.h
	server/dataAccessLayer/DeviceManager.h
	server/dataAccessLayer/DeviceManager.cpp
	server/dataAccessLayer/IDeviceManager.h
	server/dataAccessLayer/ConfigurationManager.h
	server/dataAccessLayer/ConfigurationManager.cpp
	server/dataAccessLayer/IConfigurationManager.h
	
	server/database/DatabaseException.hpp
	server/database/IConfigurationRequester.h
	server/database/IAcquisitionRequester.h
	server/database/IDataProvider.h
	server/database/IDataBackup.h
	server/database/IDeviceRequester.h
	server/database/IEventLoggerRequester.h
	server/database/IRuleRequester.h
	server/database/IKeywordRequester.h
	server/database/IPageRequester.h
	server/database/IPluginEventLoggerRequester.h
	server/database/IPluginRequester.h
	server/database/IRecipientRequester.h
	server/database/ITransactionalProvider.h
	server/database/IWidgetRequester.h

	server/database/entities/Entities.h
	server/database/entities/Entities.cpp
	server/database/entities/EntityHelpers.hpp

   server/authentication/IAuthentication.h
   server/authentication/BasicAuthentication.h
   server/authentication/BasicAuthentication.cpp

   
	server/automation/IRule.h
	server/automation/IRuleThreadHandler.h
	server/automation/IRuleStateHandler.h
	server/automation/IRuleManager.h
	server/automation/ManagerEvent.h
	server/automation/Rule.h
	server/automation/Rule.cpp
	server/automation/RuleException.hpp
	server/automation/RuleStateHandler.h
	server/automation/RuleStateHandler.cpp
	server/automation/RuleManager.h
	server/automation/RuleManager.cpp
	server/automation/RuleThread.h
	server/automation/RuleThread.cpp
   
	server/automation/script/DayLight.h
	server/automation/script/DayLight.cpp
	server/automation/script/Factory.h
	server/automation/script/Factory.cpp
	server/automation/script/GeneralInfo.h
	server/automation/script/GeneralInfo.cpp
	server/automation/script/IAutoLocation.h
	server/automation/script/IDayLight.h
	server/automation/script/IFactory.h
	server/automation/script/ILocation.h
	server/automation/script/ILogger.h
	server/automation/script/IGeneralInfo.h
	server/automation/script/IInterpreterLibrary.h
	server/automation/script/IpApiAutoLocation.h
	server/automation/script/IpApiAutoLocation.cpp
   server/automation/script/InterpreterNotFound.hpp
	server/automation/script/InterpreterLibrary.h
	server/automation/script/InterpreterLibrary.cpp
	server/automation/script/IProperties.h
	server/automation/script/Location.h
	server/automation/script/Location.cpp
	server/automation/script/PocoLogger.h
	server/automation/script/PocoLogger.cpp
	server/automation/script/Properties.h
	server/automation/script/Properties.cpp
	server/automation/script/YScriptApiImplementation.h
	server/automation/script/YScriptApiImplementation.cpp
   
	server/web/IRestHandler.h
	server/web/IWebServer.h
	server/web/IWebServerConfigurator.h
 
	server/web/rest/RestDispatcher.h
	server/web/rest/RestDispatcher.cpp
	server/web/rest/RestDispatcherHelpers.hpp
	server/web/rest/Result.h
	server/web/rest/Result.cpp

	server/web/rest/service/Acquisition.h
	server/web/rest/service/Acquisition.cpp
	server/web/rest/service/AutomationRule.h
	server/web/rest/service/AutomationRule.cpp
	server/web/rest/service/Configuration.h
	server/web/rest/service/Configuration.cpp
	server/web/rest/service/Device.h
	server/web/rest/service/Device.cpp
	server/web/rest/service/EventLogger.h
	server/web/rest/service/EventLogger.cpp
	server/web/rest/service/System.h
	server/web/rest/service/System.cpp
	server/web/rest/service/IRestService.h
	server/web/rest/service/Page.h
	server/web/rest/service/Page.cpp
	server/web/rest/service/Plugin.h
	server/web/rest/service/Plugin.cpp
	server/web/rest/service/PluginEventLogger.h
	server/web/rest/service/PluginEventLogger.cpp
	server/web/rest/service/Recipient.h
	server/web/rest/service/Recipient.cpp
	server/web/rest/service/Task.h
	server/web/rest/service/Task.cpp
	server/web/rest/service/Widget.h
	server/web/rest/service/Widget.cpp
	
	server/web/ws/AcquisitionFilterFrame.h
	server/web/ws/AcquisitionFilterFrame.cpp
	server/web/ws/AcquisitionUpdateFrame.h
	server/web/ws/AcquisitionUpdateFrame.cpp
	server/web/ws/NewDeviceFrame.h
	server/web/ws/NewDeviceFrame.cpp
	server/web/ws/FrameBase.h
	server/web/ws/FrameBase.cpp
	server/web/ws/FrameFactory.h
	server/web/ws/FrameFactory.cpp
	
	server/task/FinishedInstance.h
	server/task/FinishedInstance.cpp
	server/task/IInstance.h
	server/task/Instance.h
	server/task/Instance.cpp
	server/task/ITask.h
	server/task/IUnique.h
	server/task/Scheduler.h
	server/task/Scheduler.cpp
	server/task/TaskEvent.h
	server/task/TaskEvent.cpp
	server/task/TaskFactory.h
	server/task/TaskFactory.cpp
	server/task/update/Plugin.h
	server/task/update/Plugin.cpp
	server/task/update/Yadoms.h
	server/task/update/Yadoms.cpp
	server/task/backup/Database.h
	server/task/backup/Database.cpp

	server/communication/ISendMessageAsync.h
 	server/communication/PluginGateway.h
 	server/communication/PluginGateway.cpp	
 	server/communication/callback/CallbackRequest.h
 	server/communication/callback/SynchronousCallback.h
 	server/communication/callback/CallbackException.hpp
   server/communication/callback/ICallbackRequest.h
 	server/communication/callback/ISynchronousCallback.h
)

# Project folders
source_group(server server/*.*)
source_group(server\\authentication  server/authentication/*.*)
source_group(server\\automation  server/automation/*.*)
source_group(server\\automation\\script  server/automation/script/*.*)
source_group(server\\communication  server/communication/*.*)
source_group(server\\communication\\callback  server/communication/callback/*.*)
source_group(server\\dataAccessLayer  server/dataAccessLayer/*.*)
source_group(server\\database  server/database/*.*)
source_group(server\\database\\entities  server/database/entities/*.*)
source_group(server\\logConfiguration  server/logConfiguration/*.*)
source_group(server\\notifications  server/notifications/*.*)
source_group(server\\pluginSystem server/pluginSystem/*.* )
source_group(server\\pluginSystem\\internalPlugin server/pluginSystem/internalPlugin/*.* )
source_group(server\\startupOptions  server/startupOptions/*.*)
source_group(server\\task  server/task/*.*)
source_group(server\\task\\update  server/task/update/*.*)
source_group(server\\task\\backup  server/task/backup/*.*)
source_group(server\\web  server/web/*.*)
source_group(server\\web\\rest  server/web/rest/*.*)
source_group(server\\web\\rest\\json  server/web/rest/json/*.*)
source_group(server\\web\\rest\\service  server/web/rest/service/*.*)
source_group(server\\web\\ws  server/web/ws/*.*)

# Tools
set(YADOMS_SRC ${YADOMS_SRC}
	server/tools/tools/IApplication.h
	server/tools/tools/Version.h
	server/tools/tools/Version.cpp
	server/tools/tools/web/FileDownloader.h
	server/tools/tools/web/FileDownloader.cpp
	server/tools/tools/web/Uri.h
	server/tools/tools/web/Uri.cpp
	server/tools/tools/web/UriSinglePatternValidator.h
	server/tools/tools/web/UriSinglePatternValidator.cpp
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
		server/tools/mac/tools/OperatingSystem.h
		server/tools/mac/tools/OperatingSystem.cpp
	)	
ELSE()
	set(YADOMS_SRC ${YADOMS_SRC}
		# Add here server/tools/linux/tools/ sources
		server/tools/linux/tools/OperatingSystem.h
		server/tools/linux/tools/OperatingSystem.cpp
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
source_group(server\\tools\\platformSpecific\\windows\\service server/tools/windows/tools/service/*.*)
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
		server/database/sqlite/requesters/Rule.h
		server/database/sqlite/requesters/Rule.cpp		
		server/database/sqlite/requesters/Page.h
		server/database/sqlite/requesters/Page.cpp		
		server/database/sqlite/requesters/Recipient.h
		server/database/sqlite/requesters/Recipient.cpp		
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
		server/database/sqlite/adapters/HighchartValueAdapter.hpp
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
IF(USE_POCO_WEBSERVER)
	set(YADOMS_SRC ${YADOMS_SRC}
		server/web/poco/WebServer.h
		server/web/poco/WebServer.cpp
		server/web/poco/AuthenticationRequestHandler.h
		server/web/poco/AuthenticationRequestHandler.cpp
		server/web/poco/HttpRequestHandlerFactory.h
		server/web/poco/HttpRequestHandlerFactory.cpp
		server/web/poco/RestRequestHandler.h
		server/web/poco/RestRequestHandler.cpp
		server/web/poco/WebsiteRequestHandler.h
		server/web/poco/WebsiteRequestHandler.cpp
		server/web/poco/WebSocketRequestHandler.h
		server/web/poco/WebSocketRequestHandler.cpp	
		server/web/poco/WebSocketClient.h
		server/web/poco/WebSocketClient.cpp	
		server/web/poco/MimeType.h
		server/web/poco/MimeType.cpp	
	)
	
	source_group(server\\web\\poco  server/web/poco/*.*)
ENDIF()

#define YADOMS output
add_executable(yadoms ${YADOMS_SRC})

if(COTIRE_USE)
	#nothing to do
else()
	add_precompiled_header(yadoms stdafx.h)
endif()

##################################################################################################
## yadoms-shared
##################################################################################################

add_subdirectory(shared)

set(SHARED_COMMON_INCL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shared)
set(SHARED_INCL_DIR ${SHARED_COMMON_INCL_DIR})
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
## Script interpreters
##################################################################################################

#include Script interpreters CMake file
add_subdirectory(scriptInterpreters)


##################################################################################################
## Include directories
##################################################################################################
set(YADOMS_INCL_DIR
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/server
	${YADOMS_INCL_DIR}
	${SHARED_INCL_DIR}
)

set(YADOMS_ALL_INCL_DIR
	${YADOMS_INCL_DIR} 
	${BOOST_INCL_DIR} 
	${Poco_INCLUDE_DIRS}
)

set_property( TARGET yadoms PROPERTY INCLUDE_DIRECTORIES ${YADOMS_ALL_INCL_DIR})

##################################################################################################
## Link
##################################################################################################
target_link_libraries(yadoms yadoms-shared ${LIBS} ${CMAKE_DL_LIBS})

##################################################################################################
## Post build commands
##################################################################################################

POST_BUILD_COPY_BOOST_LIBRARIES(yadoms)
POST_BUILD_COPY_POCO(yadoms)

#copy www directory into build directory
add_custom_command(TARGET yadoms POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/www $<TARGET_FILE_DIR:yadoms>/www/)
		

##################################################################################################
## COTIRE
##################################################################################################
if(COTIRE_USE)
	set_target_properties(yadoms PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "server/stdafx.h")
	
	if(COTIRE_USE_UNITY)
		set_target_properties(yadoms PROPERTIES COTIRE_ADD_UNITY_BUILD TRUE)
	else()
		set_target_properties(yadoms PROPERTIES COTIRE_ADD_UNITY_BUILD FALSE)
	endif()
	
	cotire(yadoms)
	
	if(COTIRE_USE_UNITY)
		target_link_libraries(yadoms_unity yadoms-shared_unity ${LIBS} ${CMAKE_DL_LIBS})
		set_property( TARGET yadoms_unity PROPERTY INCLUDE_DIRECTORIES ${YADOMS_ALL_INCL_DIR})
	endif()	
endif()
      
