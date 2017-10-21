#########################################
## The file allow building Openzwave for anyplatform
##
## Build tested with:
##		- Windows /VisualStudio 2013
##		- MacOS / Xcode
##		- Linux Ubuntu / makefile
##		- Raspberry (cross compiled from Ubuntu) / makefile
##
#########################################

#########################################
## Define the main folder name
#########################################

#When changing openzwave, remeber to enable exceptions, by undefining OPENZWAVE_DISABLE_EXCEPTIONS (cpp/src/Defs.h, line 149 in v1.4.1)
#Include "vers.cpp" to git manually
set(OPENZWAVE_DIR_NAME openzwave-1.4.2766 )


#########################################
## Main sources
#########################################
set(OPENZWAVE_MAIN_SOURCES
		${OPENZWAVE_DIR_NAME}/cpp/src/Bitfield.h
		${OPENZWAVE_DIR_NAME}/cpp/src/Defs.h
		${OPENZWAVE_DIR_NAME}/cpp/src/DoxygenMain.h
		${OPENZWAVE_DIR_NAME}/cpp/src/Driver.h
		${OPENZWAVE_DIR_NAME}/cpp/src/Driver.cpp
		${OPENZWAVE_DIR_NAME}/cpp/src/Group.h
		${OPENZWAVE_DIR_NAME}/cpp/src/Group.cpp
		${OPENZWAVE_DIR_NAME}/cpp/src/Manager.h
		${OPENZWAVE_DIR_NAME}/cpp/src/Manager.cpp
		${OPENZWAVE_DIR_NAME}/cpp/src/Msg.h
		${OPENZWAVE_DIR_NAME}/cpp/src/Msg.cpp
		${OPENZWAVE_DIR_NAME}/cpp/src/Node.h
		${OPENZWAVE_DIR_NAME}/cpp/src/Node.cpp
		${OPENZWAVE_DIR_NAME}/cpp/src/Notification.h
		${OPENZWAVE_DIR_NAME}/cpp/src/Notification.cpp
		${OPENZWAVE_DIR_NAME}/cpp/src/Options.h
		${OPENZWAVE_DIR_NAME}/cpp/src/Options.cpp
		${OPENZWAVE_DIR_NAME}/cpp/src/OZWException.h
		${OPENZWAVE_DIR_NAME}/cpp/src/Scene.h
		${OPENZWAVE_DIR_NAME}/cpp/src/Scene.cpp
		${OPENZWAVE_DIR_NAME}/cpp/src/Utils.h
		${OPENZWAVE_DIR_NAME}/cpp/src/Utils.cpp
		${OPENZWAVE_DIR_NAME}/cpp/src/vers.cpp
		${OPENZWAVE_DIR_NAME}/cpp/src/ZWSecurity.h
		${OPENZWAVE_DIR_NAME}/cpp/src/ZWSecurity.cpp
)  

#########################################
## CommandClasses sources
#########################################
set(OPENZWAVE_COMMANDCLASSES_SOURCES
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/Alarm.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/Alarm.h
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/ApplicationStatus.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/ApplicationStatus.h
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/Association.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/Association.h
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/AssociationCommandConfiguration.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/AssociationCommandConfiguration.h
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/Basic.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/Basic.h
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/BasicWindowCovering.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/BasicWindowCovering.h
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/Battery.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/Battery.h
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/CentralScene.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/CentralScene.h
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/ClimateControlSchedule.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/ClimateControlSchedule.h
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/Clock.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/Clock.h
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/Color.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/Color.h
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/CommandClass.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/CommandClass.h
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/CommandClasses.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/CommandClasses.h
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/Configuration.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/Configuration.h
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/ControllerReplication.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/ControllerReplication.h
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/CRC16Encap.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/CRC16Encap.h
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/DeviceResetLocally.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/DeviceResetLocally.h
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/DoorLock.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/DoorLock.h
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/DoorLockLogging.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/DoorLockLogging.h
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/EnergyProduction.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/EnergyProduction.h
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/Hail.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/Hail.h
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/Indicator.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/Indicator.h
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/Language.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/Language.h
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/Lock.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/Lock.h
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/ManufacturerSpecific.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/ManufacturerSpecific.h
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/Meter.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/Meter.h
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/MeterPulse.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/MeterPulse.h
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/MultiChannelAssociation.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/MultiChannelAssociation.h
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/MultiCmd.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/MultiCmd.h
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/MultiInstance.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/MultiInstance.h
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/NodeNaming.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/NodeNaming.h
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/NoOperation.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/NoOperation.h
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/Powerlevel.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/Powerlevel.h
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/Proprietary.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/Proprietary.h
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/Protection.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/Protection.h
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/SceneActivation.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/SceneActivation.h
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/Security.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/Security.h
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/SensorAlarm.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/SensorAlarm.h
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/SensorBinary.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/SensorBinary.h
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/SensorMultilevel.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/SensorMultilevel.h
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/SwitchAll.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/SwitchAll.h
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/SwitchBinary.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/SwitchBinary.h
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/SwitchMultilevel.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/SwitchMultilevel.h
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/SwitchToggleBinary.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/SwitchToggleBinary.h
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/SwitchToggleMultilevel.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/SwitchToggleMultilevel.h
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/ThermostatFanMode.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/ThermostatFanMode.h
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/ThermostatFanState.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/ThermostatFanState.h
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/ThermostatMode.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/ThermostatMode.h
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/ThermostatOperatingState.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/ThermostatOperatingState.h
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/ThermostatSetpoint.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/ThermostatSetpoint.h
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/TimeParameters.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/TimeParameters.h
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/UserCode.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/UserCode.h
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/Version.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/Version.h
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/WakeUp.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/WakeUp.h
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/ZWavePlusInfo.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/ZWavePlusInfo.h
)

#########################################
## ValueClasses sources
#########################################
set(OPENZWAVE_VALUECLASSES_SOURCES
	${OPENZWAVE_DIR_NAME}/cpp/src/value_classes/Value.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/value_classes/Value.h
	${OPENZWAVE_DIR_NAME}/cpp/src/value_classes/ValueBool.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/value_classes/ValueBool.h
	${OPENZWAVE_DIR_NAME}/cpp/src/value_classes/ValueButton.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/value_classes/ValueButton.h
	${OPENZWAVE_DIR_NAME}/cpp/src/value_classes/ValueByte.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/value_classes/ValueByte.h
	${OPENZWAVE_DIR_NAME}/cpp/src/value_classes/ValueDecimal.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/value_classes/ValueDecimal.h
	${OPENZWAVE_DIR_NAME}/cpp/src/value_classes/ValueID.h
	${OPENZWAVE_DIR_NAME}/cpp/src/value_classes/ValueInt.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/value_classes/ValueInt.h
	${OPENZWAVE_DIR_NAME}/cpp/src/value_classes/ValueList.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/value_classes/ValueList.h
	${OPENZWAVE_DIR_NAME}/cpp/src/value_classes/ValueRaw.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/value_classes/ValueRaw.h
	${OPENZWAVE_DIR_NAME}/cpp/src/value_classes/ValueSchedule.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/value_classes/ValueSchedule.h
	${OPENZWAVE_DIR_NAME}/cpp/src/value_classes/ValueShort.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/value_classes/ValueShort.h
	${OPENZWAVE_DIR_NAME}/cpp/src/value_classes/ValueStore.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/value_classes/ValueStore.h
	${OPENZWAVE_DIR_NAME}/cpp/src/value_classes/ValueString.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/value_classes/ValueString.h
)


#########################################
## Platform sources
#########################################
set(OPENZWAVE_PLATFORM_SOURCES
	${OPENZWAVE_DIR_NAME}/cpp/src/platform/Controller.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/platform/Controller.h
	${OPENZWAVE_DIR_NAME}/cpp/src/platform/Event.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/platform/Event.h
	${OPENZWAVE_DIR_NAME}/cpp/src/platform/FileOps.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/platform/FileOps.h
	${OPENZWAVE_DIR_NAME}/cpp/src/platform/HidController.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/platform/HidController.h
	${OPENZWAVE_DIR_NAME}/cpp/src/platform/Log.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/platform/Log.h
	${OPENZWAVE_DIR_NAME}/cpp/src/platform/Mutex.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/platform/Mutex.h
	${OPENZWAVE_DIR_NAME}/cpp/src/platform/Ref.h
	${OPENZWAVE_DIR_NAME}/cpp/src/platform/SerialController.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/platform/SerialController.h
	${OPENZWAVE_DIR_NAME}/cpp/src/platform/Stream.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/platform/Stream.h
	${OPENZWAVE_DIR_NAME}/cpp/src/platform/Thread.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/platform/Thread.h
	${OPENZWAVE_DIR_NAME}/cpp/src/platform/TimeStamp.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/platform/TimeStamp.h
	${OPENZWAVE_DIR_NAME}/cpp/src/platform/Wait.cpp
	${OPENZWAVE_DIR_NAME}/cpp/src/platform/Wait.h
)

#########################################
## Platform specific sources
#########################################
if ( WIN32 )

	set(OPENZWAVE_PLATFORM_SOURCES
		${OPENZWAVE_PLATFORM_SOURCES}
		${OPENZWAVE_DIR_NAME}/cpp/src/platform/windows/EventImpl.cpp
		${OPENZWAVE_DIR_NAME}/cpp/src/platform/windows/EventImpl.h
		${OPENZWAVE_DIR_NAME}/cpp/src/platform/windows/FileOpsImpl.cpp
		${OPENZWAVE_DIR_NAME}/cpp/src/platform/windows/FileOpsImpl.h
		${OPENZWAVE_DIR_NAME}/cpp/src/platform/windows/LogImpl.cpp
		${OPENZWAVE_DIR_NAME}/cpp/src/platform/windows/LogImpl.h
		${OPENZWAVE_DIR_NAME}/cpp/src/platform/windows/MutexImpl.cpp
		${OPENZWAVE_DIR_NAME}/cpp/src/platform/windows/MutexImpl.h
		${OPENZWAVE_DIR_NAME}/cpp/src/platform/windows/SerialControllerImpl.cpp
		${OPENZWAVE_DIR_NAME}/cpp/src/platform/windows/SerialControllerImpl.h
		${OPENZWAVE_DIR_NAME}/cpp/src/platform/windows/ThreadImpl.cpp
		${OPENZWAVE_DIR_NAME}/cpp/src/platform/windows/ThreadImpl.h
		${OPENZWAVE_DIR_NAME}/cpp/src/platform/windows/TimeStampImpl.cpp
		${OPENZWAVE_DIR_NAME}/cpp/src/platform/windows/TimeStampImpl.h
		${OPENZWAVE_DIR_NAME}/cpp/src/platform/windows/WaitImpl.cpp
		${OPENZWAVE_DIR_NAME}/cpp/src/platform/windows/WaitImpl.h
	) 

else()

	set(OPENZWAVE_PLATFORM_SOURCES
		${OPENZWAVE_PLATFORM_SOURCES}
		${OPENZWAVE_DIR_NAME}/cpp/src/platform/unix/EventImpl.cpp
		${OPENZWAVE_DIR_NAME}/cpp/src/platform/unix/EventImpl.h
		${OPENZWAVE_DIR_NAME}/cpp/src/platform/unix/FileOpsImpl.cpp
		${OPENZWAVE_DIR_NAME}/cpp/src/platform/unix/FileOpsImpl.h
		${OPENZWAVE_DIR_NAME}/cpp/src/platform/unix/LogImpl.cpp
		${OPENZWAVE_DIR_NAME}/cpp/src/platform/unix/LogImpl.h
		${OPENZWAVE_DIR_NAME}/cpp/src/platform/unix/MutexImpl.cpp
		${OPENZWAVE_DIR_NAME}/cpp/src/platform/unix/MutexImpl.h
		${OPENZWAVE_DIR_NAME}/cpp/src/platform/unix/SerialControllerImpl.cpp
		${OPENZWAVE_DIR_NAME}/cpp/src/platform/unix/SerialControllerImpl.h
		${OPENZWAVE_DIR_NAME}/cpp/src/platform/unix/ThreadImpl.cpp
		${OPENZWAVE_DIR_NAME}/cpp/src/platform/unix/ThreadImpl.h
		${OPENZWAVE_DIR_NAME}/cpp/src/platform/unix/TimeStampImpl.cpp
		${OPENZWAVE_DIR_NAME}/cpp/src/platform/unix/TimeStampImpl.h
		${OPENZWAVE_DIR_NAME}/cpp/src/platform/unix/WaitImpl.cpp
		${OPENZWAVE_DIR_NAME}/cpp/src/platform/unix/WaitImpl.h
	) 

endif()


#########################################
## AES sources
#########################################
set(OPENZWAVE_AES_SOURCES
	${OPENZWAVE_DIR_NAME}/cpp/src/aes/aes.h
	${OPENZWAVE_DIR_NAME}/cpp/src/aes/aes_modes.c
	${OPENZWAVE_DIR_NAME}/cpp/src/aes/aescpp.h
	${OPENZWAVE_DIR_NAME}/cpp/src/aes/aescrypt.c
	${OPENZWAVE_DIR_NAME}/cpp/src/aes/aeskey.c
	${OPENZWAVE_DIR_NAME}/cpp/src/aes/aesopt.h
	${OPENZWAVE_DIR_NAME}/cpp/src/aes/aestab.c
	${OPENZWAVE_DIR_NAME}/cpp/src/aes/aestab.h
	${OPENZWAVE_DIR_NAME}/cpp/src/aes/brg_endian.h
	${OPENZWAVE_DIR_NAME}/cpp/src/aes/brg_types.h
)

#########################################
## TinyXML sources
#########################################
set(OPENZWAVE_TINYXML_SOURCES
	${OPENZWAVE_DIR_NAME}/cpp/tinyxml/tinyxml.h
	${OPENZWAVE_DIR_NAME}/cpp/tinyxml/tinyxml.cpp
	${OPENZWAVE_DIR_NAME}/cpp/tinyxml/tinystr.h
	${OPENZWAVE_DIR_NAME}/cpp/tinyxml/tinystr.cpp
	${OPENZWAVE_DIR_NAME}/cpp/tinyxml/tinyxmlerror.cpp
	${OPENZWAVE_DIR_NAME}/cpp/tinyxml/tinyxmlparser.cpp
)

#########################################
## HidAPI sources
#########################################
if ( WIN32 )
	set(OPENZWAVE_HIDAPI_SOURCES
		${OPENZWAVE_DIR_NAME}/cpp/hidapi/windows/hid.cpp
		${OPENZWAVE_DIR_NAME}/cpp/hidapi/hidapi/hidapi.h
	) 
	
ELSEIF(APPLE)
	set(OPENZWAVE_HIDAPI_SOURCES
		${OPENZWAVE_DIR_NAME}/cpp/hidapi/mac/hid.c
		${OPENZWAVE_DIR_NAME}/cpp/hidapi/hidapi/hidapi.h
	) 

else()
	set(OPENZWAVE_HIDAPI_SOURCES
		${OPENZWAVE_DIR_NAME}/cpp/hidapi/linux/hid.c
		${OPENZWAVE_DIR_NAME}/cpp/hidapi/hidapi/hidapi.h
	) 
	
endif()


#########################################
## All sources
#########################################
set(OPENZWAVE_SOURCES
	${OPENZWAVE_MAIN_SOURCES}
	${OPENZWAVE_COMMANDCLASSES_SOURCES}
	${OPENZWAVE_VALUECLASSES_SOURCES}
	${OPENZWAVE_PLATFORM_SOURCES}
   ${OPENZWAVE_AES_SOURCES}
	${OPENZWAVE_TINYXML_SOURCES}
	${OPENZWAVE_HIDAPI_SOURCES}
)

#########################################
## Define sourcegroup for IDE (msvc, xcode,...)
#########################################
source_group(Main ${OPENZWAVE_DIR_NAME}/cpp/src/*.*)
source_group(CommandClasses ${OPENZWAVE_DIR_NAME}/cpp/src/command_classes/*.*)
source_group(ValueClasses ${OPENZWAVE_DIR_NAME}/cpp/src/value_classes/*.*)
source_group(Platform ${OPENZWAVE_DIR_NAME}/cpp/src/platform/*.*)
source_group(AES ${OPENZWAVE_DIR_NAME}/cpp/src/aes/*.*)

if ( WIN32 )
	source_group(Platform\\windows ${OPENZWAVE_DIR_NAME}/cpp/src/platform/windows/*.*)
else()
	source_group(Platform\\unix ${OPENZWAVE_DIR_NAME}/cpp/src/platform/unix/*.*)
endif()


#########################################
## Remove specific warnings
#########################################
if(CMAKE_COMPILER_IS_GNUCXX)
	add_definitions ("-Wno-unknown-pragmas")
	add_definitions ("-Wno-sequence-point")
endif(CMAKE_COMPILER_IS_GNUCXX)

#########################################
## Define project outputs
#########################################
add_library ( openzwave STATIC ${OPENZWAVE_SOURCES} )
set_target_properties( openzwave PROPERTIES OUTPUT_NAME "openzwave" )
set_target_properties( openzwave PROPERTIES LIBRARY_OUTPUT_DIRECTORY static )


#########################################
## Define all include path
#########################################
set(OPENZWAVE_INCLUDE_SHARED_DIRECTORY 
	${CMAKE_CURRENT_SOURCE_DIR}/${OPENZWAVE_DIR_NAME}/cpp/src/
)

set(OPENZWAVE_INCLUDE_DIRECTORY 
	${LIBUDEV_ROOT}
	${OPENZWAVE_INCLUDE_SHARED_DIRECTORY}
	${OPENZWAVE_INCLUDE_SHARED_DIRECTORY}/command_classes/
	${OPENZWAVE_INCLUDE_SHARED_DIRECTORY}/value_classes/
	${OPENZWAVE_INCLUDE_SHARED_DIRECTORY}/platform/
	${CMAKE_CURRENT_SOURCE_DIR}/${OPENZWAVE_DIR_NAME}/cpp/aes/
	${CMAKE_CURRENT_SOURCE_DIR}/${OPENZWAVE_DIR_NAME}/cpp/hidapi/hidapi/
	${CMAKE_CURRENT_SOURCE_DIR}/${OPENZWAVE_DIR_NAME}/cpp/tinyxml/
)

if ( WIN32 )
	set(OPENZWAVE_INCLUDE_DIRECTORY 
		${OPENZWAVE_INCLUDE_DIRECTORY}
		${CMAKE_CURRENT_SOURCE_DIR}/${OPENZWAVE_DIR_NAME}/cpp/src/platform/windows/
	)
else()
	#for raspberry, we need to use libudev folder
	if(CMAKE_COMPILER_IS_RASPBERRY_CROSS_COMPILER)
	set(	OPENZWAVE_INCLUDE_DIRECTORY 
		${OPENZWAVE_INCLUDE_DIRECTORY}
		${CMAKE_CURRENT_SOURCE_DIR}/libudev/
	)
	set(UDEV_PATH_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/libudev)

	endif()

	set(	OPENZWAVE_INCLUDE_DIRECTORY 
		${OPENZWAVE_INCLUDE_DIRECTORY}
		${CMAKE_CURRENT_SOURCE_DIR}/${OPENZWAVE_DIR_NAME}/cpp/src/platform/unix/
	)
endif()
	

set_property( TARGET openzwave PROPERTY INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR} ${BOOST_ROOT} ${OPENZWAVE_INCLUDE_DIRECTORY})

#########################################
## Windows specific processing
#########################################
# Strip 'lib' prefix when compiling for windows
if(WIN32)
  set_target_properties ( openzwave PROPERTIES PREFIX "" )

  #set warning level3 only for this project
	add_definitions("/W3")
endif()

#specify that openzwave use exception instead of exit statements
add_definitions(-DOPENZWAVE_USE_EXCEPTIONS)

#########################################
## Share include directories for project which use openzwave
#########################################
set(OPENZWAVE_INCLUDE_DIR2 "${CMAKE_CURRENT_SOURCE_DIR}" )
set(OPENZWAVE_INCLUDE_DIRECTORY 
	${OPENZWAVE_INCLUDE_DIR2}
	${OPENZWAVE_INCLUDE_SHARED_DIRECTORY}
	PARENT_SCOPE
)

#share also config directory
set(OPENZWAVE_CONF_DIRECTORY  
   "${CMAKE_CURRENT_SOURCE_DIR}/${OPENZWAVE_DIR_NAME}/config"
	PARENT_SCOPE
)

