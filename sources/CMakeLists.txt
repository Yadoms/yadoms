##################################################################################################
## CMakeList.txt for YADOMS
##
## This file generates projects/makefiles for the complete Yadoms software
##
## This file actually works for
##   Windows 7 x64, 10 x64, MSVC 12, MSVC 2015 (also works with MSVC Community edition)
##   Ubuntu 12.04, 14.04, 16.04, g++ 4.7 .. 5.4
##   Raspberry Raspbian Wheezy, g++ 4.7
##   MacOSx Snow Leopard 10.8, Lion 10.9, yosemite 10.10 and ElCaptain 10.11 , g++ 4.8 and more
##################################################################################################
project (Yadoms)

# Check the minimum CMake version
cmake_minimum_required (VERSION 3.14.0)

# Global CMake policies
cmake_policy(SET CMP0074 NEW)
cmake_policy(SET CMP0078 NEW)
cmake_policy(SET CMP0086 NEW)

##################################################################################################
## Extract Yadoms version from changelog.md file
##################################################################################################
include(cmake/version.cmake)
GET_PACKAGE_VERSION("${CMAKE_SOURCE_DIR}/server/changelog.md" YADOMS_VERSION)
EXTRACT_VERSION(${YADOMS_VERSION} YADOMS_VERSION_MAJOR YADOMS_VERSION_MINOR YADOMS_VERSION_PATCH)
message (STATUS "### Building Yadoms ${YADOMS_VERSION} ###\n")
add_definitions(-DYADOMS_VERSION="${YADOMS_VERSION}")

##################################################################################################
## Include other cmake scripts
##################################################################################################

# First checks (compilers versions...)
include(cmake/preChecks.cmake)

# User specific configuration
include(CMakeListsUserConfig.txt OPTIONAL)

# Cotire 
if(COTIRE_USE)
   set(COTIRE_PCH_MEMORY_SCALING_FACTOR 256)
   include(cmake/cotire.cmake)
else()
   include(cmake/PrecompiledHeader.cmake)
endif()

# Windows specific macro
if(WIN32)
   include(cmake/windows.cmake)
endif()

if(MSVC)
   set (LINK_WITH_STATIC_RUNTIME 1)
   include(cmake/MSVCRuntime.cmake)
endif()
   
# Compilers/IDE configurations
include(cmake/compilers.cmake)

##################################################################################################
## CPack installation preconfiguration
##################################################################################################
set(INSTALL_BINDIR "bin")
SET(CPACK_OUTPUT_FILE_PREFIX "${PROJECT_SOURCE_DIR}/../builds/package")
set(CMAKE_INSTALL_DEFAULT_COMPONENT_NAME "YadomsCore")

##################################################################################################
## Project output configuration
##################################################################################################

#define outputs
set(YADOMS_OUTPUT_DIR ${PROJECT_SOURCE_DIR}/../builds)

# First for the generic no-config case (e.g. with gcc)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${YADOMS_OUTPUT_DIR})
set(CMAKE_PDB_OUTPUT_DIRECTORY .)
# Second, for multi-config builds (e.g. msvc)
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
   string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
   set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${YADOMS_OUTPUT_DIR}/${OUTPUTCONFIG})
endforeach(OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES)


##################################################################################################
## Externals libraries (Boost, Thread, RT) only linked with
##################################################################################################


###############################################
# Boost
#
###############################################
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME ON)
add_definitions(-DBOOST_SPIRIT_THREADSAFE)

# define all needed boost libraries
set(BOOST_LIBS 
   atomic
   system 
   filesystem 
   thread 
   date_time 
   chrono 
   regex
)

# search boost libs
find_package(Boost 1.60.0 REQUIRED COMPONENTS ${BOOST_LIBS})
set(BOOST_INCL_DIR ${Boost_INCLUDE_DIR} ${BOOST_ROOT})


## Thread Library
FIND_PACKAGE(Threads)

#ensure "RT" is added after OpenSSL libs
if(UNIX AND NOT APPLE)
   set(LIBS ${LIBS} rt)
endif()

## Poco dependencies
if(WIN32)
   set(LIBS ${LIBS} Iphlpapi.lib)
endif()

## OpenSSL dependencies
if(WIN32)
   set(LIBS ${LIBS} legacy_stdio_definitions.lib Crypt32.lib)
endif()

## Yadoms dependencies
if(WIN32)
   set(LIBS ${LIBS} SetupApi.lib)
endif()

## IOKIT (needed for Mac)
if(APPLE)
   include(cmake/mac.cmake)
   
   if(NOT IOKIT_FOUND)
      message(SEND_ERROR "Unable to find the required IOKit libraries")
   endif()

   if(NOT CARBON_FOUND)
      message(SEND_ERROR "Unable to find the required Carbon libraries")
   endif()
   set(LIBS ${LIBS} ${IOKIT_LIBRARY} ${CARBON_LIBRARY})
endif()

##specific linker lib for osxcross (cross build for Darwin)
if (CMAKE_SYSTEM_NAME MATCHES Darwin)
   #check osxcross cross compilation
   #in taht case, we must use 
   find_package(Iconv)
   if(Iconv_FOUND)
         set(LIBS ${LIBS} ${Iconv_LIBRARIES})
   else(Iconv_FOUND)
         message(FATAL_ERROR "libiconv not FOUND")
   endif()
   find_package(Intl)
   if(Intl_FOUND)
         set(LIBS ${LIBS} ${Intl_LIBRARIES})
   else(Intl_FOUND)
         message(FATAL_ERROR "libintl not FOUND")
   endif()
endif()

###############################################
# OpenSSL
#
###############################################
SET(OPENSSL_USE_STATIC_LIBS TRUE)
SET(OPENSSL_MSVC_STATIC_RT TRUE)
SET(OPENSSL_ROOT_DIR ${OPENSSL_ROOT})
FIND_PACKAGE(OpenSSL)

if(NOT ${OPENSSL_FOUND})
   message("Cannot find opensll, all plugins which requires SSL features will not be loaded")
   set(OPENSSL_INCLUDE_DIR "")
endif()
#Do not add OPENSSL_SSL_LIBRARY and OPENSSL_CRYPTO_LIBRARY here; it will be later
#if done here, this is too early, and for gcc the libraries appears in bad order regarding Poco for example


###############################################
# POCO
#
# POCO_LIBS : define all required poco libraries
###############################################

SET (POCO_LIBS
   PocoUtil
   PocoNetSSL
   PocoCrypto
   PocoZip
   PocoXML
   PocoNet
   PocoJSON
   PocoFoundation
)

set(POCO_REQUIRE_MINIMUM_VERSION "1.7.3")
set(POCO_USE_STATIC_RUNTIME 1)
add_definitions(-DPOCO_STATIC)
add_definitions(-DPOCO_NO_AUTOMATIC_LIBS)
if(MSVC)
	#fix for linking with external OpenSSL
	SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:libcrypto.lib ")
	SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:libssl.lib ")
endif(MSVC)

add_definitions(-DPOCO_EXTERNAL_OPENSSL=42) #force an unknown value (empty value do not work correctly)
include(cmake/FindPoco.cmake)

IF(NOT ${Poco_FOUND})
   message(FATAL_ERROR "Unable to find the required POCO libraries")
ENDIF()


###############################################
# Protobuf
#
###############################################
set (PROTOBUF_SRC_ROOT_FOLDER ${PROTOBUF_ROOT})
set (Protobuf_USE_STATIC_LIBS ON)

#Helper (FindProtoBuf.cmake allows defining all variables before finding it)
#In case Protobuf has been installed in a custom directory (${PROTOBUF_ROOT}) 
#then predefine all files and paths (make it works with old cmake <3.9)
if(PROTOBUF_INSTALLED_TO_CUSTOM_DIRECTORY)
   if(NOT PROTOBUF_INCLUDE_DIR)
      set(PROTOBUF_INCLUDE_DIR "${PROTOBUF_ROOT}/include")
   endif(NOT PROTOBUF_INCLUDE_DIR)
   if(NOT PROTOBUF_LIBRARY)
      set(PROTOBUF_LIBRARY "${PROTOBUF_ROOT}/lib/libprotobuf.a")
   endif(NOT PROTOBUF_LIBRARY)
   if(NOT PROTOBUF_LIBRARY_DEBUG)
      set(PROTOBUF_LIBRARY_DEBUG "${PROTOBUF_ROOT}/lib/libprotobuf.a")
   endif(NOT PROTOBUF_LIBRARY_DEBUG)
   if(NOT PROTOBUF_LITE_LIBRARY)
      set(PROTOBUF_LITE_LIBRARY "${PROTOBUF_ROOT}/lib/libprotobuf-lite.a")
   endif(NOT PROTOBUF_LITE_LIBRARY)
   if(NOT PROTOBUF_LITE_LIBRARY_DEBUG)
      set(PROTOBUF_LITE_LIBRARY_DEBUG "${PROTOBUF_ROOT}/lib/libprotobuf-lite.a")
   endif(NOT PROTOBUF_LITE_LIBRARY_DEBUG)
   if(NOT PROTOBUF_PROTOC_EXECUTABLE)
      set(PROTOBUF_PROTOC_EXECUTABLE "${PROTOBUF_ROOT}/bin/protoc")
   endif(NOT PROTOBUF_PROTOC_EXECUTABLE)
endif(PROTOBUF_INSTALLED_TO_CUSTOM_DIRECTORY)

find_package(Protobuf 3.1.0 REQUIRED)

if(NOT ${PROTOBUF_FOUND})
   message(FATAL_ERROR "Unable to find the required Protobuf tool")
endif()


if(MSVC)   
   # Workaround to avoid conflict between min/max constants defined in windows.h and Protobuf
   add_definitions(-DNOMINMAX)
endif()


###############################################
# define libraries to link with
#
###############################################

# This library Order is VERY important for GCC
# At this time what is needed:
#   1/ OpenSSL libs must appear after Poco
#   2/ Libs must appear after Boost
#   3/ CMAKE_THREAD_LIBS_INIT must appear after Openssl
# maybe other conditions are needed, but only those are actually detected
set(LIBS
   ${Poco_FOUND_LIBS}
   ${OPENSSL_SSL_LIBRARY}
   ${OPENSSL_CRYPTO_LIBRARY}
   ${CMAKE_THREAD_LIBS_INIT}
   ${Boost_LIBRARIES}
   ${Protobuf_LIBRARIES}
   ${LIBS}
)




##################################################################################################
## Internal libraries (SQlite, Webem) completely build from sources
##################################################################################################

#Use Sqlite database system
set(USE_SQLITE_DATABASE TRUE)
#Use PostgreSQL database system
set(USE_PGSQL_DATABASE TRUE)
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
   server/ErrorHandler.h
   server/ErrorHandler.cpp
   server/IPathProvider.h
   server/PathProvider.cpp
   server/PathProvider.h   
   server/IRunningInformation.h
   server/RunningInformation.cpp
   server/RunningInformation.h   
   server/Supervisor.cpp
   server/Supervisor.h

   server/authentication/IAuthentication.h
   server/authentication/BasicAuthentication.h
   server/authentication/BasicAuthentication.cpp

   server/dateTime/DateTimeNotifier.h
   server/dateTime/DateTimeNotifier.cpp
   server/dateTime/ITimeZoneProvider.h
   server/dateTime/TimeZoneDatabase.h
   server/dateTime/TimeZoneDatabase.cpp
   server/dateTime/TimeZoneProvider.h
   server/dateTime/TimeZoneProvider.cpp
   server/dateTime/TimeZoneProviderFromId.h
   server/dateTime/TimeZoneProviderFromId.cpp
   
   server/automation/IRule.h
   server/automation/IRuleManager.h
   server/automation/IRuleThreadHandler.h
   server/automation/Rule.h
   server/automation/Rule.cpp
   server/automation/RuleException.hpp
   server/automation/RuleManager.h
   server/automation/RuleManager.cpp
   
   server/automation/interpreter/AvalaibleRequest.h
   server/automation/interpreter/AvalaibleRequest.cpp
   server/automation/interpreter/IFactory.h
   server/automation/interpreter/Factory.h
   server/automation/interpreter/Factory.cpp
   server/automation/interpreter/Information.h
   server/automation/interpreter/Information.cpp
   server/automation/interpreter/IInstance.h
   server/automation/interpreter/IManager.h
   server/automation/interpreter/Instance.h
   server/automation/interpreter/Instance.cpp
   server/automation/interpreter/InstanceStateHandler.h
   server/automation/interpreter/InstanceStateHandler.cpp
   server/automation/interpreter/IIpcAdapter.h
   server/automation/interpreter/IpcAdapter.h
   server/automation/interpreter/IpcAdapter.cpp
   server/automation/interpreter/LoadScriptContentRequest.h
   server/automation/interpreter/LoadScriptContentRequest.cpp
   server/automation/interpreter/Manager.h
   server/automation/interpreter/Manager.cpp
   server/automation/interpreter/PurgeScriptLog.h
   server/automation/interpreter/PurgeScriptLog.cpp
   server/automation/interpreter/SaveScriptContentRequest.h
   server/automation/interpreter/SaveScriptContentRequest.cpp
   server/automation/interpreter/StartScript.h
   server/automation/interpreter/StartScript.cpp
   server/automation/interpreter/StopScript.h
   server/automation/interpreter/StopScript.cpp
   server/automation/interpreter/yInterpreterApiImplementation.h
   server/automation/interpreter/yInterpreterApiImplementation.cpp

   server/automation/interpreter/serializers/IInformation.h
   server/automation/interpreter/serializers/Information.h
   server/automation/interpreter/serializers/Information.cpp
   
   server/automation/script/DayLightProvider.h
   server/automation/script/DayLightProvider.cpp
   server/automation/script/GeneralInfo.h
   server/automation/script/GeneralInfo.cpp
   server/automation/script/IDayLightProvider.h
   server/automation/script/IGeneralInfo.h
   server/automation/script/IIpcAdapter.h
   server/automation/script/IpcAdapter.h
   server/automation/script/IpcAdapter.cpp
   server/automation/script/IProperties.h
   server/automation/script/Properties.h
   server/automation/script/Properties.cpp
   server/automation/script/YScriptApiImplementation.h
   server/automation/script/YScriptApiImplementation.cpp


   server/communication/ISendMessageAsync.h
   server/communication/PluginGateway.h
   server/communication/PluginGateway.cpp   
   server/communication/callback/CallbackRequest.h
   server/communication/callback/NoDataCallbackRequest.h
   server/communication/callback/SynchronousCallback.h
   server/communication/callback/CallbackException.hpp
   server/communication/callback/ICallbackRequest.h
   server/communication/callback/ISynchronousCallback.h

   server/dataAccessLayer/AcquisitionHistorizer.h
   server/dataAccessLayer/AcquisitionHistorizer.cpp
   server/dataAccessLayer/ConfigurationManager.h
   server/dataAccessLayer/ConfigurationManager.cpp
   server/dataAccessLayer/DataAccessLayer.h
   server/dataAccessLayer/DataAccessLayer.cpp
   server/dataAccessLayer/DeviceManager.h
   server/dataAccessLayer/DeviceManager.cpp
   server/dataAccessLayer/EventLogger.h
   server/dataAccessLayer/EventLogger.cpp
   server/dataAccessLayer/KeywordManager.h
   server/dataAccessLayer/KeywordManager.cpp
   server/dataAccessLayer/IAcquisitionHistorizer.h
   server/dataAccessLayer/IConfigurationManager.h
   server/dataAccessLayer/IDataAccessLayer.h
   server/dataAccessLayer/IDeviceManager.h
   server/dataAccessLayer/IEventLogger.h
   server/dataAccessLayer/IKeywordManager.h
   
   server/database/DatabaseException.hpp
   server/database/IConfigurationRequester.h
   server/database/IAcquisitionRequester.h
   server/database/IDataProvider.h
   server/database/Factory.h
   server/database/Factory.cpp
   server/database/IDatabaseEngine.h
   server/database/IDatabaseRequester.h
   server/database/IDataBackup.h
   server/database/IDeviceRequester.h
   server/database/IEventLoggerRequester.h
   server/database/IRuleRequester.h
   server/database/IKeywordRequester.h
   server/database/IPageRequester.h
   server/database/IPluginEventLoggerRequester.h
   server/database/IPluginRequester.h
   server/database/IRecipientRequester.h
   server/database/ITableCreationScriptProvider.h
   server/database/ITransactionalProvider.h
   server/database/IWidgetRequester.h

   server/database/entities/Entities.h
   server/database/entities/Entities.cpp
   server/database/entities/EntityHelpers.hpp
  
   server/i18n/ClientStrings.h
   server/i18n/ClientStrings.cpp
  
   server/hardware/usb/DevicesListerFactory.h
   server/hardware/usb/IDevice.h
   server/hardware/usb/IDevicesLister.h
   
   server/location/IAutoLocation.h
   server/location/IpApiAutoLocation.h
   server/location/IpApiAutoLocation.cpp
   server/location/Location.h
   server/location/Location.cpp
   
   server/logging/YadomsSubModuleProcessLogger.h
   server/logging/YadomsSubModuleProcessLogger.cpp  
   server/logging/LogConfiguration.h
   server/logging/LogConfiguration.cpp   
   server/logging/LogConfigurationImpl.h
   server/logging/LogConfigurationImpl.cpp
   server/logging/LogConfigurationException.h
   
   server/pluginSystem/BasicQualifier.h
   server/pluginSystem/BasicQualifier.cpp
   server/pluginSystem/BindingQueryData.cpp
   server/pluginSystem/BindingQueryData.h
   server/pluginSystem/BindingQueryRequest.cpp
   server/pluginSystem/BindingQueryRequest.h
   server/pluginSystem/CommandHistorizer.h
   server/pluginSystem/CommandHistorizer.cpp
   server/pluginSystem/DeviceCommand.h
   server/pluginSystem/DeviceCommand.cpp
   server/pluginSystem/DeviceConfigurationSchemaRequest.h
   server/pluginSystem/DeviceConfigurationSchemaRequest.cpp
   server/pluginSystem/DeviceRemoved.h
   server/pluginSystem/DeviceRemoved.cpp
   server/pluginSystem/DummyQualifier.h
   server/pluginSystem/DummyQualifier.cpp
   server/pluginSystem/ExtraQuery.h
   server/pluginSystem/ExtraQuery.cpp
   server/pluginSystem/ExtraQueryData.cpp
   server/pluginSystem/ExtraQueryData.h
   server/pluginSystem/Factory.cpp
   server/pluginSystem/Factory.h
   server/pluginSystem/FromPluginHistorizer.h
   server/pluginSystem/FromPluginHistorizer.cpp
   server/pluginSystem/IdentityForQualifier.h
   server/pluginSystem/IdentityForQualifier.cpp
   server/pluginSystem/IFactory.h
   server/pluginSystem/IInstance.h
   server/pluginSystem/IInstanceStartErrorObserver.h
   server/pluginSystem/IInstanceStateHandler.h
   server/pluginSystem/IIpcAdapter.h
   server/pluginSystem/IndicatorQualifier.h
   server/pluginSystem/IndicatorQualifier.cpp
   server/pluginSystem/Information.h
   server/pluginSystem/Information.cpp
   server/pluginSystem/Instance.h
   server/pluginSystem/Instance.cpp
   server/pluginSystem/InstanceStateHandler.h
   server/pluginSystem/InstanceStateHandler.cpp
   server/pluginSystem/InvalidPluginException.hpp
   server/pluginSystem/IpcAdapter.h
   server/pluginSystem/IpcAdapter.cpp
   server/pluginSystem/IQualifier.h
   server/pluginSystem/Manager.h
   server/pluginSystem/Manager.cpp
   server/pluginSystem/ManuallyDeviceCreationData.cpp
   server/pluginSystem/ManuallyDeviceCreationData.h
   server/pluginSystem/ManuallyDeviceCreationRequest.h
   server/pluginSystem/ManuallyDeviceCreationRequest.cpp
   server/pluginSystem/PluginException.hpp
   server/pluginSystem/SetDeviceConfiguration.h
   server/pluginSystem/SetDeviceConfiguration.cpp
   server/pluginSystem/yPluginApiImplementation.h
   server/pluginSystem/yPluginApiImplementation.cpp
   server/pluginSystem/YadomsInformation.h
   server/pluginSystem/YadomsInformation.cpp
   
   server/pluginSystem/internalPlugin/CustomEnumHistorizer.h
   server/pluginSystem/internalPlugin/CustomEnumHistorizer.cpp
   server/pluginSystem/internalPlugin/CustomEnumTypeInfo.h
   server/pluginSystem/internalPlugin/CustomEnumTypeInfo.cpp
   server/pluginSystem/internalPlugin/Information.h
   server/pluginSystem/internalPlugin/Information.cpp
   server/pluginSystem/internalPlugin/Instance.h
   server/pluginSystem/internalPlugin/Instance.cpp

   server/pluginSystem/serializers/IInformation.h
   server/pluginSystem/serializers/Information.h
   server/pluginSystem/serializers/Information.cpp
   
   server/notification/NotificationCenter.h
   server/notification/NotificationCenter.cpp
   server/notification/Helpers.hpp
   server/notification/INotification.h
   server/notification/IObserver.h
   
   server/notification/action/IAction.h
   server/notification/action/EventAction.hpp
   server/notification/action/FunctionPointerAction.hpp
   server/notification/action/WaitAction.hpp
   
   server/notification/basic/Observer.hpp
   server/notification/basic/Notification.hpp
   
   server/notification/change/Observer.hpp
   server/notification/change/Notification.hpp
   server/notification/change/Type.h
   server/notification/change/Type.cpp

   server/notification/acquisition/Observer.hpp
   server/notification/acquisition/Notification.hpp
   
   server/notification/summary/Notification.hpp
   
   server/startupOptions/DatabaseEngine.h
   server/startupOptions/DatabaseEngine.cpp
   server/startupOptions/EnumValidator.h
   server/startupOptions/ListValidator.h
   server/startupOptions/IStartupOptions.h
   server/startupOptions/StartupOptions.h
   server/startupOptions/StartupOptions.cpp
   server/startupOptions/MustExistPathValidator.h
   server/startupOptions/MustExistPathValidator.cpp
   
   server/task/FinishedInstance.h
   server/task/FinishedInstance.cpp
   server/task/GenericTask.h
   server/task/GenericTask.cpp
   server/task/IInstance.h
   server/task/Instance.h
   server/task/Instance.cpp
   server/task/InstanceNotificationData.h
   server/task/InstanceNotificationData.cpp
   server/task/ITask.h
   server/task/IUnique.h
   server/task/Scheduler.h
   server/task/Scheduler.cpp
   server/task/TaskEvent.h
   server/task/TaskEvent.cpp
   server/task/TaskFactory.h
   server/task/TaskFactory.cpp
   server/task/TaskStatus.h
   server/task/TaskStatus.cpp
   server/task/backup/Backup.h
   server/task/backup/Backup.cpp
   server/task/exportData/ExportData.h
   server/task/exportData/ExportData.cpp
   server/task/packLogs/PackLogs.h
   server/task/packLogs/PackLogs.cpp
   server/task/plugins/ExtraQuery.h
   server/task/plugins/ExtraQuery.cpp
   
   
   server/update/IUpdateManager.h
   server/update/UpdateManager.h
   server/update/UpdateManager.cpp
   server/update/UpdateType.h
   server/update/UpdateType.cpp
   server/update/info/UpdateSite.h
   server/update/info/UpdateSite.cpp
   server/update/worker/IWidgetInformation.h
   server/update/worker/Plugin.h
   server/update/worker/Plugin.cpp
   server/update/worker/ScriptInterpreter.h
   server/update/worker/ScriptInterpreter.cpp
   server/update/worker/Widget.h
   server/update/worker/Widget.cpp
   server/update/worker/WidgetInformation.h
   server/update/worker/WidgetInformation.cpp
   server/update/worker/Yadoms.h
   server/update/worker/Yadoms.cpp
   server/update/worker/WorkerHelpers.h
   server/update/worker/WorkerHelpers.cpp
   
   server/web/IRestHandler.h
   server/web/IWebServer.h
   server/web/IWebServerConfigurator.h
 
   server/web/rest/RestDispatcher.h
   server/web/rest/RestDispatcher.cpp
   server/web/rest/RestDispatcherHelpers.hpp
   server/web/rest/Result.h
   server/web/rest/Result.cpp
   server/web/rest/StringContainer.h
   server/web/rest/StringContainer.cpp

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
   server/web/rest/service/Maintenance.h
   server/web/rest/service/Maintenance.cpp
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
   server/web/rest/service/Update.h
   server/web/rest/service/Update.cpp
   server/web/rest/service/Widget.h
   server/web/rest/service/Widget.cpp
   
   server/web/ws/AcquisitionFilterFrame.h
   server/web/ws/AcquisitionFilterFrame.cpp
   server/web/ws/AcquisitionUpdateFrame.h
   server/web/ws/AcquisitionUpdateFrame.cpp
   server/web/ws/AcquisitionSummaryUpdateFrame.h
   server/web/ws/AcquisitionSummaryUpdateFrame.cpp
   server/web/ws/DeviceBlackListedFrame.h
   server/web/ws/DeviceBlackListedFrame.cpp
   server/web/ws/DeviceDeletedFrame.h
   server/web/ws/DeviceDeletedFrame.cpp
   server/web/ws/KeywordDeletedFrame.h
   server/web/ws/KeywordDeletedFrame.cpp
   server/web/ws/KeywordNewFrame.h
   server/web/ws/KeywordNewFrame.cpp
   server/web/ws/FrameBase.h
   server/web/ws/FrameBase.cpp
   server/web/ws/FrameFactory.h
   server/web/ws/FrameFactory.cpp
   server/web/ws/IsAliveFrame.h
   server/web/ws/IsAliveFrame.cpp
   server/web/ws/LogEventFrame.h
   server/web/ws/LogEventFrame.cpp
   server/web/ws/NewDeviceFrame.h
   server/web/ws/NewDeviceFrame.cpp
   server/web/ws/TaskUpdateNotificationFrame.h
   server/web/ws/TaskUpdateNotificationFrame.cpp
   server/web/ws/TimeNotificationFrame.h
   server/web/ws/TimeNotificationFrame.cpp
)

# Project folders
source_group(server server/*.*)
source_group(server\\authentication  server/authentication/*.*)
source_group(server\\automation  server/automation/*.*)
source_group(server\\automation\\interpreter  server/automation/interpreter/*.*)
source_group(server\\automation\\interpreter\\serializers server/automation/interpreter/serializers/*.* )
source_group(server\\automation\\script  server/automation/script/*.*)
source_group(server\\communication  server/communication/*.*)
source_group(server\\communication\\callback  server/communication/callback/*.*)
source_group(server\\dataAccessLayer  server/dataAccessLayer/*.*)
source_group(server\\database  server/database/*.*)
source_group(server\\database\\entities  server/database/entities/*.*)
source_group(server\\dateTime  server/dateTime/*.*)
source_group(server\\i18n  server/i18n/*.*)
source_group(server\\hardware\\usb  server/hardware/usb/*.*)
source_group(server\\hardware\\usb\\lsusb  server/hardware/usb/lsusb/*.*)
source_group(server\\hardware\\usb\\winapi  server/hardware/usb/winapi/*.*)
source_group(server\\location  server/location/*.*)
source_group(server\\logging  server/logging/*.*)
source_group(server\\notification  server/notification/*.*)
source_group(server\\notification\\action  server/notification/action/*.*)
source_group(server\\notification\\basic  server/notification/basic/*.*)
source_group(server\\notification\\change  server/notification/change/*.*)
source_group(server\\notification\\acquisition  server/notification/acquisition/*.*)
source_group(server\\notification\\summary  server/notification/summary/*.*)
source_group(server\\pluginSystem server/pluginSystem/*.* )
source_group(server\\pluginSystem\\serializers server/pluginSystem/serializers/*.* )
source_group(server\\pluginSystem\\internalPlugin server/pluginSystem/internalPlugin/*.* )
source_group(server\\startupOptions  server/startupOptions/*.*)
source_group(server\\task  server/task/*.*)
source_group(server\\task\\backup  server/task/backup/*.*)
source_group(server\\task\\packLogs  server/task/packLogs/*.*)
source_group(server\\task\\plugins  server/task/plugins/*.*)
source_group(server\\update  server/update/*.*)
source_group(server\\update\\info  server/update/info/*.*)
source_group(server\\update\\worker  server/update/worker/*.*)
source_group(server\\web  server/web/*.*)
source_group(server\\web\\rest  server/web/rest/*.*)
source_group(server\\web\\rest\\json  server/web/rest/json/*.*)
source_group(server\\web\\rest\\service  server/web/rest/service/*.*)
source_group(server\\web\\ws  server/web/ws/*.*)

# Tools
set(YADOMS_SRC ${YADOMS_SRC}
   server/tools/SupportedPlatformsChecker.h
   server/tools/SupportedPlatformsChecker.cpp
   server/tools/FileSystem.h
   server/tools/FileSystem.cpp
)

IF(WIN32)
   set(YADOMS_SRC ${YADOMS_SRC}
      # Add here server/tools/windows/tools/ sources
      server/tools/windows/tools/OperatingSystem.h
      server/tools/windows/tools/OperatingSystem.cpp
      server/hardware/serial/windows/SerialPortsLister.h
      server/hardware/serial/windows/SerialPortsLister.cpp
      server/hardware/usb/winapi/DevicesListerFactory.cpp
      server/hardware/usb/winapi/WinapiDevice.h
      server/hardware/usb/winapi/WinapiDevice.cpp
      server/hardware/usb/winapi/WinapiDevicesLister.h
      server/hardware/usb/winapi/WinapiDevicesLister.cpp
   )   
ELSEIF(APPLE)
   set(YADOMS_SRC ${YADOMS_SRC}
      # Add here server/tools/mac/tools/ sources
      server/tools/mac/tools/OperatingSystem.h
      server/tools/mac/tools/OperatingSystem.cpp
      server/hardware/serial/mac/SerialPortsLister.h
      server/hardware/serial/mac/SerialPortsLister.cpp
      server/hardware/usb/lsusb/LsusbCall.h
      server/hardware/usb/lsusb/LsusbCall.cpp
      server/hardware/usb/lsusb/LsusbDevice.h
      server/hardware/usb/lsusb/LsusbDevice.cpp
      server/hardware/usb/lsusb/LsusbDevicesLister.h
      server/hardware/usb/lsusb/LsusbDevicesLister.cpp
   )   
ELSE()
   set(YADOMS_SRC ${YADOMS_SRC}
      # Add here server/tools/linux/tools/ sources
      server/tools/linux/tools/OperatingSystem.h
      server/tools/linux/tools/OperatingSystem.cpp
      server/hardware/serial/linux/SerialPortsLister.h
      server/hardware/serial/linux/SerialPortsLister.cpp
      server/hardware/usb/lsusb/DevicesListerFactory.cpp
      server/hardware/usb/lsusb/LsusbCall.h
      server/hardware/usb/lsusb/LsusbCall.cpp
      server/hardware/usb/lsusb/LsusbDevice.h
      server/hardware/usb/lsusb/LsusbDevice.cpp
      server/hardware/usb/lsusb/LsusbDevicesLister.h
      server/hardware/usb/lsusb/LsusbDevicesLister.cpp
   )   
ENDIF()

# Define include directories
IF(WIN32)
   set(YADOMS_INCL_DIR ${YADOMS_INCL_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/server/tools/windows)
   set(YADOMS_INCL_DIR ${YADOMS_INCL_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/server/hardware/serial/windows)
   set(YADOMS_INCL_DIR ${YADOMS_INCL_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/server/hardware/usb/winapi)
ELSEIF(APPLE)
   set(YADOMS_INCL_DIR ${YADOMS_INCL_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/server/tools/mac)
   set(YADOMS_INCL_DIR ${YADOMS_INCL_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/server/hardware/serial/mac)
   set(YADOMS_INCL_DIR ${YADOMS_INCL_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/server/hardware/usb/lsusb)
ELSE()
   set(YADOMS_INCL_DIR ${YADOMS_INCL_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/server/tools/linux)
   set(YADOMS_INCL_DIR ${YADOMS_INCL_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/server/hardware/serial/linux)
   set(YADOMS_INCL_DIR ${YADOMS_INCL_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/server/hardware/usb/lsusb)
ENDIF()

source_group(server\\tools tools/*.*)
source_group(server\\tools\\platformSpecific\\windows server/tools/windows/tools/*.*)
source_group(server\\tools\\platformSpecific\\windows\\service server/tools/windows/tools/service/*.*)
source_group(server\\tools\\platformSpecific\\linux server/tools/linux/tools/*.*)
source_group(server\\tools\\platformSpecific\\mac server/tools/mac/tools/*.*)
source_group(server\\hardware\\serial\\windows server/hardware/serial/windows/*.*)
source_group(server\\hardware\\serial\\linux server/hardware/serial/linux/*.*)
source_group(server\\hardware\\serial\\mac server/hardware/serial/mac/*.*)

################################################
#### Database (common)
################################################
set(YADOMS_SRC ${YADOMS_SRC}
   server/database/common/Query.h
   server/database/common/Query.cpp
   server/database/common/QuerySpecializations.h
   server/database/common/DataProvider.h
   server/database/common/DataProvider.cpp
   server/database/common/DatabaseColumn.h
   server/database/common/DatabaseColumn.cpp
   server/database/common/DatabaseTables.h
   server/database/common/DatabaseTables.cpp
   server/database/common/DatabaseTablesHelpers.hpp
   server/database/common/SummaryDataTask.h
   server/database/common/SummaryDataTask.cpp      
   server/database/common/PurgeTask.h
   server/database/common/PurgeTask.cpp      
   server/database/common/adapters/AdapterHelpers.hpp
   server/database/common/adapters/DatabaseAdapters.h
   server/database/common/adapters/DatabaseAdapters.cpp
   server/database/common/adapters/GenericAdapter.h
   server/database/common/adapters/GenericAdapter.cpp
   server/database/common/adapters/HugeDataVectorAdapter.h
   server/database/common/adapters/HugeDataVectorAdapter.cpp
   server/database/common/adapters/IResultAdapter.h
   server/database/common/adapters/MultipleValueAdapter.hpp
   server/database/common/adapters/SingleValueAdapter.hpp
   server/database/common/adapters/SqlExtension.hpp
   server/database/common/requesters/Plugin.h
   server/database/common/requesters/Plugin.cpp
   server/database/common/requesters/Configuration.h
   server/database/common/requesters/Configuration.cpp
   server/database/common/requesters/Keyword.h
   server/database/common/requesters/Keyword.cpp
   server/database/common/requesters/Device.h
   server/database/common/requesters/Device.cpp      
   server/database/common/requesters/Rule.h
   server/database/common/requesters/Rule.cpp      
   server/database/common/requesters/Page.h
   server/database/common/requesters/Page.cpp      
   server/database/common/requesters/Recipient.h
   server/database/common/requesters/Recipient.cpp      
   server/database/common/requesters/Widget.h
   server/database/common/requesters/Widget.cpp      
   server/database/common/requesters/PluginEventLogger.h
   server/database/common/requesters/PluginEventLogger.cpp
   server/database/common/requesters/EventLogger.h
   server/database/common/requesters/EventLogger.cpp
   server/database/common/requesters/Acquisition.h
   server/database/common/requesters/Acquisition.cpp
   server/database/common/versioning/IVersionUpgrade.h
   server/database/common/versioning/Version_1_0_0.h
   server/database/common/versioning/Version_1_0_0.cpp
   server/database/common/versioning/Version_2_0_0.h
   server/database/common/versioning/Version_2_0_0.cpp
   server/database/common/versioning/Version_3_0_0.h
   server/database/common/versioning/Version_3_0_0.cpp
   server/database/common/versioning/Version_3_0_1.h
   server/database/common/versioning/Version_3_0_1.cpp
   server/database/common/versioning/Version_4_0_0.h
   server/database/common/versioning/Version_4_0_0.cpp
   server/database/common/versioning/Version_4_0_1.h
   server/database/common/versioning/Version_4_0_1.cpp
   server/database/common/versioning/Version_4_1_0.h
   server/database/common/versioning/Version_4_1_0.cpp
   server/database/common/versioning/Version_4_2_0.h
   server/database/common/versioning/Version_4_2_0.cpp
   server/database/common/versioning/Version_4_3_0.h
   server/database/common/versioning/Version_4_3_0.cpp
   server/database/common/versioning/Version_4_4_0.h
   server/database/common/versioning/Version_4_4_0.cpp
   server/database/common/versioning/Version_5_0_0.h
   server/database/common/versioning/Version_5_0_0.cpp
   server/database/common/versioning/Version_5_1_0.h
   server/database/common/versioning/Version_5_1_0.cpp
   server/database/common/versioning/Version_5_2_0.h
   server/database/common/versioning/Version_5_2_0.cpp
   server/database/common/versioning/VersionUpgraderFactory.h
   server/database/common/versioning/VersionUpgraderFactory.cpp
   server/database/common/versioning/VersionException.h
                   
   server/database/common/IResultHandler.h
)

source_group(server\\database\\common  server/database/common/*.*)
source_group(server\\database\\common\\adapters  server/database/common/adapters/*.*)
source_group(server\\database\\common\\requesters  server/database/common/requesters/*.*)
source_group(server\\database\\common\\versioning  server/database/common/versioning/*.*)

################################################
#### Database (SQlite)
################################################

IF(USE_SQLITE_DATABASE)
   set(YADOMS_SRC ${YADOMS_SRC}
      server/database/sqlite/SQLiteQuery.h
      server/database/sqlite/SQLiteQuery.cpp
      server/database/sqlite/SQLiteRequester.h
      server/database/sqlite/SQLiteRequester.cpp
      server/database/sqlite/SQLiteResultHandler.h
      server/database/sqlite/SQLiteResultHandler.cpp
      server/database/sqlite/SQLiteSystemTables.h
      server/database/sqlite/SQLiteSystemTables.cpp
      server/database/sqlite/SQLiteTableCreationScriptProvider.h
      server/database/sqlite/SQLiteTableCreationScriptProvider.cpp
   )
   
   source_group(server\\database\\sqlite  server/database/sqlite/*.*)
   
   set(YADOMS_EXTERNAL_LINK ${YADOMS_EXTERNAL_LINK} sqlite)
   set(YADOMS_INCL_DIR ${YADOMS_INCL_DIR} ${SQLITE_INCLUDE_DIRECTORY})
ENDIF()

################################################
#### Database (PostgreSQL)
################################################

IF(USE_PGSQL_DATABASE)
   # Be careful updating PostgreSQL version : you maybe have to update ssleay32.dll dependency (MSVCRxxx.DLL)
   find_package(PostgreSQL 9.6)

   if(PostgreSQL_FOUND)
      message(STATUS "Found postgresql ${PostgreSQL_VERSION_STRING}")

      set(YADOMS_SRC ${YADOMS_SRC}
         server/database/pgsql/PgsqlLibrary.h
         server/database/pgsql/PgsqlLibrary.cpp
         server/database/pgsql/PgsqlQuery.h
         server/database/pgsql/PgsqlQuery.cpp
         server/database/pgsql/PgsqlRequester.h
         server/database/pgsql/PgsqlRequester.cpp
         server/database/pgsql/PgsqlResultHandler.h
         server/database/pgsql/PgsqlResultHandler.cpp
         server/database/pgsql/PgsqlSqlState.h
         server/database/pgsql/PgsqlSqlState.cpp
         server/database/pgsql/PgsqlSystemTables.h
         server/database/pgsql/PgsqlSystemTables.cpp
         server/database/pgsql/PgsqlTableCreationScriptProvider.h
         server/database/pgsql/PgsqlTableCreationScriptProvider.cpp
      )
      
      source_group(server\\database\\pgsql  server/database/pgsql/*.*)
   
      IF(WIN32)
         #add PostgreSQL as dependencies
         IF(EXISTS "${PostgreSQL_ROOT}/bin/libpq.dll")
            SET(PostgreSqlDepends "${PostgreSQL_ROOT}/bin/libpq.dll")
         ENDIF()
         
         IF(EXISTS "${PostgreSQL_ROOT}/bin/ssleay32.dll")
            SET(PostgreSqlDepends ${PostgreSqlDepends} "${PostgreSQL_ROOT}/bin/ssleay32.dll")
            SET(PostgreSqlDepends ${PostgreSqlDepends} "${CMAKE_SOURCE_DIR}/external-libs/msvcr120.dll")
         ENDIF()
         
         IF(EXISTS "${PostgreSQL_ROOT}/bin/libeay32.dll")
            SET(PostgreSqlDepends ${PostgreSqlDepends} "${PostgreSQL_ROOT}/bin/libeay32.dll")
         ENDIF()

         IF(EXISTS "${PostgreSQL_ROOT}/bin/intl.dll")
            SET(PostgreSqlDepends ${PostgreSqlDepends} "${PostgreSQL_ROOT}/bin/intl.dll")
         ENDIF()

         IF(EXISTS "${PostgreSQL_ROOT}/bin/libintl-8.dll")
            SET(PostgreSqlDepends ${PostgreSqlDepends} "${PostgreSQL_ROOT}/bin/libintl-8.dll")
         ENDIF()

         IF(EXISTS "${PostgreSQL_ROOT}/bin/libiconv-2.dll")
            SET(PostgreSqlDepends ${PostgreSqlDepends} "${PostgreSQL_ROOT}/bin/libiconv-2.dll")
         ENDIF()
      ENDIF(WIN32)
   
      set(YADOMS_INCL_DIR ${YADOMS_INCL_DIR} ${PostgreSQL_INCLUDE_DIRS})
   else()
      message(WARNING "PostgreSQL not found. Only SQLite will be available")
      add_definitions( -DPGSQL_NOT_FOUND )
   ENDIF(PostgreSQL_FOUND)   
ENDIF(USE_PGSQL_DATABASE)

#Web server
IF(USE_POCO_WEBSERVER)
   set(YADOMS_SRC ${YADOMS_SRC}
      server/web/poco/WebServer.h
      server/web/poco/WebServer.cpp
      server/web/poco/AuthenticationRequestHandler.h
      server/web/poco/AuthenticationRequestHandler.cpp
      server/web/poco/HttpRequestHandlerFactory.h
      server/web/poco/HttpRequestHandlerFactory.cpp   
      server/web/poco/HttpRequestHandlerContainer.h
      server/web/poco/HttpRequestHandlerContainer.cpp
      server/web/poco/RestRequestHandler.h
      server/web/poco/RestRequestHandler.cpp
      server/web/poco/WebsiteCacheManager.h
      server/web/poco/WebsiteCacheManager.cpp
      server/web/poco/WebsiteRequestHandler.h
      server/web/poco/WebsiteRequestHandler.cpp
      server/web/poco/WebSocketRequestHandler.h
      server/web/poco/WebSocketRequestHandler.cpp   
      server/web/poco/MimeType.h
      server/web/poco/MimeType.cpp   
   )
   
   source_group(server\\web\\poco  server/web/poco/*.*)
ENDIF()


##################################################################################################
## Resources
##################################################################################################
if(WIN32)

   ##################################################################################################
   ## Pre build commands (only visual studio)
   ##################################################################################################
   if(MSVC)
      #update the Windows specific 'yadoms.rc' file which add properties to executable (version, releaseType=
      #as pre build step (yadoms.rc is modified only if needed, to avoid unjustified build

      # apply templating to the manifest for setting the version
      configure_file(${CMAKE_SOURCE_DIR}/resources/windows/yadoms.rc.in
         "${CMAKE_BINARY_DIR}/yadoms-generated.rc"
      @ONLY)
         
      # If we build for windows systems, we also include the resource file
      # containing the manifest, icon and other resources
      set(YADOMS_SRC ${YADOMS_SRC} 
         ${CMAKE_BINARY_DIR}/yadoms-generated.rc
         resources/windows/resource.h
      )
      source_group(resources resources/windows/*.*)
   endif(MSVC)
endif(WIN32)


# Add changelog file
set(YADOMS_SRC ${YADOMS_SRC} 
   server/changelog.md
)

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
# IPC messages static library for plugins
add_subdirectory(plugin_IPC)

# C++ API for plugins
add_subdirectory(plugin_cpp_api)

# Plugins
add_subdirectory(plugins)


##################################################################################################
## Script interpreters
##################################################################################################
# IPC messages static library for interpreters
add_subdirectory(interpreter_IPC)

# C++ API for interpreters
add_subdirectory(interpreter_cpp_api)

# IPC messages static library for scripts
add_subdirectory(script_IPC)

# C++ API for script wrappers
add_subdirectory(script_cpp_api)

# Interpreters
add_subdirectory(scriptInterpreters)



##################################################################################################
## Include directories
##################################################################################################
set(YADOMS_INCL_DIR
   ${YADOMS_INCL_DIR}
   ${SHARED_INCL_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}/server
   ${plugin_IPC_INCLUDE_DIRS}
   ${interpreter_IPC_INCLUDE_DIRS}
   ${script_IPC_INCLUDE_DIRS}
)

#setup include dirs. Add protobuf include dirs in first lines to make xcode-clang work)
# include OPENSSL_INCLUDE_DIR before YADOMS_INCL_DIR because YADOMS_INCL_DIR include postgresql which may embed its own openssl files (not used)
# but this may infer in some compilation cases
set(YADOMS_ALL_INCL_DIR
   ${PROTOBUF_INCLUDE_DIRS}
   ${BOOST_INCL_DIR} 
   ${OPENSSL_INCLUDE_DIR}
   ${Poco_INCLUDE_DIRS}
   ${YADOMS_INCL_DIR} 
)

set_property(TARGET yadoms PROPERTY INCLUDE_DIRECTORIES ${YADOMS_ALL_INCL_DIR})

##################################################################################################
## Link
##################################################################################################
target_link_libraries(yadoms yadoms-shared plugin_IPC interpreter_IPC script_IPC ${YADOMS_EXTERNAL_LINK} ${LIBS} ${CMAKE_DL_LIBS})

##################################################################################################
## RPATH
##################################################################################################
if(CMAKE_CROSSCOMPILING)
   #Fix RPATH for cross compilation of Yadoms
   set_target_properties(yadoms PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)
endif(CMAKE_CROSSCOMPILING)
                       
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
      set_property( TARGET yadoms_unity PROPERTY INCLUDE_DIRECTORIES ${YADOMS_ALL_INCL_DIR})
       if(CMAKE_CROSSCOMPILING)
          #Fix RPATH for cross compilation of yadoms_unity
          set_target_properties(yadoms_unity PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)
       endif(CMAKE_CROSSCOMPILING)
   endif()   
endif()
      
      
##################################################################################################
## Manifest
## Allow Windows10 detection ability by Poco
##################################################################################################
if(WIN32)
   if(MSVC)
   
      #prepare version to be used in windows manifest (only 3 digits)
      set(YADOMS_VERSION_FOR_MANIFEST "${YADOMS_VERSION_MAJOR}.${YADOMS_VERSION_MINOR}.${YADOMS_VERSION_PATCH}")

   
      # apply templating to the manifest for setting the version
      configure_file(${CMAKE_SOURCE_DIR}/../distribution/yadoms.exe.manifest.in
         "${CMAKE_BINARY_DIR}/yadoms.exe.manifest"
      @ONLY)
      
      #add a custom command
      add_custom_command(TARGET yadoms POST_BUILD
          COMMAND "mt.exe" -manifest \"${CMAKE_BINARY_DIR}\\yadoms.exe.manifest\" -outputresource:"$(TargetDir)$(TargetFileName)"\;\#1
          COMMENT "Adding manifest..." 
      )
      
   if(COTIRE_USE)   
      if(COTIRE_USE_UNITY)
         add_custom_command(TARGET yadoms_unity POST_BUILD
             COMMAND "mt.exe" -manifest \"${CMAKE_BINARY_DIR}\\yadoms.exe.manifest\" -outputresource:"$(TargetDir)$(TargetFileName)"\;\#1
             COMMENT "Adding manifest..." 
         )
      endif(COTIRE_USE_UNITY)
   endif(COTIRE_USE)
      
   endif(MSVC)
endif(WIN32)

##################################################################################################
## Post build commands
##################################################################################################

#copy Poco depends
FOREACH (PocoNetSSLDepends ${PocoNetSSLDepends})
   add_custom_command(TARGET yadoms POST_BUILD
                      COMMAND ${CMAKE_COMMAND} -E copy_if_different
                          ${PocoNetSSLDepends} $<TARGET_FILE_DIR:yadoms>/)
                          
  if(COTIRE_USE)   
     if(COTIRE_USE_UNITY)
        add_custom_command(TARGET yadoms_unity POST_BUILD
                           COMMAND ${CMAKE_COMMAND} -E copy_if_different
                           ${PocoNetSSLDepends} $<TARGET_FILE_DIR:yadoms_unity>/)
     endif(COTIRE_USE_UNITY)
  endif(COTIRE_USE)                          
ENDFOREACH(PocoNetSSLDepends)

#copy PostgreSql depends
FOREACH (PostgreSqlDepend ${PostgreSqlDepends})
   add_custom_command(TARGET yadoms POST_BUILD
                      COMMAND ${CMAKE_COMMAND} -E copy_if_different
                          ${PostgreSqlDepend} $<TARGET_FILE_DIR:yadoms>/)

  if(COTIRE_USE)   
     if(COTIRE_USE_UNITY)
        add_custom_command(TARGET yadoms_unity POST_BUILD
                           COMMAND ${CMAKE_COMMAND} -E copy_if_different
                               ${PostgreSqlDepend} $<TARGET_FILE_DIR:yadoms_unity>/)
     endif(COTIRE_USE_UNITY)
  endif(COTIRE_USE)                          
                          
ENDFOREACH(PostgreSqlDepend)

#copy www directory into build directory
add_custom_command(TARGET yadoms POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_SOURCE_DIR}/www $<TARGET_FILE_DIR:yadoms>/www/)
      
if(COTIRE_USE)   
   if(COTIRE_USE_UNITY)
      add_custom_command(TARGET yadoms_unity POST_BUILD
                         COMMAND ${CMAKE_COMMAND} -E copy_directory
                         ${CMAKE_SOURCE_DIR}/www $<TARGET_FILE_DIR:yadoms_unity>/www/)
   endif(COTIRE_USE_UNITY)
endif(COTIRE_USE)                          

##################################################################################################
## CPACK
##################################################################################################


#set yadoms executable to be copied in bindir
install(TARGETS yadoms RUNTIME DESTINATION ${INSTALL_BINDIR})

#set yadoms.ini configuration file to be copied in bindir
install(FILES ${CMAKE_SOURCE_DIR}/../config/yadoms.ini
   DESTINATION ${INSTALL_BINDIR})

#Configure all building targets to allow finding dependencies next to them
#so just copying boost and poco next to yadoms executable and it will work
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE) 
if(UNIX AND NOT APPLE)
   set(CMAKE_EXE_LINKER_FLAGS "-Wl,-rpath='$ORIGIN' ")
endif()
   

#add depends
if(USE_PGSQL_DATABASE AND PostgreSQL_FOUND)
   FOREACH (PostgreSqlDepend ${PostgreSqlDepends})
      if (DEFINED CMAKE_CONFIGURATION_TYPES)
         install(FILES ${PostgreSqlDepend} CONFIGURATIONS Debug;Release;RelWithDebInfo;MinSizeRel RUNTIME DESTINATION ${INSTALL_BINDIR})
      else()
         install(FILES ${PostgreSqlDepend} RUNTIME DESTINATION ${INSTALL_BINDIR})
      endif()
   ENDFOREACH(PostgreSqlDepend)
endif()


#add www directory
install(DIRECTORY ${CMAKE_SOURCE_DIR}/www 
   DESTINATION ${INSTALL_BINDIR}
   PATTERN "widgets/dev-*" EXCLUDE)


set(CPACK_COMPONENT_YADOMSCORE_DISPLAY_NAME "Yadoms Server")
set(CPACK_COMPONENT_YADOMSCORE_REQUIRED ON)
set(CPACK_COMPONENT_YADOMSCORE_DESCRIPTION "Yadoms core server software components.")

set(CPACK_PACKAGE_NAME "Yadoms")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "The ultimate house automation solution")
set(CPACK_PACKAGE_VENDOR "Yadoms")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/../distribution/Readme.txt")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/../distribution/Copyright.txt")
set(CPACK_PACKAGE_VERSION_MAJOR ${YADOMS_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${YADOMS_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${YADOMS_VERSION_PATCH})
if(CMAKE_PACKAGE_PLATFORM_NAME)
   set(CPACK_SYSTEM_NAME ${CMAKE_PACKAGE_PLATFORM_NAME})
else(CMAKE_PACKAGE_PLATFORM_NAME)
   set(CPACK_SYSTEM_NAME ${CMAKE_SYSTEM_NAME})
endif(CMAKE_PACKAGE_PLATFORM_NAME)
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${YADOMS_VERSION}-${CPACK_SYSTEM_NAME}") 
set(CPACK_PACKAGE_INSTALL_DIRECTORY "Yadoms")
set(CPACK_PACKAGE_CONTACT "Jean-Michel DECORET <decoretjm@gmail.com>")

set(CPACK_COMPONENTS_ALL_IN_ONE_PACKAGE 0)

   
#configure the plugin to be grouped in sub components
FOREACH (PLUGIN ${PLUGINLIST})
   string(TOUPPER ${PLUGIN} _pluginNameUpperCase)
   string(REPLACE "-" "_" pluginCompatibleName ${_pluginNameUpperCase})
   set(CPACK_COMPONENT_${pluginCompatibleName}_GROUP "Plugins")
ENDFOREACH(PLUGIN)
set(CPACK_COMPONENT_GROUP_PLUGINS_DISPLAY_NAME  "Plugins")
set(CPACK_COMPONENT_GROUP_PLUGINS_DESCRIPTION  "All the plugins which control hardware components")

#configure the script interpreters to be grouped in sub components
FOREACH (SCRIPTINTERPRETER ${SCRIPTINTERPRETERSLIST})
   string(TOUPPER ${SCRIPTINTERPRETER} _scriptInterpreterNameUpperCase)
   string(REPLACE "-" "_" scriptInterpreterCompatibleName ${_scriptInterpreterNameUpperCase})
   set(CPACK_COMPONENT_${scriptInterpreterCompatibleName}_GROUP "ScriptInterpreters")
ENDFOREACH(SCRIPTINTERPRETER)
set(CPACK_COMPONENT_GROUP_SCRIPTINTERPRETERS_DISPLAY_NAME  "Script interpreters")
set(CPACK_COMPONENT_GROUP_SCRIPTINTERPRETERS_DESCRIPTION  "All the script interpreters (allow to run custom scripts)")

IF(WIN32 AND NOT UNIX)

   # Custom target for packaging.
   if(USE_NSIS)
      set(CPACK_GENERATOR "NSIS;ZIP")
     
      # There is a bug in NSI that does not handle full unix paths properly. Make
      # sure there is at least one set of four (4) backlasshes.
      SET(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/../distribution\\\\Yadoms.bmp")

      set(CPACK_NSIS_PLUGINS_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../distribution/NSIS/extensions")
      set(CPACK_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../distribution/NSIS")
      SET(CPACK_NSIS_INSTALLED_ICON_NAME "${INSTALL_BINDIR}\\\\yadoms.exe")
      SET(CPACK_NSIS_MUI_ICON "${CMAKE_CURRENT_SOURCE_DIR}/..\\\\distribution\\\\yadoms.ico")
      SET(CPACK_NSIS_MUI_UNIICON "${CMAKE_CURRENT_SOURCE_DIR}/..\\\\distribution\\\\yadoms.ico")
      SET(CPACK_NSIS_DISPLAY_NAME "Yadoms")
      SET(CPACK_NSIS_HELP_LINK "http:\\\\\\\\www.yadoms.com")
      SET(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\www.yadoms.com")
      SET(CPACK_NSIS_CONTACT "contact@yadoms.com")
      SET(CPACK_NSIS_MODIFY_PATH ON)

      set(CPACK_NSIS_CREATE_ICONS_STARTMENU
         "SetOutPath $INSTDIR\\\\bin
         SetShellVarContext all
         CreateShortCut '\$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\Yadoms-StartServer.lnk' '\$INSTDIR\\\\bin\\\\yadoms.exe' '' '\$INSTDIR\\\\bin\\\\yadoms.exe' 0
         CreateShortCut '\$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\Yadoms.lnk' 'http://localhost:8080' '' '\$INSTDIR\\\\bin\\\\yadoms.exe' 0"
      )

      set(CPACK_NSIS_CREATE_ICONS_DESKTOP
         "SetOutPath $INSTDIR\\\\bin
         SetShellVarContext all
         CreateShortCut '\$DESKTOP\\\\Yadoms.lnk' 'http://localhost:8080' '' '\$INSTDIR\\\\bin\\\\yadoms.exe' 0"
      )
     
      set(CPACK_NSIS_CREATE_ICONS_DESKTOP_NOSERVICE
         "SetOutPath $INSTDIR\\\\bin
         SetShellVarContext all
         CreateShortCut '\$DESKTOP\\\\Yadoms-StartServer.lnk' '\$INSTDIR\\\\bin\\\\yadoms.exe' '' '\$INSTDIR\\\\bin\\\\yadoms.exe' 0
         CreateShortCut '\$DESKTOP\\\\Yadoms.lnk' 'http://localhost:8080' '' '\$INSTDIR\\\\bin\\\\yadoms.exe' 0"
      )
     

      set(CPACK_NSIS_INSTALL_SERVICE
         "ExecWait '\$INSTDIR\\\\bin\\\\yadoms.exe --registerService --displayName:Yadoms --startup:automatic' $0
         !insertmacro SERVICE 'start' 'Yadoms' ''"
      )   

      set(CPACK_NSIS_UNINSTALL_SERVICE
         "!ifdef UN
         !undef UN
         !endif
         !define UN 'un.'         
         !insertmacro SERVICE 'stop' 'Yadoms' ''
         ExecWait '\$INSTDIR\\\\bin\\\\yadoms.exe --unregisterService' $0"
      )
      
      # Grant full authorisations for install directory (all users : identified by (S-1-1-0) )
      set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS
         "AccessControl::GrantOnFile  \\\"$INSTDIR\\\" \\\"(S-1-1)\\\" \\\"FullAccess\\\"
         AccessControl::GrantOnFile  \\\"$INSTDIR\\\" \\\"(S-1-1-0)\\\" \\\"FullAccess\\\""
      )
   else(USE_NSIS)
      set(CPACK_GENERATOR "ZIP")
   endif(USE_NSIS)

ELSE(WIN32 AND NOT UNIX)

  IF(APPLE)
     SET(CPACK_GENERATOR "TGZ")
  ELSE(APPLE)

    SET(CPACK_STRIP_FILES "Yadoms")
    SET(CPACK_SOURCE_STRIP_FILES "")
    SET(CPACK_GENERATOR "TGZ")

  ENDIF()

ENDIF(WIN32 AND NOT UNIX)
SET(CPACK_PACKAGE_EXECUTABLES "Yadoms" "Yadoms")

# When calling "make package" it do not run "make all" 
# So package can be realized with make all_unity call before make package
set(CMAKE_SKIP_INSTALL_ALL_DEPENDENCY TRUE)
set(CMAKE_SKIP_PACKAGE_ALL_DEPENDENCY ON)

INCLUDE(CPack)


##################################################################################################
## SONARQUBE ANALYSIS
##################################################################################################
include(cmake/analysis.cmake)
analyse(
      yadoms
      ${YADOMS_VERSION}
      "${YADOMS_SRC}"
      Yadoms.SonarQube.Analysis.txt
      Yadoms.SonarQube.Analysis.xml
   )
#TODO : add analysis for all libs (shared, xxx_IPC, plugins...)
