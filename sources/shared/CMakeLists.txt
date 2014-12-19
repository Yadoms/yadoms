

########################
## Sources
########################
set(SHARED_LIB_SRC
	stdafx.cpp
	stdafx.h

 	shared/Log.h
	shared/Log.cpp
	
   shared/logInternal/ILogStream.h
   shared/logInternal/LogStreamEnhanced.h
	shared/logInternal/LogStreamEnhanced.cpp	

	shared/ThreadBase.cpp
	shared/ThreadBase.h

	shared/StringExtension.h
	shared/StringExtension.cpp	
	
	shared/Field.hpp

	shared/IDataContainable.h
	shared/DataContainer.h
	shared/DataContainer.cpp
   
   shared/communication/AsyncSerialPort.h
   shared/communication/AsyncSerialPort.cpp
   shared/communication/AsyncTcpClient.h
   shared/communication/AsyncTcpClient.cpp
   shared/communication/Buffer.hpp
   shared/communication/AsciiBufferLogger.h
   shared/communication/AsciiBufferLogger.cpp
   shared/communication/BufferLogger.h
   shared/communication/BufferLogger.cpp
   shared/communication/EOFReceiveBufferHandler.h
   shared/communication/EOFReceiveBufferHandler.cpp
   shared/communication/FixedSizeReceiveBufferHandler.h
   shared/communication/FixedSizeReceiveBufferHandler.cpp
   shared/communication/IAsyncPort.h
   shared/communication/IBufferLogger.h
   shared/communication/IReceiveBufferHandler.h
   shared/communication/PortException.hpp
	
	shared/encryption/Xor.h
	shared/encryption/Xor.cpp
	shared/encryption/Base64.h
	shared/encryption/Base64.cpp
	shared/encryption/md5.h
	
	shared/enumeration/IExtendedEnum.h
	shared/enumeration/EnumHelpers.hpp
	
	shared/event/Event.hpp
	shared/event/EventBase.hpp
	shared/event/EventHandler.hpp
	shared/event/EventTimer.cpp
	shared/event/EventTimer.h
	shared/event/EventTimePoint.cpp
	shared/event/EventTimePoint.h
	shared/event/ITimeEvent.h
	shared/event/Now.h
	shared/event/Now.cpp

 	shared/exception/BadConversion.hpp
	shared/exception/EmptyResult.hpp
	shared/exception/Exception.hpp
	shared/exception/InvalidParameter.hpp
	shared/exception/OutOfRange.hpp
	shared/exception/NotImplemented.hpp
	shared/exception/NotSupported.hpp
	shared/exception/NullReference.hpp
   
	shared/http/HttpMethods.h
	shared/http/HttpMethods.cpp

	shared/plugin/ImplementationHelper.h
	shared/plugin/IPlugin.h
	
	shared/plugin/information/IInformation.h

   shared/plugin/yPluginApi/commonIncludes.h
   shared/plugin/yPluginApi/IDeviceCommand.h
	shared/plugin/yPluginApi/IManuallyDeviceCreationData.h
	shared/plugin/yPluginApi/IManuallyDeviceCreationRequest.h
	shared/plugin/yPluginApi/IBindingQueryData.h
	shared/plugin/yPluginApi/IBindingQueryRequest.h
	shared/plugin/yPluginApi/IManuallyDeviceCreationTestData.h
	shared/plugin/yPluginApi/KeywordAccessMode.h
	shared/plugin/yPluginApi/KeywordAccessMode.cpp
	shared/plugin/yPluginApi/KeywordDataType.h
	shared/plugin/yPluginApi/KeywordDataType.cpp
	shared/plugin/yPluginApi/StandardUnits.h
	shared/plugin/yPluginApi/StandardCapacities.h
	shared/plugin/yPluginApi/StandardValues.h
	shared/plugin/yPluginApi/StandardDeclarations.cpp
	shared/plugin/yPluginApi/StandardCapacity.h
	shared/plugin/yPluginApi/StandardCapacity.cpp
	shared/plugin/yPluginApi/IYPluginApi.h

   shared/plugin/yPluginApi/historization/IHistorizable.h
   shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp
   shared/plugin/yPluginApi/historization/BatteryLevel.cpp
   shared/plugin/yPluginApi/historization/BatteryLevel.h
   shared/plugin/yPluginApi/historization/ArmingAlarm.cpp
   shared/plugin/yPluginApi/historization/ArmingAlarm.h
   shared/plugin/yPluginApi/historization/Historizers.h
   shared/plugin/yPluginApi/historization/CameraMove.cpp
   shared/plugin/yPluginApi/historization/CameraMove.h
   shared/plugin/yPluginApi/historization/Counter.cpp
   shared/plugin/yPluginApi/historization/Counter.h
   shared/plugin/yPluginApi/historization/Current.cpp
   shared/plugin/yPluginApi/historization/Current.h
   shared/plugin/yPluginApi/historization/Curtain.cpp
   shared/plugin/yPluginApi/historization/Curtain.h
   shared/plugin/yPluginApi/historization/DateTime.cpp
   shared/plugin/yPluginApi/historization/DateTime.h
   shared/plugin/yPluginApi/historization/Dimmable.cpp
   shared/plugin/yPluginApi/historization/Dimmable.h
   shared/plugin/yPluginApi/historization/Direction.cpp
   shared/plugin/yPluginApi/historization/Direction.h
   shared/plugin/yPluginApi/historization/Energy.cpp
   shared/plugin/yPluginApi/historization/Energy.h
   shared/plugin/yPluginApi/historization/Event.cpp
   shared/plugin/yPluginApi/historization/Event.h
   shared/plugin/yPluginApi/historization/Frequency.cpp
   shared/plugin/yPluginApi/historization/Frequency.h
   shared/plugin/yPluginApi/historization/Humidity.cpp
   shared/plugin/yPluginApi/historization/Humidity.h
   shared/plugin/yPluginApi/historization/Load.cpp
   shared/plugin/yPluginApi/historization/Load.h
   shared/plugin/yPluginApi/historization/Message.cpp
   shared/plugin/yPluginApi/historization/Message.h
   shared/plugin/yPluginApi/historization/MeasureType.cpp
   shared/plugin/yPluginApi/historization/MeasureType.h
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
   shared/plugin/yPluginApi/historization/Switch.cpp
   shared/plugin/yPluginApi/historization/Switch.h
   shared/plugin/yPluginApi/historization/Temperature.cpp
   shared/plugin/yPluginApi/historization/Temperature.h
   shared/plugin/yPluginApi/historization/UpDownStop.cpp
   shared/plugin/yPluginApi/historization/UpDownStop.h
   shared/plugin/yPluginApi/historization/Uv.cpp
   shared/plugin/yPluginApi/historization/Uv.h
   shared/plugin/yPluginApi/historization/Voltage.cpp
   shared/plugin/yPluginApi/historization/Voltage.h
   shared/plugin/yPluginApi/historization/Weight.cpp
   shared/plugin/yPluginApi/historization/Weight.h
   
	shared/notification/NotificationCenter.h
	shared/notification/NotificationCenter.cpp
	shared/notification/NotificationObserver.h
	shared/notification/NotificationObserver.cpp

	shared/serialization/IDataSerializable.h
	shared/serialization/IDataFileSerializable.h
   
	shared/tools/Random.h
	shared/tools/Random.cpp
)

#Append platform specific files
IF(WIN32)
set(SHARED_LIB_SRC	
	${SHARED_LIB_SRC}
	windows/shared/DynamicLibrary.h
	windows/shared/DynamicLibrary.cpp
	windows/shared/Export.h
   windows/shared/FileSystemExtension.h
	windows/shared/FileSystemExtension.cpp
	windows/shared/Peripherals.h
	windows/shared/Peripherals.cpp
	windows/shared/NetworkHelper.h
	windows/shared/NetworkHelper.cpp
)	
set(PLATFORM_SPECIFIC_INCL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/windows)

ELSEIF(APPLE)
set(SHARED_LIB_SRC	
	${SHARED_LIB_SRC}
	mac/shared/DynamicLibrary.h
	mac/shared/DynamicLibrary.cpp
	mac/shared/Export.h
   mac/shared/FileSystemExtension.h
	mac/shared/FileSystemExtension.cpp
	mac/shared/Peripherals.h
	mac/shared/Peripherals.cpp
	mac/shared/NetworkHelper.h
	mac/shared/NetworkHelper.cpp
)	
set(PLATFORM_SPECIFIC_INCL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/mac)

ELSE()
set(SHARED_LIB_SRC	
	${SHARED_LIB_SRC}
	linux/shared/DynamicLibrary.h
	linux/shared/DynamicLibrary.cpp
	linux/shared/Export.h
   linux/shared/FileSystemExtension.h
	linux/shared/FileSystemExtension.cpp
	linux/shared/Peripherals.h
	linux/shared/Peripherals.cpp
	linux/shared/NetworkHelper.h
	linux/shared/NetworkHelper.cpp
)	
set(PLATFORM_SPECIFIC_INCL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/linux)

ENDIF()

# Define project folders
source_group(communication shared/communication/*.*)
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
source_group(notification shared/shared/notification/*.* )	
source_group(serialization shared/shared/serialization/*.* )	
source_group(tools shared/shared/tools/*.* )	

source_group(platformSpecific\\windows shared/windows/shared/*.*)
source_group(platformSpecific\\linux shared/linux/shared/*.*)
source_group(platformSpecific\\mac shared/mac/shared/*.*)





########################
## Output
########################
add_library(yadoms-shared SHARED  ${SHARED_LIB_SRC})

target_link_libraries(yadoms-shared ${LIBS} ${CMAKE_DL_LIBS})
set_property( TARGET yadoms-shared PROPERTY INCLUDE_DIRECTORIES ${BOOST_INCL_DIR} ${CMAKE_CURRENT_SOURCE_DIR}  ${PLATFORM_SPECIFIC_INCL_DIR}  ${Poco_INCLUDE_DIRS})

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
set_property( TARGET yadoms-shared_unity PROPERTY INCLUDE_DIRECTORIES ${BOOST_INCL_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${PLATFORM_SPECIFIC_INCL_DIR}  ${Poco_INCLUDE_DIRS})
	endif()	
else()
	add_precompiled_header(yadoms-shared stdafx.h)
endif()






