# include common plugin definitions
include("../YadomsPluginsCommon.cmake")

#########################################
## Place here all the plugin source files
#########################################
set(PHILIPS_HUE_PLUGIN_SOURCES
	stdafx.h
	stdafx.cpp
	PhilipsHue.h
	PhilipsHue.cpp 
	Configuration.h
	Configuration.cpp 
)

#########################################
## Place here all additional groups (solution folder)
#########################################
source_group(sources ./*.*)

#########################################
## Place here all additional include directories
## (by default yadoms-shared and boost are included)
#########################################
set(PHILIPS_HUE_PLUGIN_INCLDIR
)

#########################################
## Place here all additional libraries to link with
## (by default yadoms-shared and boost are included)
#########################################
set(PHILIPS_HUE_PLUGIN_LINK
)

#########################################
## Place here all additional compiler flags
## i.e. : for raspberry to link with wiringPi : use add_definition("-lwiringPi")
#########################################
##add_definitions("-lwiringPi")

#########################################
## Always leave these lines to configure plugin correctly
## even if EMPTY_PLUGIN_INCLDIR or EMPTY_PLUGIN_LINK are empty
#########################################
PLUGIN_SOURCES(PhilipsHue ${PHILIPS_HUE_PLUGIN_SOURCES})
PLUGIN_INCLDIR(PhilipsHue ${PHILIPS_HUE_PLUGIN_ADDITIONAL_INC_DIR})
PLUGIN_LINK(PhilipsHue ${PHILIPS_HUE_PLUGIN_ADDITIONAL_LIB})

