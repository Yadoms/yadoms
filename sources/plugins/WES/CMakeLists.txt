# include common plugin definitions

include("../YadomsPluginsCommon.cmake")

#########################################
## Place here all the plugin source files
#########################################
set(WES_PLUGIN_SOURCES
   IOManager.cpp
   IOManager.h
   IWESConfiguration.h
   stateCommonDeclaration.hpp
   WES.cpp
   WES.h
   WESConfiguration.cpp
   WESConfiguration.h
   WESFactory.cpp
   WESFactory.h
   stdafx.cpp
   stdafx.h
   urlManager.cpp
   urlManager.h
   deviceStateMachine/deviceStateMachine.cpp
   deviceStateMachine/deviceStateMachine.hpp
   deviceStateMachine/deviceStateConnected.hpp
   deviceStateMachine/deviceStateDisconnected.hpp
   deviceStateMachine/deviceStateInitialization.hpp
   deviceStateMachine/deviceStateTerminate.hpp
   deviceStateMachine/deviceStateTimeout.hpp
   deviceStateMachine/deviceStateUpdateConfiguration.hpp
   equipments/IEquipment.h
   equipments/WESEquipment.cpp
   equipments/WESEquipment.h	
   equipments/manuallyDeviceCreationException.hpp	
   equipments/noInformationException.hpp
   pluginStateMachine/pluginStateMachine.cpp
   pluginStateMachine/pluginStateMachine.hpp
   pluginStateMachine/pluginStateConnected.hpp
   pluginStateMachine/pluginStateDisconnected.hpp
   pluginStateMachine/pluginStateInitialization.hpp
   pluginStateMachine/pluginStateTerminate.hpp
   pluginStateMachine/pluginStateTimeout.hpp
   pluginStateMachine/pluginStateUpdateConfiguration.hpp
)

source_group(deviceState deviceState/*.*)
source_group(equipments equipments/*.*)
source_group(pluginState pluginState/*.*)
	
#########################################
## Place here all additional include directories
## (by default yadoms-shared and boost are included)
#########################################
set(WES_PLUGIN_INCLDIR
)

#########################################
## Place here all additional libraries to link with
## (by default yadoms-shared and boost are included)
#########################################
set(WES_PLUGIN_LINK
)

#########################################
## Place here all additional compiler flags
## i.e. : for raspberry to link with wiringPi : use add_definition("-lwiringPi")
#########################################

#########################################
## Always leave these lines to configure plugin correctly
## even if WES_PLUGIN_INCLDIR or WES_PLUGIN_LINK are empty
#########################################

PLUGIN_SOURCES(WES ${WES_PLUGIN_SOURCES})
PLUGIN_INCLDIR(WES ${WES_PLUGIN_INCLDIR})
PLUGIN_LINK   (WES ${WES_PLUGIN_ADDITIONAL_LIB})

#########################################
## Post-build commands
## Copy files to build directory
## "package.json" and "icon.png" are needed, but you can add extra-files
#########################################
PLUGIN_POST_BUILD_COPY_FILE(WES package.json)
PLUGIN_POST_BUILD_COPY_FILE(WES icon.png)
PLUGIN_POST_BUILD_COPY_DIRECTORY(WES locales)