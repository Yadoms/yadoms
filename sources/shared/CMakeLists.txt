

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
  
	shared/Event/Event.hpp
	shared/Event/EventBase.hpp
	shared/Event/EventHandler.hpp

 	shared/Exceptions/BadConversionException.hpp
	shared/Exceptions/EmptyResultException.hpp
	shared/Exceptions/Exception.hpp
	shared/Exceptions/InvalidParameterException.hpp
	shared/Exceptions/OutOfRangeException.hpp
	shared/Exceptions/NotImplementedException.hpp
	shared/Exceptions/NotSupportedException.hpp
	shared/Exceptions/NullReferenceException.hpp

  shared/plugin/ImplementationHelper.h
	
	shared/plugin/Configuration/Configuration.h
	shared/plugin/Configuration/Configuration.cpp
	shared/plugin/Configuration/ConfigurationFactory.h
	shared/plugin/Configuration/ConfigurationFactory.cpp
	shared/plugin/Configuration/BitsFieldParameter.h
	shared/plugin/Configuration/BitsFieldParameter.cpp
	shared/plugin/Configuration/BoolParameter.h
	shared/plugin/Configuration/BoolParameter.cpp
	shared/plugin/Configuration/EnumGenericParameter.h
	shared/plugin/Configuration/EnumGenericParameter.cpp
	shared/plugin/Configuration/DoubleParameter.h
	shared/plugin/Configuration/DoubleParameter.cpp
	shared/plugin/Configuration/EnumParameter.hpp
	shared/plugin/Configuration/IntParameter.h
	shared/plugin/Configuration/IntParameter.cpp
	shared/plugin/Configuration/Parameter.h
	shared/plugin/Configuration/Parameter.cpp
	shared/plugin/Configuration/Parameters.h
	shared/plugin/Configuration/SimpleParameter.hpp
	shared/plugin/Configuration/SerialPortParameter.h
	shared/plugin/Configuration/SerialPortParameter.cpp
	shared/plugin/Configuration/StringParameter.h
	shared/plugin/Configuration/StringParameter.cpp	
  
	shared/plugin/Information/IInformation.h
	shared/plugin/Information/Information.cpp
	shared/plugin/Information/Information.h

	shared/plugin/Interfaces/IHardwarePlugin.h
	shared/plugin/Interfaces/IHardwarePluginConfigurationSchema.h

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
	windows/shared/NetworkHelper.h
	windows/shared/NetworkHelper.cpp
)	
set(PLATFORM_SPECIFIC_INCL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/windows)

ELSEIF(APPLE)
set(SHARED_LIB_SRC	
	${SHARED_LIB_SRC}
	mac/shared/DynamicLibrary.hpp
	mac/shared/Export.h
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
	linux/shared/Peripherals.h
	linux/shared/Peripherals.cpp
	linux/shared/NetworkHelper.h
	linux/shared/NetworkHelper.cpp
)	
set(PLATFORM_SPECIFIC_INCL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/linux)

ENDIF()

# Define project folders
source_group(Event shared/Event/*.*)

source_group(Exceptions shared/Exceptions/*.*)

source_group(plugin shared/shared/plugin/*.* )	
source_group(plugin\\Configuration shared/shared/plugin/Configuration/*.* )	
source_group(plugin\\Information shared/shared/plugin/Information/*.* )	
source_group(plugin\\Interfaces shared/shared/plugin/Interfaces/*.* )	

source_group(PlatformSpecific\\windows shared/windows/shared/*.*)
source_group(PlatformSpecific\\linux shared/linux/shared/*.*)
source_group(PlatformSpecific\\mac shared/mac/shared/*.*)

source_group(Xpl shared/Xpl/*.*)



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


