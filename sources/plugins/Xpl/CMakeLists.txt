# include common plugin definitions
include("../YadomsPluginsCommon.cmake")

#########################################
## Place here all the plugin source files
#########################################
set(XPL_CORE_SOURCES
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
	xplcore/XplMessageSchemaIdentifier.cpp
	xplcore/XplMessageSchemaIdentifier.h
	xplcore/XplService.cpp
	xplcore/XplService.h
	xplcore/XplHubConnectedPeripheral.cpp
	xplcore/XplHubConnectedPeripheral.h 
)

set(XPL_RULES_SOURCES
	xplrules/IRule.h
	xplrules/ICommandRule.h
	xplrules/IProtocol.h
	xplrules/Protocol.h
	xplrules/IDeviceManager.h
	xplrules/DeviceIdentifier.h
	xplrules/DeviceIdentifier.cpp
	xplrules/RuleInstanceManager.hpp
	xplrules/RulerFactory.h
	xplrules/RulerFactory.cpp
	xplrules/standard/DeviceManager.h
	xplrules/standard/DeviceManager.cpp
	xplrules/standard/X10Basic.h
	xplrules/standard/X10Basic.cpp
	xplrules/standard/SensorBasic.h
	xplrules/standard/SensorBasic.cpp
	xplrules/rfxLanXpl/DeviceManager.h
	xplrules/rfxLanXpl/DeviceManager.cpp
	xplrules/rfxLanXpl/AcBasic.h
	xplrules/rfxLanXpl/AcBasic.cpp
	xplrules/rfxLanXpl/SensorBasic.h
	xplrules/rfxLanXpl/SensorBasic.cpp
	xplrules/rfxLanXpl/X10Basic.h
	xplrules/rfxLanXpl/X10Basic.cpp
)


set(XPL_PLUGIN_SOURCES
	stdafx.h
	stdafx.cpp
	Xpl.h
	Xpl.cpp 
	XplConfiguration.h
	XplConfiguration.cpp 
	${XPL_CORE_SOURCES}
	${XPL_RULES_SOURCES}
)


source_group(xplcore xplcore/*.*)
source_group(xplrules xplrules/*.*)
source_group(xplrules\\standard xplrules/standard/*.*)
source_group(xplrules\\rfxLanXpl xplrules/rfxLanXpl/*.*)


#########################################
## Place here all additional include directories
## (by default yadoms-shared and boost are included)
#########################################
set(XPL_PLUGIN_INCLDIR
)

#########################################
## Place here all additional libraries to link with
## (by default yadoms-shared and boost are included)
#########################################
set(XPL_PLUGIN_LINK
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
PLUGIN_SOURCES(xpl ${XPL_PLUGIN_SOURCES})
PLUGIN_INCLDIR(xpl ${XPL_PLUGIN_ADDITIONAL_INC_DIR})
PLUGIN_LINK(xpl ${XPL_PLUGIN_ADDITIONAL_LIB})

#########################################
## Post-build commands
## Copy files to build directory
## "package.json" and "icon.png" are needed, but you can add extra-files
#########################################
PLUGIN_POST_BUILD_COPY_FILE(xpl package.json)
PLUGIN_POST_BUILD_COPY_FILE(xpl icon.png)
PLUGIN_POST_BUILD_COPY_FILE(xpl locales/fr.json)
