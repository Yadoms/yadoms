# include common plugin definitions
include("../YadomsPluginsCommon.cmake")

#########################################
## Place here all the plugin source files
#########################################
set(OLED_ESPEASY_SOURCES
	stdafx.h
	stdafx.cpp
	OledEspEasy.h
	OledEspEasy.cpp 
	OledEspEasyConfiguration.h
	OledEspEasyConfiguration.cpp 
	OledEspEasyController.h
	OledEspEasyController.cpp 
	OledScreenDevice.h
	OledScreenDevice.cpp
	OledScreenLine.h
	OledScreenLine.cpp
)

#########################################
## Place here all additional groups (solution folder)
#########################################
source_group(sources ./*.*)

#########################################
## Place here all additional include directories
## (by default yadoms-shared and boost are included)
#########################################
set(OLED_ESPEASY_INCLDIR
)

#########################################
## Place here all additional libraries to link with
## (by default yadoms-shared and boost are included)
#########################################
set(OLED_ESPEASY_LINK
)

#########################################
## Place here all additional compiler flags
## i.e. : for raspberry to link with wiringPi : use add_definition("-lwiringPi")
#########################################
##add_definitions("-lwiringPi")

#########################################
## Always leave these lines to configure plugin correctly
## even if OLED_ESPEASY_INCLDIR or OLED_ESPEASY_LINK are empty
#########################################
PLUGIN_SOURCES(oledEspEasy ${OLED_ESPEASY_SOURCES})
PLUGIN_INCLDIR(oledEspEasy ${OLED_ESPEASY_ADDITIONAL_INC_DIR})
PLUGIN_LINK(oledEspEasy ${OLED_ESPEASY_ADDITIONAL_LIB})

