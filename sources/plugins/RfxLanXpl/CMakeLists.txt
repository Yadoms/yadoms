# include common plugin definitions
include("../YadomsPluginsCommon.cmake")

#########################################
## Place here all the plugin source files
#########################################
set(RFXLANXPL_CORE_SOURCES
	xplcore/XplConstants.h
	xplcore/XplConstants.cpp
	xplcore/XplActor.cpp
	xplcore/XplActor.h
	xplcore/XplException.h
	xplcore/XplHelper.cpp
	xplcore/XplHelper.h
	xplcore/XplHub.cpp
	xplcore/XplHub.h
	xplcore/XplMessage.cpp
	xplcore/XplMessage.h
	xplcore/XplMessageFactory.cpp
	xplcore/XplMessageFactory.h
	xplcore/XplMessageFilter.cpp
	xplcore/XplMessageFilter.h
	xplcore/XplMessageSchemaIdentifier.cpp
	xplcore/XplMessageSchemaIdentifier.h
	xplcore/XplService.cpp
	xplcore/XplService.h
	xplcore/XplServiceTask.cpp
	xplcore/XplServiceTask.h
	xplcore/XplHubConnectedPeripheral.cpp
	xplcore/XplHubConnectedPeripheral.h 
)

set(RFXLANXPL_RULES_SOURCES
	xplrules/IRule.h
	xplrules/IReadRule.h
	xplrules/ICommandRule.h
	xplrules/ISupportManuallyDeviceCreationRule.h
	xplrules/IProtocol.h
	xplrules/Protocol.h
	xplrules/IDeviceManager.h
	xplrules/DeviceIdentifier.h
	xplrules/DeviceIdentifier.cpp
	xplrules/DeviceContainer.h
	xplrules/DeviceContainer.cpp
	xplrules/RuleInstanceManager.hpp
	xplrules/rfxLanXpl/DeviceManager.h
	xplrules/rfxLanXpl/DeviceManager.cpp
	xplrules/rfxLanXpl/AcBasic.h
	xplrules/rfxLanXpl/AcBasic.cpp
	xplrules/rfxLanXpl/DateTimeBasic.h
	xplrules/rfxLanXpl/DateTimeBasic.cpp
	xplrules/rfxLanXpl/ControlBasic.h
	xplrules/rfxLanXpl/ControlBasic.cpp
	xplrules/rfxLanXpl/LogBasic.h
	xplrules/rfxLanXpl/LogBasic.cpp
	xplrules/rfxLanXpl/RemoteBasic.h
	xplrules/rfxLanXpl/RemoteBasic.cpp
	xplrules/rfxLanXpl/SensorBasic.h
	xplrules/rfxLanXpl/SensorBasic.cpp
	xplrules/rfxLanXpl/X10Basic.h
	xplrules/rfxLanXpl/X10Basic.cpp
	xplrules/rfxLanXpl/X10Security.h
	xplrules/rfxLanXpl/X10Security.cpp	

	xplrules/rfxLanXpl/data/MertikCommand.h
	xplrules/rfxLanXpl/data/MertikCommand.cpp
	xplrules/rfxLanXpl/data/MertikStatus.h
	xplrules/rfxLanXpl/data/MertikStatus.cpp
	xplrules/rfxLanXpl/data/Ninja.h
	xplrules/rfxLanXpl/data/Ninja.cpp
	xplrules/rfxLanXpl/data/DigitalIoCommand.h
	xplrules/rfxLanXpl/data/DigitalIoCommand.cpp	
	xplrules/rfxLanXpl/data/DigitalIoStatus.h
	xplrules/rfxLanXpl/data/DigitalIoStatus.cpp	
	xplrules/rfxLanXpl/data/DigimaxDemand.h
	xplrules/rfxLanXpl/data/DigimaxDemand.cpp
	xplrules/rfxLanXpl/data/HumidityDescription.h
	xplrules/rfxLanXpl/data/HumidityDescription.cpp
	xplrules/rfxLanXpl/data/UvDescription.h
	xplrules/rfxLanXpl/data/UvDescription.cpp
	xplrules/rfxLanXpl/data/Forecast.h
	xplrules/rfxLanXpl/data/Forecast.cpp
	xplrules/rfxLanXpl/data/Log.h
	xplrules/rfxLanXpl/data/Log.cpp
	xplrules/rfxLanXpl/data/Security.h
	xplrules/rfxLanXpl/data/Security.cpp
	xplrules/rfxLanXpl/data/RemotePC.h
	xplrules/rfxLanXpl/data/RemotePC.cpp
	xplrules/rfxLanXpl/data/RemoteAtiWonder.h
	xplrules/rfxLanXpl/data/RemoteAtiWonder.cpp
	xplrules/rfxLanXpl/data/RemoteAtiWonderPlus.h
	xplrules/rfxLanXpl/data/RemoteAtiWonderPlus.cpp
	xplrules/rfxLanXpl/data/RemoteMedion.h
	xplrules/rfxLanXpl/data/RemoteMedion.cpp
)


set(RFXLANXPL_PLUGIN_SOURCES
	stdafx.h
	stdafx.cpp
	RfxLanXpl.h
	RfxLanXpl.cpp 
	RfxLanXplConfiguration.h
	RfxLanXplConfiguration.cpp 
	${RFXLANXPL_CORE_SOURCES}
	${RFXLANXPL_RULES_SOURCES}
)

#########################################
## Place here all additional groups (solution folder)
#########################################
source_group(sources ./*.*)
source_group(sources\\xplcore xplcore/*.*)
source_group(sources\\xplrules xplrules/*.*)
source_group(sources\\xplrules\\rfxLanXpl xplrules/rfxLanXpl/*.*)
source_group(sources\\xplrules\\rfxLanXpl\\data xplrules/rfxLanXpl/data/*.*)


#########################################
## Place here all additional include directories
## (by default yadoms-shared and boost are included)
#########################################
set(RFXLANXPL_PLUGIN_INCLDIR
)

#########################################
## Place here all additional libraries to link with
## (by default yadoms-shared and boost are included)
#########################################
set(RFXLANXPL_PLUGIN_LINK
)

#########################################
## Place here all additional compiler flags
## i.e. : for raspberry to link with wiringPi : use add_definition("-lwiringPi")
#########################################
##add_definitions("-lwiringPi")

#########################################
## Always leave these lines to configure plugin correctly
## even if FAKE_PLUGIN_INCLDIR or FAKE_PLUGIN_LINK are empty
#########################################
PLUGIN_SOURCES(RfxLanXpl ${RFXLANXPL_PLUGIN_SOURCES})
PLUGIN_INCLDIR(RfxLanXpl ${RFXLANXPL_PLUGIN_ADDITIONAL_INC_DIR})
PLUGIN_LINK(RfxLanXpl ${RFXLANXPL_PLUGIN_ADDITIONAL_LIB})

#########################################
## Post-build commands
## Copy files to build directory
## "package.json" and "icon.png" are needed, but you can add extra-files
#########################################
PLUGIN_POST_BUILD_COPY_FILE(RfxLanXpl package.json)
PLUGIN_POST_BUILD_COPY_FILE(RfxLanXpl changelog.md)
PLUGIN_POST_BUILD_COPY_FILE(RfxLanXpl icon.png)
PLUGIN_POST_BUILD_COPY_DIRECTORY(RfxLanXpl locales)
