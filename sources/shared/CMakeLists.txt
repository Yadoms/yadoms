

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

	shared/StringExtension.hpp	
  
  shared/Exceptions/BadConversionException.hpp
	shared/Exceptions/EmptyResultException.hpp
	shared/Exceptions/Exception.hpp
	shared/Exceptions/InvalidParameterException.hpp
	shared/Exceptions/OutOfRangeException.hpp
	shared/Exceptions/NotImplementedException.hpp
	shared/Exceptions/NotSupportedException.hpp
	shared/Exceptions/NullReferenceException.hpp

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

  shared/HardwarePlugin/HardwarePluginImplementationHelper.h
  shared/HardwarePlugin/Configuration/HardwarePluginConfiguration.hpp
  shared/HardwarePlugin/Configuration/HardwarePluginConfigurationParameters.hpp
  shared/HardwarePlugin/Information/HardwarePluginInformation.hpp
  shared/HardwarePlugin/Interfaces/IHardwarePlugin.h
  shared/HardwarePlugin/Interfaces/IHardwarePluginInformation.h
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

ELSEIF(APPLE)
set(SHARED_LIB_SRC	
	${SHARED_LIB_SRC}
	mac/shared/DynamicLibrary.hpp
	mac/shared/Export.h
	mac/shared/Peripherals.h
	mac/shared/Peripherals.cpp
)	
ELSE()
set(SHARED_LIB_SRC	
	${SHARED_LIB_SRC}
	linux/shared/DynamicLibrary.hpp
	linux/shared/Export.h
	linux/shared/Peripherals.h
	linux/shared/Peripherals.cpp
)	
ENDIF()

# Define project folders
IF(MSVC)
	source_group(HardwarePlugin shared/shared/HardwarePlugin/*.* )	
	source_group(HardwarePlugin\\Configuration shared/shared/HardwarePlugin/Configuration/*.* )	
	source_group(HardwarePlugin\\Information shared/shared/HardwarePlugin/Information/*.* )	
	source_group(HardwarePlugin\\Interfaces shared/shared/HardwarePlugin/Interfaces/*.* )	

 	source_group(Exceptions shared/Exceptions/*.*)

 	source_group(Xpl shared/Xpl/*.*)

	source_group(PlatformSpecific\\windows shared/windows/shared/*.*)
	source_group(PlatformSpecific\\linux shared/linux/shared/*.*)
	source_group(PlatformSpecific\\mac shared/mac/shared/*.*)
ENDIF()


########################
## Output
########################
add_library(yadoms-shared STATIC  ${SHARED_LIB_SRC})
add_precompiled_header(yadoms-shared stdafx.h)
target_link_libraries(yadoms-shared ${LIBS} ${CMAKE_DL_LIBS})


########################
## Dependencies
########################
set_property( TARGET yadoms-shared PROPERTY INCLUDE_DIRECTORIES ${BOOST_INCL_DIR} ${CMAKE_CURRENT_SOURCE_DIR})


