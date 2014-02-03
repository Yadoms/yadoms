

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
  
  	shared/Exceptions/BadConversionException.hpp
	shared/Exceptions/EmptyResultException.hpp
	shared/Exceptions/Exception.hpp
	shared/Exceptions/InvalidParameterException.hpp
	shared/Exceptions/OutOfRangeException.hpp
	shared/Exceptions/NotImplementedException.hpp
	shared/Exceptions/NotSupportedException.hpp
	shared/Exceptions/NullReferenceException.hpp

  	shared/HardwarePlugin/ImplementationHelper.h
	
	shared/HardwarePlugin/Configuration/Configuration.h
	shared/HardwarePlugin/Configuration/Configuration.cpp
	shared/HardwarePlugin/Configuration/ConfigurationFactory.h
	shared/HardwarePlugin/Configuration/ConfigurationFactory.cpp
	shared/HardwarePlugin/Configuration/BitsFieldParameter.h
	shared/HardwarePlugin/Configuration/BitsFieldParameter.cpp
	shared/HardwarePlugin/Configuration/BoolParameter.h
	shared/HardwarePlugin/Configuration/BoolParameter.cpp
	shared/HardwarePlugin/Configuration/EnumGenericParameter.h
	shared/HardwarePlugin/Configuration/EnumGenericParameter.cpp
	shared/HardwarePlugin/Configuration/DoubleParameter.h
	shared/HardwarePlugin/Configuration/DoubleParameter.cpp
	shared/HardwarePlugin/Configuration/EnumParameter.hpp
	shared/HardwarePlugin/Configuration/IntParameter.h
	shared/HardwarePlugin/Configuration/IntParameter.cpp
	shared/HardwarePlugin/Configuration/Parameter.h
	shared/HardwarePlugin/Configuration/Parameter.cpp
	shared/HardwarePlugin/Configuration/Parameters.h
	shared/HardwarePlugin/Configuration/SimpleParameter.hpp
	shared/HardwarePlugin/Configuration/SerialPortParameter.h
	shared/HardwarePlugin/Configuration/SerialPortParameter.cpp
	shared/HardwarePlugin/Configuration/StringParameter.h
	shared/HardwarePlugin/Configuration/StringParameter.cpp	
	shared/HardwarePlugin/Information/Information.cpp
	shared/HardwarePlugin/Information/Information.h

	shared/HardwarePlugin/Interfaces/IHardwarePlugin.h
	shared/HardwarePlugin/Interfaces/IHardwarePluginInformation.h
	shared/HardwarePlugin/Interfaces/IHardwarePluginConfigurationSchema.h

	shared/Serialization/IPtreeToStringSerializer.h
	shared/Serialization/PTreeToJsonSerializer.h
	shared/Serialization/PTreeToJsonSerializer.cpp

	shared/Xpl/XplConstants.h
	shared/Xpl/XplConstants.cpp
	shared/Xpl/XplActor.cpp
	shared/Xpl/XplActor.h
	shared/Xpl/XplException.h
	shared/Xpl/XplHelper.cpp
	shared/Xpl/XplHelper.h
	shared/Xpl/XplHub.cpp
	shared/Xpl/XplHub.h
	shared/Xpl/XplMessage.cpp
	shared/Xpl/XplMessage.h
	shared/Xpl/XplMessageFactory.cpp
	shared/Xpl/XplMessageFactory.h
	shared/Xpl/XplMessageSchemaIdentifier.cpp
	shared/Xpl/XplMessageSchemaIdentifier.h
	shared/Xpl/XplService.cpp
	shared/Xpl/XplService.h
	shared/Xpl/XplHubConnectedPeripheral.cpp
	shared/Xpl/XplHubConnectedPeripheral.h 
)

#Append platform specific files
IF(MSVC)
set(SHARED_LIB_SRC	
	${SHARED_LIB_SRC}
	windows/shared/DynamicLibrary.hpp
	windows/shared/Export.h
	windows/shared/Peripherals.h
	windows/shared/Peripherals.cpp
)	
set(PLATFORM_SPECIFIC_INCL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/windows)

ELSEIF(APPLE)
set(SHARED_LIB_SRC	
	${SHARED_LIB_SRC}
	mac/shared/DynamicLibrary.hpp
	mac/shared/Export.h
	mac/shared/Peripherals.h
	mac/shared/Peripherals.cpp
)	
set(PLATFORM_SPECIFIC_INCL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/mac)

ELSE()
set(SHARED_LIB_SRC	
	${SHARED_LIB_SRC}
	linux/shared/DynamicLibrary.hpp
	linux/shared/Export.h
	linux/shared/Peripherals.h
	linux/shared/Peripherals.cpp
)	
set(PLATFORM_SPECIFIC_INCL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/linux)

ENDIF()

# Define project folders
source_group(HardwarePlugin shared/shared/HardwarePlugin/*.* )	
source_group(HardwarePlugin\\Configuration shared/shared/HardwarePlugin/Configuration/*.* )	
source_group(HardwarePlugin\\Information shared/shared/HardwarePlugin/Information/*.* )	
source_group(HardwarePlugin\\Interfaces shared/shared/HardwarePlugin/Interfaces/*.* )	

source_group(Exceptions shared/Exceptions/*.*)

source_group(Xpl shared/Xpl/*.*)

source_group(PlatformSpecific\\windows shared/windows/shared/*.*)
source_group(PlatformSpecific\\linux shared/linux/shared/*.*)
source_group(PlatformSpecific\\mac shared/mac/shared/*.*)


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


