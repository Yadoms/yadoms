# include common plugin definitions

include("../YadomsPluginsCommon.cmake")

#########################################
## Place here all the plugin source files
#########################################
set(VRTIP_PLUGIN_SOURCES
   IOManager.cpp
   IOManager.h
   stdafx.cpp
   stdafx.h
   urlManager.cpp
   urlManager.h
   VRTIP.cpp
   VRTIP.h
   VRTIPFactory.cpp
   VRTIPFactory.h   
   equipments/IdeviceConfiguration.h
   equipments/IEquipment.h
   equipments/VRTIPEquipment.cpp
   equipments/VRTIPEquipment.h
   equipments/manuallyDeviceCreationException.hpp
   equipments/deviceConfiguration.cpp
   equipments/deviceConfiguration.h	
   equipments/specificHistorizers/deviceStatus.cpp
   equipments/specificHistorizers/deviceStatus.h   
   http/HttpContext.cpp
   http/HttpContext.h   
   http/HttpMethods.cpp
   http/HttpMethods.h
   http/timeOutException.hpp
)

#########################################
## Place here all additional groups (solution folder)
#########################################
source_group(sources ./*.*)
source_group(sources\\equipments equipments/*.*)
source_group(sources\\http http/*.*)
	
#########################################
## Place here all additional include directories
## (by default yadoms-shared and boost are included)
#########################################
set(VRTIP_PLUGIN_INCLDIR
)

#########################################
## Place here all additional libraries to link with
## (by default yadoms-shared and boost are included)
#########################################
set(VRTIP_PLUGIN_LINK
)

#########################################
## Place here all additional compiler flags
## i.e. : for raspberry to link with wiringPi : use add_definition("-lwiringPi")
#########################################

#########################################
## Always leave these lines to configure plugin correctly
## even if VRTIP_PLUGIN_INCLDIR or VRTIP_PLUGIN_LINK are empty
#########################################
PLUGIN_SOURCES(VRTIP ${VRTIP_PLUGIN_SOURCES})
PLUGIN_INCLDIR(VRTIP ${VRTIP_PLUGIN_INCLDIR})
PLUGIN_LINK   (VRTIP ${VRTIP_PLUGIN_LINK})