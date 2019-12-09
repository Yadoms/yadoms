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
   equipments/Helpers.cpp
   equipments/Helpers.h   
   equipments/WESEquipment.cpp
   equipments/WESEquipment.h
   equipments/manuallyDeviceCreationException.hpp
   equipments/masterDeviceConfiguration.cpp
   equipments/masterDeviceConfiguration.h	
   equipments/noInformationException.hpp
   equipments/tooLowRevisionException.hpp
   equipments/TIC.cpp
   equipments/TIC.h
   equipments/specificHistorizers/Color.cpp
   equipments/specificHistorizers/Color.h
   equipments/specificHistorizers/deviceStatus.cpp
   equipments/specificHistorizers/deviceStatus.h
   equipments/specificHistorizers/TeleInfoStatus.cpp
   equipments/specificHistorizers/TeleInfoStatus.h
   equipments/specificHistorizers/Periods/Base.cpp
   equipments/specificHistorizers/Periods/Base.h
   equipments/specificHistorizers/Periods/BASEA5.cpp
   equipments/specificHistorizers/Periods/BASEA5.h
   equipments/specificHistorizers/Periods/BT4SUP36.cpp
   equipments/specificHistorizers/Periods/BT4SUP36.h
   equipments/specificHistorizers/Periods/BT5SUP36.cpp
   equipments/specificHistorizers/Periods/BT5SUP36.h
   equipments/specificHistorizers/Periods/Ejp.cpp
   equipments/specificHistorizers/Periods/Ejp.h
   equipments/specificHistorizers/Periods/HCWE.cpp
   equipments/specificHistorizers/Periods/HCWE.h
   equipments/specificHistorizers/Periods/HCWeekDayOfWeek.cpp
   equipments/specificHistorizers/Periods/HCWeekDayOfWeek.h
   equipments/specificHistorizers/Periods/HpHc.cpp
   equipments/specificHistorizers/Periods/HpHc.h
   equipments/specificHistorizers/Periods/HTA5.cpp
   equipments/specificHistorizers/Periods/HTA5.h
   equipments/specificHistorizers/Periods/Tempo.cpp
   equipments/specificHistorizers/Periods/Tempo.h
   equipments/specificHistorizers/Periods/TJEJP.cpp
   equipments/specificHistorizers/Periods/TJEJP.h
   equipments/specificHistorizers/Periods/TeleInfoCapacities.cpp
   equipments/specificHistorizers/Periods/TeleInfoCapacities.h
   equipments/WESSubEquipments/Analog.cpp
   equipments/WESSubEquipments/Analog.h
   equipments/WESSubEquipments/Clamp.cpp
   equipments/WESSubEquipments/Clamp.h
   equipments/WESSubEquipments/Pulse.cpp
   equipments/WESSubEquipments/Pulse.h
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
source_group(sources\\equipments\\specificHistorizers\\Periods equipments/specificHistorizers/Periods/*.*)
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