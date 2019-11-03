# include common plugin definitions
include("../YadomsPluginsCommon.cmake")

#########################################
## Place here all the plugin source files
#########################################
set(LAMETRIC_PLUGIN_SOURCES
	stdafx.h
	stdafx.cpp
	LametricNotificationSender.cpp
	LametricNotificationSender.h
	LametricDeviceState.cpp
	LametricDeviceState.h
	ILametricDeviceState.h
	UrlManagerHelper.cpp
	UrlManagerHelper.h
	NotificationProperties.h
	Lametric.h
	Lametric.cpp 
	LametricConfiguration.h
	LametricConfiguration.cpp 
)

#########################################
## Place here all additional groups (solution folder)
#########################################
source_group(sources ./*.*)

#########################################
## Place here all additional include directories
## (by default yadoms-shared and boost are included)
#########################################
set(LAMETRIC_PLUGIN_INCLDIR
)

#########################################
## Place here all additional libraries to link with
## (by default yadoms-shared and boost are included)
#########################################
set(LAMETRIC_PLUGIN_LINK
)

#########################################
## Place here all additional compiler flags
## i.e. : for raspberry to link with wiringPi : use add_definition("-lwiringPi")
#########################################
##add_definitions("-lwiringPi")

#########################################
## Always leave these lines to configure plugin correctly
## even if LAMETRIC_PLUGIN_INCLDIR or LAMETRIC_PLUGIN_LINK are LAMETRIC
#########################################
PLUGIN_SOURCES(Lametric ${LAMETRIC_PLUGIN_SOURCES})
PLUGIN_INCLDIR(Lametric ${LAMETRIC_PLUGIN_ADDITIONAL_INC_DIR})
PLUGIN_LINK(Lametric ${LAMETRIC_PLUGIN_ADDITIONAL_LIB})

