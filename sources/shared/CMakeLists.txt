

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
	
	shared/plugin/configuration/Configuration.h
	shared/plugin/configuration/Configuration.cpp
	shared/plugin/configuration/Factory.h
	shared/plugin/configuration/Factory.cpp
	shared/plugin/configuration/BitsFieldParameter.h
	shared/plugin/configuration/BitsFieldParameter.cpp
	shared/plugin/configuration/BoolParameter.h
	shared/plugin/configuration/BoolParameter.cpp
	shared/plugin/configuration/EnumGenericParameter.h
	shared/plugin/configuration/EnumGenericParameter.cpp
	shared/plugin/configuration/DoubleParameter.h
	shared/plugin/configuration/DoubleParameter.cpp
	shared/plugin/configuration/EnumParameter.hpp
	shared/plugin/configuration/IntParameter.h
	shared/plugin/configuration/IntParameter.cpp
	shared/plugin/configuration/Parameter.h
	shared/plugin/configuration/Parameter.cpp
	shared/plugin/configuration/Parameters.h
	shared/plugin/configuration/SimpleParameter.hpp
	shared/plugin/configuration/SerialPortParameter.h
	shared/plugin/configuration/SerialPortParameter.cpp
	shared/plugin/configuration/StringParameter.h
	shared/plugin/configuration/StringParameter.cpp	
  
	shared/plugin/information/IInformation.h
	shared/plugin/information/Information.cpp
	shared/plugin/information/Information.h

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
IF(WIN32)
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
source_group(plugin\\configuration shared/shared/plugin/configuration/*.* )	
source_group(plugin\\information shared/shared/plugin/information/*.* )	
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


