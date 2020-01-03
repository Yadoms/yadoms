# include common plugin definitions

include("../YadomsPluginsCommon.cmake")

#########################################
## Place here all the plugin source files
#########################################
set(CARTELECTRONICIPBLINDVELUX_PLUGIN_SOURCES
   IOManager.cpp
   IOManager.h
   stdafx.cpp
   stdafx.h
   urlManager.cpp
   urlManager.h
   main.cpp
   main.h
   Factory.cpp
   Factory.h   
   equipments/IdeviceConfiguration.h
   equipments/IEquipment.h
   equipments/Equipment.cpp
   equipments/Equipment.h
   equipments/manuallyDeviceCreationException.hpp
   equipments/deviceConfiguration.cpp
   equipments/deviceConfiguration.h	
   equipments/specificHistorizers/deviceStatus.cpp
   equipments/specificHistorizers/deviceStatus.h
   equipments/specificHistorizers/VeluxCurtain.cpp
   equipments/specificHistorizers/VeluxCurtain.h
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
set(CARTELECTRONICIPBLINDVELUX_PLUGIN_INCLDIR
)

#########################################
## Place here all additional libraries to link with
## (by default yadoms-shared and boost are included)
#########################################
set(CARTELECTRONICIPBLINDVELUX_PLUGIN_LINK
)

#########################################
## Place here all additional compiler flags
## i.e. : for raspberry to link with wiringPi : use add_definition("-lwiringPi")
#########################################

#########################################
## Always leave these lines to configure plugin correctly
## even if CARTELECTRONICIPBLINDVELUX_PLUGIN_INCLDIR or CARTELECTRONICIPBLINDVELUX_PLUGIN_LINK are empty
#########################################
PLUGIN_SOURCES(CartelectronicIpBlindVelux ${CARTELECTRONICIPBLINDVELUX_PLUGIN_SOURCES})
PLUGIN_INCLDIR(CartelectronicIpBlindVelux ${CARTELECTRONICIPBLINDVELUX_PLUGIN_INCLDIR})
PLUGIN_LINK   (CartelectronicIpBlindVelux ${CARTELECTRONICIPBLINDVELUX_PLUGIN_LINK})