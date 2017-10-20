

########################
## Sources
########################
set(SHARED_LIB_SRC
   stdafx.cpp
   stdafx.h

   shared/Log.h
   shared/Log.cpp
   
   shared/ILocation.h
	
   shared/logInternal/ILogStream.h
   shared/logInternal/LogStreamEnhanced.h
   shared/logInternal/LogStreamEnhanced.cpp	

   shared/ThreadBase.cpp
   shared/ThreadBase.h

   shared/StringExtension.h
	
   shared/Field.hpp

   shared/IDataContainable.h
   shared/DataContainer.h
   shared/DataContainer.cpp
   
   shared/ServiceLocator.h
   shared/ServiceLocator.cpp
   
   shared/communication/AsyncPortConnectionNotification.h
   shared/communication/AsyncPortConnectionNotification.cpp
   shared/communication/AsyncSerialPort.h
   shared/communication/AsyncSerialPort.cpp
   shared/communication/AsyncTcpClient.h
   shared/communication/AsyncTcpClient.cpp
   shared/communication/Buffer.hpp
   shared/communication/AsciiBufferLogger.h
   shared/communication/AsciiBufferLogger.cpp
   shared/communication/BufferLogger.h
   shared/communication/BufferLogger.cpp
   shared/communication/BufferLoggerHelper.h
   shared/communication/BufferLoggerHelper.cpp
   shared/communication/EOFReceiveBufferHandler.h
   shared/communication/EOFReceiveBufferHandler.cpp
   shared/communication/FixedSizeReceiveBufferHandler.h
   shared/communication/FixedSizeReceiveBufferHandler.cpp
   shared/communication/IAsyncPort.h
   shared/communication/IBufferLogger.h
   shared/communication/IMessageAssembler.h
   shared/communication/IMessageCutter.h
   shared/communication/IMessageAssemblerPart.h
   shared/communication/IMessageCutterPart.h
   shared/communication/IReceiveBufferHandler.h
   shared/communication/MessageQueueRemover.hpp
   shared/communication/NoBufferLogger.h
   shared/communication/NoBufferLogger.cpp
   shared/communication/StringBuffer.h
   shared/communication/StringBuffer.cpp
   shared/communication/PortException.hpp
   shared/communication/SmallHeader.h
   shared/communication/SmallHeaderMessageAssembler.h
   shared/communication/SmallHeaderMessageAssembler.cpp
   shared/communication/SmallHeaderMessageAssemblerPart.h
   shared/communication/SmallHeaderMessageAssemblerPart.cpp
   shared/communication/SmallHeaderMessageCutter.h
   shared/communication/SmallHeaderMessageCutter.cpp
   shared/communication/SmallHeaderMessageCutterPart.h
   shared/communication/SmallHeaderMessageCutterPart.cpp
	
   shared/compression/Extract.h
   shared/compression/Extract.cpp

   shared/currentTime/ICurrentTime.h
   shared/currentTime/Local.h
   shared/currentTime/Local.cpp
   shared/currentTime/Provider.h
   shared/currentTime/Provider.cpp

   shared/dateTime/DateTimeContainer.h
   shared/dateTime/DateTimeContainer.cpp

   shared/encryption/Xor.h
   shared/encryption/Xor.cpp
   shared/encryption/Base64.h
   shared/encryption/Base64.cpp
   shared/encryption/Md5.h
   shared/encryption/Md5.cpp
	
   shared/enumeration/IExtendedEnum.h
   shared/enumeration/ExtendedEnumHelpers.cpp
   shared/enumeration/EnumHelpers.hpp
	
   shared/event/Event.hpp
   shared/event/EventBase.hpp
   shared/event/EventHandler.hpp
   shared/event/EventTimer.cpp
   shared/event/EventTimer.h
   shared/event/EventTimePoint.cpp
   shared/event/EventTimePoint.h
   shared/event/ITimeEvent.h

   shared/exception/BadConversion.hpp
   shared/exception/EmptyResult.hpp
   shared/exception/Exception.hpp
   shared/exception/Extract.hpp
   shared/exception/InvalidParameter.hpp
   shared/exception/NotImplemented.hpp
   shared/exception/NotSupported.hpp
   shared/exception/NullReference.hpp
   shared/exception/OutOfRange.hpp
   shared/exception/Parse.hpp
   shared/exception/JSONParse.hpp
   
   shared/http/IHttpSession.h
   shared/http/HttpMethods.h
   shared/http/HttpMethods.cpp
   shared/http/StandardSession.h
   shared/http/StandardSession.cpp

   shared/plugin/information/IInformation.h
   shared/plugin/information/IYadomsInformation.h

   shared/plugin/yPluginApi/commonIncludes.h
   shared/plugin/yPluginApi/IBindingQueryData.h
   shared/plugin/yPluginApi/IBindingQueryRequest.h
   shared/plugin/yPluginApi/IDeviceCommand.h
   shared/plugin/yPluginApi/IDeviceConfigurationSchemaRequest.h
   shared/plugin/yPluginApi/IDeviceRemoved.h
   shared/plugin/yPluginApi/IExtraQuery.h
   shared/plugin/yPluginApi/IExtraQueryData.h
   shared/plugin/yPluginApi/IManuallyDeviceCreationData.h
   shared/plugin/yPluginApi/IManuallyDeviceCreationRequest.h
   shared/plugin/yPluginApi/ISetDeviceConfiguration.h
   shared/plugin/yPluginApi/IYPluginApi.h
   shared/plugin/yPluginApi/KeywordAccessMode.cpp
   shared/plugin/yPluginApi/KeywordAccessMode.h
   shared/plugin/yPluginApi/KeywordDataType.cpp
   shared/plugin/yPluginApi/KeywordDataType.h
   shared/plugin/yPluginApi/StandardCapacities.cpp
   shared/plugin/yPluginApi/StandardCapacities.h
   shared/plugin/yPluginApi/StandardCapacity.cpp
   shared/plugin/yPluginApi/StandardCapacity.h
   shared/plugin/yPluginApi/StandardUnits.cpp
   shared/plugin/yPluginApi/StandardUnits.h
   shared/plugin/yPluginApi/StandardValues.cpp
   shared/plugin/yPluginApi/StandardValues.h
   shared/plugin/yPluginApi/YPluginConfiguration.hpp

   shared/plugin/yPluginApi/historization/IHistorizable.h
   shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp
   shared/plugin/yPluginApi/historization/BatteryLevel.cpp
   shared/plugin/yPluginApi/historization/BatteryLevel.h
   shared/plugin/yPluginApi/historization/Alarm.cpp
   shared/plugin/yPluginApi/historization/Alarm.h
   shared/plugin/yPluginApi/historization/ApparentPower.cpp
   shared/plugin/yPluginApi/historization/ApparentPower.h
   shared/plugin/yPluginApi/historization/ArmingAlarm.cpp
   shared/plugin/yPluginApi/historization/ArmingAlarm.h
   shared/plugin/yPluginApi/historization/Historizers.h
   shared/plugin/yPluginApi/historization/CameraMove.cpp
   shared/plugin/yPluginApi/historization/CameraMove.h
   shared/plugin/yPluginApi/historization/ColorRGB.cpp
   shared/plugin/yPluginApi/historization/ColorRGB.h
   shared/plugin/yPluginApi/historization/ColorRGBW.cpp
   shared/plugin/yPluginApi/historization/ColorRGBW.h
   shared/plugin/yPluginApi/historization/Counter.cpp
   shared/plugin/yPluginApi/historization/Counter.h
   shared/plugin/yPluginApi/historization/Current.cpp
   shared/plugin/yPluginApi/historization/Current.h
   shared/plugin/yPluginApi/historization/Curtain.cpp
   shared/plugin/yPluginApi/historization/Curtain.h
   shared/plugin/yPluginApi/historization/DateTime.cpp
   shared/plugin/yPluginApi/historization/DateTime.h
   shared/plugin/yPluginApi/historization/DeviceState.cpp
   shared/plugin/yPluginApi/historization/DeviceState.h
   shared/plugin/yPluginApi/historization/DeviceStateMessage.cpp
   shared/plugin/yPluginApi/historization/DeviceStateMessage.h
   shared/plugin/yPluginApi/historization/Dimmable.cpp
   shared/plugin/yPluginApi/historization/Dimmable.h
   shared/plugin/yPluginApi/historization/Direction.cpp
   shared/plugin/yPluginApi/historization/Direction.h
   shared/plugin/yPluginApi/historization/Distance.cpp
   shared/plugin/yPluginApi/historization/Distance.h
   shared/plugin/yPluginApi/historization/Duration.cpp
   shared/plugin/yPluginApi/historization/Duration.h
   shared/plugin/yPluginApi/historization/Energy.cpp
   shared/plugin/yPluginApi/historization/Energy.h
   shared/plugin/yPluginApi/historization/Event.cpp
   shared/plugin/yPluginApi/historization/Event.h
   shared/plugin/yPluginApi/historization/Frequency.cpp
   shared/plugin/yPluginApi/historization/Frequency.h
   shared/plugin/yPluginApi/historization/Humidity.cpp
   shared/plugin/yPluginApi/historization/Humidity.h
   shared/plugin/yPluginApi/historization/Illumination.cpp
   shared/plugin/yPluginApi/historization/Illumination.h
   shared/plugin/yPluginApi/historization/Load.cpp
   shared/plugin/yPluginApi/historization/Load.h
   shared/plugin/yPluginApi/historization/Message.cpp
   shared/plugin/yPluginApi/historization/Message.h
   shared/plugin/yPluginApi/historization/MessageFormatter.cpp
   shared/plugin/yPluginApi/historization/MessageFormatter.h
   shared/plugin/yPluginApi/historization/MeasureType.cpp
   shared/plugin/yPluginApi/historization/MeasureType.h
   shared/plugin/yPluginApi/historization/PluginState.cpp
   shared/plugin/yPluginApi/historization/PluginState.h
   shared/plugin/yPluginApi/historization/PluginStateMessage.cpp
   shared/plugin/yPluginApi/historization/PluginStateMessage.h
   shared/plugin/yPluginApi/historization/Power.cpp
   shared/plugin/yPluginApi/historization/Power.h
   shared/plugin/yPluginApi/historization/PowerFactor.cpp
   shared/plugin/yPluginApi/historization/PowerFactor.h
   shared/plugin/yPluginApi/historization/Pressure.cpp
   shared/plugin/yPluginApi/historization/Pressure.h
   shared/plugin/yPluginApi/historization/Rain.cpp
   shared/plugin/yPluginApi/historization/Rain.h
   shared/plugin/yPluginApi/historization/RainRate.cpp
   shared/plugin/yPluginApi/historization/RainRate.h
   shared/plugin/yPluginApi/historization/Rssi.cpp
   shared/plugin/yPluginApi/historization/Rssi.h
   shared/plugin/yPluginApi/historization/Speed.cpp
   shared/plugin/yPluginApi/historization/Speed.h
   shared/plugin/yPluginApi/historization/SignalPower.cpp
   shared/plugin/yPluginApi/historization/SignalPower.h
   shared/plugin/yPluginApi/historization/SignalLevel.cpp
   shared/plugin/yPluginApi/historization/SignalLevel.h
   shared/plugin/yPluginApi/historization/Switch.cpp
   shared/plugin/yPluginApi/historization/Switch.h
   shared/plugin/yPluginApi/historization/Tamper.cpp
   shared/plugin/yPluginApi/historization/Tamper.h
   shared/plugin/yPluginApi/historization/Temperature.cpp
   shared/plugin/yPluginApi/historization/Temperature.h
   shared/plugin/yPluginApi/historization/Text.cpp
   shared/plugin/yPluginApi/historization/Text.h
   shared/plugin/yPluginApi/historization/UpDownStop.cpp
   shared/plugin/yPluginApi/historization/UpDownStop.h
   shared/plugin/yPluginApi/historization/Uv.cpp
   shared/plugin/yPluginApi/historization/Uv.h
   shared/plugin/yPluginApi/historization/Voltage.cpp
   shared/plugin/yPluginApi/historization/Voltage.h
   shared/plugin/yPluginApi/historization/Volume.cpp
   shared/plugin/yPluginApi/historization/Volume.h
   shared/plugin/yPluginApi/historization/Weight.cpp
   shared/plugin/yPluginApi/historization/WeatherCondition.h
   shared/plugin/yPluginApi/historization/WeatherCondition.cpp
   shared/plugin/yPluginApi/historization/Weight.h
   
   shared/plugin/yPluginApi/historization/typeInfo/ITypeInfo.h
   shared/plugin/yPluginApi/historization/typeInfo/EmptyTypeInfo.h
   shared/plugin/yPluginApi/historization/typeInfo/EmptyTypeInfo.cpp
   shared/plugin/yPluginApi/historization/typeInfo/BoolTypeInfo.h
   shared/plugin/yPluginApi/historization/typeInfo/BoolTypeInfo.cpp
   shared/plugin/yPluginApi/historization/typeInfo/IntTypeInfo.h
   shared/plugin/yPluginApi/historization/typeInfo/IntTypeInfo.cpp
   shared/plugin/yPluginApi/historization/typeInfo/DoubleTypeInfo.h
   shared/plugin/yPluginApi/historization/typeInfo/DoubleTypeInfo.cpp
   shared/plugin/yPluginApi/historization/typeInfo/StringTypeInfo.h
   shared/plugin/yPluginApi/historization/typeInfo/StringTypeInfo.cpp
   shared/plugin/yPluginApi/historization/typeInfo/EnumTypeInfo.hpp

   shared/process/CoutCerrConsoleLogChannel.h
   shared/process/CoutCerrConsoleLogChannel.cpp
   shared/process/IApplication.h
   shared/process/IApplicationStopHandler.h
   shared/process/IProcessObserver.h
   shared/process/ICommandLine.h
   shared/process/IProcess.h
   shared/process/IExternalProcessLogger.h
   shared/process/NativeExecutableCommandLine.h
   shared/process/NativeExecutableCommandLine.cpp
   shared/process/Process.h
   shared/process/Process.cpp
   shared/process/SoftwareStop.h
   shared/process/SoftwareStop.cpp
   shared/process/YadomsSubModuleLogConfiguration.h
   shared/process/YadomsSubModuleLogConfiguration.cpp

   shared/serialization/IDataSerializable.h
   shared/serialization/IDataFileSerializable.h

   shared/script/yInterpreterApi/IAvalaibleRequest.h
   shared/script/yInterpreterApi/IInformation.h
   shared/script/yInterpreterApi/ILoadScriptContentRequest.h
   shared/script/yInterpreterApi/ISaveScriptContentRequest.h
   shared/script/yInterpreterApi/IStartScript.h
   shared/script/yInterpreterApi/IStopScript.h
   shared/script/yInterpreterApi/IYInterpreterApi.h

   shared/script/yScriptApi/IYScriptApi.h
   shared/script/yScriptApi/WaitForEventResult.h

   shared/tools/Random.h
   shared/tools/Random.cpp

   shared/versioning/Version.h
   shared/versioning/Version.cpp
   shared/versioning/VersionInformation.h
   shared/versioning/VersionInformation.cpp
   
   shared/web/FileDownloader.h
   shared/web/FileDownloader.cpp
   shared/web/UriHelpers.h
   shared/web/UriHelpers.cpp
   shared/web/exception/InvalidHash.hpp
   shared/web/exception/DownloadFailed.hpp
)

#Append platform specific files
IF(WIN32)
set(SHARED_LIB_SRC	
	${SHARED_LIB_SRC}
   windows/shared/DynamicLibrary.h
   windows/shared/DynamicLibrary.cpp
   windows/shared/DynamicLibraryExport.h
   windows/shared/Executable.h
   windows/shared/Executable.cpp
   windows/shared/Export.h
   windows/shared/FileSystemExtension.h
   windows/shared/FileSystemExtension.cpp
   windows/shared/iobFuncLegacyAdapter.cpp
   windows/shared/NetworkHelper.h
   windows/shared/NetworkHelper.cpp
   windows/shared/Peripherals.h
   windows/shared/Peripherals.cpp
   windows/shared/Thread.h
   windows/shared/Thread.cpp
   
   windows/shared/process/ConsoleControlHandler.h
   windows/shared/process/ConsoleControlHandler.cpp
   windows/shared/process/ServiceControlHandler.h
   windows/shared/process/ServiceControlHandler.cpp
   windows/shared/process/ApplicationStopHandler.h
   windows/shared/process/ApplicationStopHandler.cpp
)	
set(PLATFORM_SPECIFIC_INCL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/windows)

ELSEIF(APPLE)
set(SHARED_LIB_SRC	
   ${SHARED_LIB_SRC}
   mac/shared/DynamicLibrary.h
   mac/shared/DynamicLibrary.cpp
   mac/shared/DynamicLibraryExport.h
   mac/shared/Executable.h
   mac/shared/Executable.cpp
   mac/shared/Export.h
   mac/shared/FileSystemExtension.h
   mac/shared/FileSystemExtension.cpp
   mac/shared/NetworkHelper.h
   mac/shared/NetworkHelper.cpp
   mac/shared/Peripherals.h
   mac/shared/Peripherals.cpp
   mac/shared/Thread.h
   mac/shared/Thread.cpp
   
   mac/shared/process/ApplicationStopHandler.h
   mac/shared/process/ApplicationStopHandler.cpp
)	
set(PLATFORM_SPECIFIC_INCL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/mac)

ELSE()
set(SHARED_LIB_SRC	
   ${SHARED_LIB_SRC}
   linux/shared/DynamicLibrary.h
   linux/shared/DynamicLibrary.cpp
   linux/shared/DynamicLibraryExport.h
   linux/shared/Executable.h
   linux/shared/Executable.cpp
   linux/shared/Export.h
   linux/shared/FileSystemExtension.h
   linux/shared/FileSystemExtension.cpp
   linux/shared/NetworkHelper.h
   linux/shared/NetworkHelper.cpp
   linux/shared/Peripherals.h
   linux/shared/Peripherals.cpp
   linux/shared/Thread.h
   linux/shared/Thread.cpp
   
   linux/shared/process/ApplicationStopHandler.h
   linux/shared/process/ApplicationStopHandler.cpp
)	
set(PLATFORM_SPECIFIC_INCL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/linux)

ENDIF()

# Define project folders
source_group(communication shared/communication/*.*)
source_group(compression shared/shared/compression/*.* )	
source_group(currentTime shared/shared/currentTime/*.* )	
source_group(dateTime shared/shared/dateTime/*.* )	
source_group(encryption shared/shared/encryption/*.* )	
source_group(enumeration shared/shared/enumeration/*.* )	
source_group(event shared/event/*.*)
source_group(exception shared/exception/*.*)
source_group(http shared/http/*.*)
source_group(logInternal shared/logInternal/*.*)
source_group(plugin shared/shared/plugin/*.* )	
source_group(plugin\\information shared/shared/plugin/information/*.* )	
source_group(plugin\\yPluginApi shared/shared/plugin/yPluginApi/*.* )	
source_group(plugin\\yPluginApi\\historization shared/shared/plugin/yPluginApi/historization/*.* )	
source_group(plugin\\yPluginApi\\historization\\typeInfo shared/shared/plugin/yPluginApi/historization/typeInfo/*.* )	
source_group(process process/*.* )	
source_group(notification shared/shared/notification/*.* )	
source_group(serialization shared/shared/serialization/*.* )	
source_group(script shared/shared/script/*.* )	
source_group(script\\yScriptApi shared/shared/script/yScriptApi/*.* )	
source_group(tools shared/shared/tools/*.* )	
source_group(versioning shared/shared/versioning/*.* )	
source_group(web shared/shared/web/*.* )	
source_group(web\\exception shared/shared/web/exception/*.* )	

source_group(platformSpecific\\windows shared/windows/shared/*.*)
source_group(platformSpecific\\windows\\process shared/windows/shared/process/*.*)
source_group(platformSpecific\\linux shared/linux/shared/*.*)
source_group(platformSpecific\\linux\\process shared/linux/shared/process/*.*)
source_group(platformSpecific\\mac shared/mac/shared/*.*)
source_group(platformSpecific\\mac\\process shared/mac/shared/process/*.*)





########################
## Output
########################
add_library(yadoms-shared STATIC ${SHARED_LIB_SRC})

target_link_libraries(yadoms-shared ${LIBS} ${CMAKE_DL_LIBS})
set_property( TARGET yadoms-shared PROPERTY INCLUDE_DIRECTORIES ${BOOST_INCL_DIR} ${CMAKE_CURRENT_SOURCE_DIR}  ${PLATFORM_SPECIFIC_INCL_DIR}  ${Poco_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR})

if(COTIRE_USE)
   set_target_properties(yadoms-shared PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "stdafx.h")
	
   if(COTIRE_USE_UNITY)
      set_target_properties(yadoms-shared PROPERTIES COTIRE_ADD_UNITY_BUILD TRUE)
   else()
      set_target_properties(yadoms-shared PROPERTIES COTIRE_ADD_UNITY_BUILD FALSE)
   endif()
	
   cotire(yadoms-shared)
	
   if(COTIRE_USE_UNITY)
      target_link_libraries(yadoms-shared_unity ${LIBS} ${CMAKE_DL_LIBS})
set_property( TARGET yadoms-shared_unity PROPERTY INCLUDE_DIRECTORIES ${BOOST_INCL_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${PLATFORM_SPECIFIC_INCL_DIR}  ${Poco_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR})
   endif()	
else()
   add_precompiled_header(yadoms-shared stdafx.h)
endif()






