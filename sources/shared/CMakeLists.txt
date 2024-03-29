

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
   shared/dateTime/Format.h
   shared/dateTime/Format.cpp

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

   shared/exception/DownloadFailed.hpp
   shared/exception/EmptyResult.hpp
   shared/exception/Exception.hpp
   shared/exception/Extract.hpp
   shared/exception/HttpException.hpp
   shared/exception/InvalidHash.hpp
   shared/exception/InvalidParameter.hpp
   shared/exception/JSONParse.hpp
   shared/exception/NotImplemented.hpp
   shared/exception/NotSupported.hpp
   shared/exception/NullReference.hpp
   shared/exception/OutOfRange.hpp
   shared/exception/Parse.hpp
   
   shared/http/Codes.h
   shared/http/Codes.cpp
   shared/http/curlppHelpers.h
   shared/http/curlppHelpers.cpp
   shared/http/curlppHttpRestRequest.h
   shared/http/curlppHttpRestRequest.cpp
   shared/http/FileDownloader.h
   shared/http/FileDownloader.cpp
   shared/http/HttpHelpers.h
   shared/http/HttpHelpers.cpp
   shared/http/HttpRestHelpers.h
   shared/http/HttpRestHelpers.cpp
   shared/http/IHttpRestRequest.h
   shared/http/Proxy.h
   shared/http/Proxy.cpp
   shared/http/UrlHelpers.h
   shared/http/UrlHelpers.cpp   
   
	shared/http/ssdp/Client.cpp
	shared/http/ssdp/Client.h
	shared/http/ssdp/DiscoveredDevice.cpp
	shared/http/ssdp/DiscoveredDevice.h
	shared/http/ssdp/DiscoverService.cpp
	shared/http/ssdp/DiscoverService.h
	shared/http/ssdp/IDiscoveredDevice.h

   shared/plugin/information/IInformation.h
   shared/plugin/information/IYadomsInformation.h

   shared/plugin/yPluginApi/commonIncludes.h
   shared/plugin/yPluginApi/HistoryDepth.cpp
   shared/plugin/yPluginApi/HistoryDepth.h
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
   shared/plugin/yPluginApi/MeasureType.cpp
   shared/plugin/yPluginApi/MeasureType.h
   shared/plugin/yPluginApi/StandardCapacities.cpp
   shared/plugin/yPluginApi/StandardCapacities.h
   shared/plugin/yPluginApi/StandardCapacity.cpp
   shared/plugin/yPluginApi/StandardCapacity.h
   shared/plugin/yPluginApi/StandardUnits.cpp
   shared/plugin/yPluginApi/StandardUnits.h
   shared/plugin/yPluginApi/StandardValues.cpp
   shared/plugin/yPluginApi/StandardValues.h
   shared/plugin/yPluginApi/YPluginConfiguration.hpp

   shared/plugin/yPluginApi/configuration/File.cpp
   shared/plugin/yPluginApi/configuration/File.h

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
   shared/plugin/yPluginApi/historization/Debit.cpp
   shared/plugin/yPluginApi/historization/Debit.h
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
   shared/plugin/yPluginApi/historization/ElectricLoad.cpp
   shared/plugin/yPluginApi/historization/ElectricLoad.h
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
   shared/plugin/yPluginApi/historization/IlluminationWm2.cpp
   shared/plugin/yPluginApi/historization/IlluminationWm2.h
   shared/plugin/yPluginApi/historization/IMessageFormatter.h
   shared/plugin/yPluginApi/historization/Load.cpp
   shared/plugin/yPluginApi/historization/Load.h
   shared/plugin/yPluginApi/historization/Message.cpp
   shared/plugin/yPluginApi/historization/Message.h
   shared/plugin/yPluginApi/historization/MessageFormatter.cpp
   shared/plugin/yPluginApi/historization/MessageFormatter.h
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
   shared/plugin/yPluginApi/historization/UserCode.cpp
   shared/plugin/yPluginApi/historization/UserCode.h
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
   
   shared/plugin/yPluginApi/typeInfo/ITypeInfo.h
   shared/plugin/yPluginApi/typeInfo/EmptyTypeInfo.h
   shared/plugin/yPluginApi/typeInfo/EmptyTypeInfo.cpp
   shared/plugin/yPluginApi/typeInfo/ByteArrayTypeInfo.h
   shared/plugin/yPluginApi/typeInfo/ByteArrayTypeInfo.cpp
   shared/plugin/yPluginApi/typeInfo/BoolTypeInfo.h
   shared/plugin/yPluginApi/typeInfo/BoolTypeInfo.cpp
   shared/plugin/yPluginApi/typeInfo/IntTypeInfo.h
   shared/plugin/yPluginApi/typeInfo/IntTypeInfo.cpp
   shared/plugin/yPluginApi/typeInfo/DoubleTypeInfo.h
   shared/plugin/yPluginApi/typeInfo/DoubleTypeInfo.cpp
   shared/plugin/yPluginApi/typeInfo/PercentageTypeInfo.h
   shared/plugin/yPluginApi/typeInfo/PercentageTypeInfo.cpp
   shared/plugin/yPluginApi/typeInfo/StringTypeInfo.h
   shared/plugin/yPluginApi/typeInfo/StringTypeInfo.cpp
   shared/plugin/yPluginApi/typeInfo/EnumTypeInfo.hpp

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

   shared/rapidjson/allocators.h
   shared/rapidjson/cursorstreamwrapper.h
   shared/rapidjson/document.h
   shared/rapidjson/encodedstream.h
   shared/rapidjson/encodings.h
   shared/rapidjson/filereadstream.h
   shared/rapidjson/filewritestream.h
   shared/rapidjson/fwd.h
   shared/rapidjson/istreamwrapper.h
   shared/rapidjson/memorybuffer.h
   shared/rapidjson/memorystream.h
   shared/rapidjson/ostreamwrapper.h
   shared/rapidjson/pointer.h
   shared/rapidjson/prettywriter.h
   shared/rapidjson/rapidjson.h
   shared/rapidjson/reader.h
   shared/rapidjson/schema.h
   shared/rapidjson/stream.h
   shared/rapidjson/stringbuffer.h
   shared/rapidjson/writer.h
   shared/rapidjson/error/en.h
   shared/rapidjson/error/error.h
   shared/rapidjson/internal/biginteger.h
   shared/rapidjson/internal/clzll.h
   shared/rapidjson/internal/diyfp.h
   shared/rapidjson/internal/dtoa.h
   shared/rapidjson/internal/ieee754.h
   shared/rapidjson/internal/itoa.h
   shared/rapidjson/internal/meta.h
   shared/rapidjson/internal/pow10.h
   shared/rapidjson/internal/regex.h
   shared/rapidjson/internal/stack.h
   shared/rapidjson/internal/strfunc.h
   shared/rapidjson/internal/strtod.h
   shared/rapidjson/internal/swap.h
   shared/rapidjson/msinttypes/inttypes.h
   shared/rapidjson/msinttypes/stdint.h
   
   shared/serialization/IDataSerializable.h
   shared/serialization/IDataFileSerializable.h

   shared/script/yInterpreterApi/IAvalaibleRequest.h
   shared/script/yInterpreterApi/IInformation.h
   shared/script/yInterpreterApi/ILoadScriptContentRequest.h
   shared/script/yInterpreterApi/IPurgeScriptLog.h
   shared/script/yInterpreterApi/ISaveScriptContentRequest.h
   shared/script/yInterpreterApi/IStartScript.h
   shared/script/yInterpreterApi/IStopScript.h
   shared/script/yInterpreterApi/IYInterpreterApi.h

   shared/script/yScriptApi/IYScriptApi.h
   shared/script/yScriptApi/WaitForEventResult.h

   shared/tools/Filesystem.h
   shared/tools/Filesystem.cpp
   shared/tools/Random.h
   shared/tools/Random.cpp

   shared/versioning/SemVer.h
   shared/versioning/SemVer.cpp
   shared/versioning/VersionInformation.h
   shared/versioning/VersionInformation.cpp
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
   windows/shared/FileSystemExtension.h
   windows/shared/FileSystemExtension.cpp
   windows/shared/iobFuncLegacyAdapter.cpp
   windows/shared/NetworkHelper.h
   windows/shared/NetworkHelper.cpp 
   windows/shared/SerialPortHelper.h
   windows/shared/SerialPortHelper.cpp
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
   mac/shared/FileSystemExtension.h
   mac/shared/FileSystemExtension.cpp
   mac/shared/NetworkHelper.h
   mac/shared/NetworkHelper.cpp 
   mac/shared/SerialPortHelper.h
   mac/shared/SerialPortHelper.cpp
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
   linux/shared/FileSystemExtension.h
   linux/shared/FileSystemExtension.cpp
   linux/shared/NetworkHelper.h
   linux/shared/NetworkHelper.cpp 
   linux/shared/SerialPortHelper.h
   linux/shared/SerialPortHelper.cpp
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
source_group(http\\ssdp shared/http/ssdp/*.*)
source_group(logInternal shared/logInternal/*.*)
source_group(notification shared/shared/notification/*.* )	
source_group(plugin shared/shared/plugin/*.* )	
source_group(plugin\\information shared/shared/plugin/information/*.* )	
source_group(plugin\\yPluginApi shared/shared/plugin/yPluginApi/*.* )	
source_group(plugin\\yPluginApi\\configuration shared/shared/plugin/yPluginApi/configuration/*.* )	
source_group(plugin\\yPluginApi\\historization shared/shared/plugin/yPluginApi/historization/*.* )	
source_group(plugin\\yPluginApi\\typeInfo shared/shared/plugin/yPluginApi/typeInfo/*.* )	
source_group(process process/*.* )	
source_group(rapidjson shared/shared/rapidjson/*.* )	
source_group(rapidjson\\msinttypes shared/shared/rapidjson/msinttypes/*.* )	
source_group(rapidjson\\internal shared/shared/rapidjson/internal/*.* )	
source_group(rapidjson\\error shared/shared/rapidjson/error/*.* )
source_group(serialization shared/shared/serialization/*.* )	
source_group(script shared/shared/script/*.* )	
source_group(script\\yScriptApi shared/shared/script/yScriptApi/*.* )	
source_group(tools shared/shared/tools/*.* )	
source_group(versioning shared/shared/versioning/*.* )

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

add_dependencies(yadoms-shared curl_build curlpp_build)
target_link_libraries(yadoms-shared
   ${LIBS}
   ${CMAKE_DL_LIBS}
   ${CURL_LIBRARY}
   ${CURLPP_LIBRARY}
)

set_property(TARGET yadoms-shared PROPERTY INCLUDE_DIRECTORIES
   ${BOOST_INCL_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${PLATFORM_SPECIFIC_INCL_DIR}
   ${Poco_INCLUDE_DIRS}
   ${OPENSSL_INCLUDE_DIR}
   ${CURL_INCLUDE_DIR}
   ${CURLPP_INCLUDE_DIR}
)

if(COTIRE_USE)
   set_target_properties(yadoms-shared PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "stdafx.h")
	
   if(COTIRE_USE_UNITY)
      set_target_properties(yadoms-shared PROPERTIES COTIRE_ADD_UNITY_BUILD TRUE)
   else()
      set_target_properties(yadoms-shared PROPERTIES COTIRE_ADD_UNITY_BUILD FALSE)
   endif()
	
   cotire(yadoms-shared)
	
   if(COTIRE_USE_UNITY)
      add_dependencies(yadoms-shared_unity curl_build curlpp_build)
      target_link_libraries(yadoms-shared_unity
         ${LIBS}
         ${CMAKE_DL_LIBS}
         ${CURL_LIBRARY}
         ${CURLPP_LIBRARY}
      )
      set_property(TARGET yadoms-shared PROPERTY INCLUDE_DIRECTORIES
         ${BOOST_INCL_DIR}
         ${CMAKE_CURRENT_SOURCE_DIR}
         ${PLATFORM_SPECIFIC_INCL_DIR}
         ${Poco_INCLUDE_DIRS}
         ${OPENSSL_INCLUDE_DIR}
         ${CURL_INCLUDE_DIR}
         ${CURLPP_INCLUDE_DIR}
      )
   endif()	
else()
   add_precompiled_header(yadoms-shared stdafx.h)
endif()






