# include common plugin definitions

include("../YadomsPluginsCommon.cmake")

#########################################
## Place here all the plugin source files
#########################################
set(WES_PLUGIN_SOURCES
   IOManager.cpp
   IOManager.h
   IWESConfiguration.h
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
   equipments/IEquipment.h
   equipments/ImasterDeviceConfiguration.h
   equipments/WESEquipment.cpp
   equipments/WESEquipment.h
   equipments/manuallyDeviceCreationException.hpp
   equipments/masterDeviceConfiguration.cpp
   equipments/masterDeviceConfiguration.h	
   equipments/noInformationException.hpp
   equipments/tooLowRevisionException.hpp
   equipments/specificHistorizers/Analog.cpp
   equipments/specificHistorizers/Analog.h
   equipments/specificHistorizers/Color.cpp
   equipments/specificHistorizers/Color.h
   equipments/specificHistorizers/deviceStatus.cpp
   equipments/specificHistorizers/deviceStatus.h
   equipments/specificHistorizers/Period.cpp
   equipments/specificHistorizers/Period.h
   equipments/specificHistorizers/TeleInfoStatus.cpp
   equipments/specificHistorizers/TeleInfoStatus.h
   equipments/WESSubEquipments/Clamp.cpp
   equipments/WESSubEquipments/Clamp.h
   equipments/WESSubEquipments/Pulse.cpp
   equipments/WESSubEquipments/Pulse.h
   equipments/WESSubEquipments/TIC.cpp
   equipments/WESSubEquipments/TIC.h
   http/failedSendingException.hpp
   http/HttpMethods.cpp
   http/HttpMethods.h
   http/invalidHTTPResultException.hpp
   http/timeOutException.hpp
)

#########################################
## Place here all additional groups (solution folder)
#########################################
source_group(sources ./*.*)
source_group(sources\\equipments equipments/*.*)
source_group(sources\\http http/*.*)
source_group(sources\\equipments\\specificHistorizers equipments/specificHistorizers/*.*)
source_group(sources\\equipments\\WESSubEquipments equipments/WESSubEquipments/*.*)
	
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
PLUGIN_POST_BUILD_COPY_FILE(WES changelog.md)
PLUGIN_POST_BUILD_COPY_FILE(WES icon.png)
PLUGIN_POST_BUILD_COPY_DIRECTORY(WES locales)