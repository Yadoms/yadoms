# include common plugin definitions

include("../YadomsPluginsCommon.cmake")

#########################################
## Place here all the plugin source files
#########################################
set(IPX800_PLUGIN_SOURCES
	IOManager.cpp
	IOManager.h
	IIPX800Configuration.h
   IPX800.cpp
   IPX800.h
	IPX800Configuration.cpp
	IPX800Configuration.h
	IPX800Factory.cpp
	IPX800Factory.h
	stdafx.cpp
	stdafx.h
	urlManager.cpp
	urlManager.h
	equipments/IEquipment.h
	equipments/IPX800Equipment.cpp
	equipments/IPX800Equipment.h	
   equipments/manuallyDeviceCreationException.hpp	
   equipments/noInformationException.hpp
	equipments/X24DExtension.cpp
	equipments/X24DExtension.h	
	equipments/X8DExtension.cpp
	equipments/X8DExtension.h	
	equipments/X8RExtension.cpp
	equipments/X8RExtension.h
	http/HttpMethods.cpp
	http/HttpMethods.h
   http/failedSendingException.hpp
   http/invalidHTTPResultException.hpp
	specificHistorizers/Analog.cpp
	specificHistorizers/Analog.h
	specificHistorizers/counter.cpp
	specificHistorizers/counter.h	
	specificHistorizers/inputOutput.cpp
	specificHistorizers/inputOutput.h
)

   
#########################################
## Place here all additional groups (solution folder)
#########################################
source_group(sources ./*.*)
source_group(sources\\equipments equipments/*.*)
source_group(sources\\http http/*.*)
source_group(sources\\specificHistorizers specificHistorizers/*.*)
	
#########################################
## Place here all additional include directories
## (by default yadoms-shared and boost are included)
#########################################
set(IPX800_PLUGIN_INCLDIR
)

#########################################
## Place here all additional libraries to link with
## (by default yadoms-shared and boost are included)
#########################################
set(IPX800_PLUGIN_LINK
)

#########################################
## Place here all additional compiler flags
## i.e. : for raspberry to link with wiringPi : use add_definition("-lwiringPi")
#########################################

#########################################
## Always leave these lines to configure plugin correctly
## even if FAKE_PLUGIN_INCLDIR or FAKE_PLUGIN_LINK are empty
#########################################
PLUGIN_SOURCES(IPX800 ${IPX800_PLUGIN_SOURCES})
PLUGIN_INCLDIR(IPX800 ${IPX800_PLUGIN_INCLDIR})
PLUGIN_LINK   (IPX800 ${IPX800_PLUGIN_ADDITIONAL_LIB})
