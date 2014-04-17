

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
	
	shared/plugin/configuration/Configuration.h
	shared/plugin/configuration/Configuration.cpp
	shared/plugin/configuration/Factory.h
	shared/plugin/configuration/Factory.cpp
  
	shared/plugin/information/IInformation.h
	shared/plugin/information/Information.cpp
	shared/plugin/information/Information.h

	shared/serialization/IPtreeToStringSerializer.h
	shared/serialization/PTreeToJsonSerializer.h
	shared/serialization/PTreeToJsonSerializer.cpp

	shared/xpl/XplConstants.h
	shared/xpl/XplConstants.cpp
	shared/xpl/XplActor.cpp
	shared/xpl/XplActor.h
	shared/xpl/XplException.h
	shared/xpl/XplHelper.cpp
	shared/xpl/XplHelper.h
	shared/xpl/XplHub.cpp
	shared/xpl/XplHub.h
	shared/xpl/XplMessage.cpp
	shared/xpl/XplMessage.h
	shared/xpl/XplMessageFactory.cpp
	shared/xpl/XplMessageFactory.h
	shared/xpl/XplMessageSchemaIdentifier.cpp
	shared/xpl/XplMessageSchemaIdentifier.h
	shared/xpl/XplService.cpp
	shared/xpl/XplService.h
	shared/xpl/XplHubConnectedPeripheral.cpp
	shared/xpl/XplHubConnectedPeripheral.h 
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
source_group(plugin\\configuration shared/shared/plugin/configuration/*.* )	
source_group(plugin\\information shared/shared/plugin/information/*.* )	
source_group(plugin\\Interfaces shared/shared/plugin/Interfaces/*.* )	

source_group(platformSpecific\\windows shared/windows/shared/*.*)
source_group(platformSpecific\\linux shared/linux/shared/*.*)
source_group(platformSpecific\\mac shared/mac/shared/*.*)

source_group(serialization shared/shared/serialization/*.* )	

source_group(xpl shared/xpl/*.*)



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


