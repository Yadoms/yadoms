

########################
## Sources
########################
set(SHARED_LIB_SRC
	stdafx.cpp
	stdafx.h

 	shared/Log.h
	shared/Log.cpp

	shared/ThreadBase.cpp
	shared/ThreadBase.h

	shared/StringExtension.h
	shared/StringExtension.cpp	
	
	shared/enumeration/IExtendedEnum.h
	shared/enumeration/EnumHelpers.hpp
	
	shared/Field.hpp

	shared/IDataContainable.h
	shared/DataContainer.h
	shared/DataContainer.cpp
	shared/serialization/IDataSerializable.h
	shared/serialization/IDataFileSerializable.h
  
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

	shared/plugin/ImplementationHelper.h
	shared/plugin/IPlugin.h
	
	shared/plugin/information/IInformation.h

   shared/plugin/yadomsApi/IDeviceCommand.h
   shared/plugin/yadomsApi/IManuallyDeviceCreationData.h
   shared/plugin/yadomsApi/IManuallyDeviceCreationTestData.h
	shared/plugin/yadomsApi/StandardUnits.h
	shared/plugin/yadomsApi/StandardCapacities.h
	shared/plugin/yadomsApi/StandardValues.h
	shared/plugin/yadomsApi/StandardDeclarations.cpp
	shared/plugin/yadomsApi/StandardCapacity.h
	shared/plugin/yadomsApi/StandardCapacity.cpp
	shared/plugin/yadomsApi/KeywordAccessMode.h
	shared/plugin/yadomsApi/IYadomsApi.h

   shared/plugin/yadomsApi/commands/Curtain.cpp
   shared/plugin/yadomsApi/commands/Curtain.h
   shared/plugin/yadomsApi/commands/Message.cpp
   shared/plugin/yadomsApi/commands/Message.h
   shared/plugin/yadomsApi/commands/Switch.cpp
   shared/plugin/yadomsApi/commands/Switch.h
   
	shared/encryption/Xor.h
	shared/encryption/Xor.cpp
	shared/encryption/Base64.h
	shared/encryption/Base64.cpp
	shared/encryption/md5.h
	
	shared/tools/Random.h
)

#Append platform specific files
IF(WIN32)
set(SHARED_LIB_SRC	
	${SHARED_LIB_SRC}
	windows/shared/DynamicLibrary.hpp
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
	mac/shared/DynamicLibrary.hpp
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
	linux/shared/DynamicLibrary.hpp
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
source_group(event shared/event/*.*)

source_group(exception shared/exception/*.*)

source_group(plugin shared/shared/plugin/*.* )	
source_group(plugin\\information shared/shared/plugin/information/*.* )	
source_group(plugin\\yadomsApi shared/shared/plugin/yadomsApi/*.* )	
source_group(plugin\\yadomsApi\\commands shared/shared/plugin/yadomsApi/commands/*.* )	

source_group(platformSpecific\\windows shared/windows/shared/*.*)
source_group(platformSpecific\\linux shared/linux/shared/*.*)
source_group(platformSpecific\\mac shared/mac/shared/*.*)

source_group(serialization shared/shared/serialization/*.* )	
source_group(encryption shared/shared/encryption/*.* )	
source_group(tools shared/shared/tools/*.* )	
source_group(enumeration shared/shared/enumeration/*.* )	




########################
## Output
########################
add_library(yadoms-shared SHARED  ${SHARED_LIB_SRC})
add_precompiled_header(yadoms-shared stdafx.h)
target_link_libraries(yadoms-shared ${LIBS} ${CMAKE_DL_LIBS})


########################
## Dependencies
########################
set_property( TARGET yadoms-shared PROPERTY INCLUDE_DIRECTORIES ${BOOST_INCL_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${PLATFORM_SPECIFIC_INCL_DIR})


