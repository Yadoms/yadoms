# include common plugin definitions
include("../YadomsPluginsCommon.cmake")

#########################################
## Place here all the plugin source files
#########################################
set(LAMETRICTIME_PLUGIN_SOURCES
	stdafx.h
	stdafx.cpp
	LametricTime.cpp
	LametricTime.h
	Configuration.cpp
	Configuration.h
	NotificationSender.cpp 
	NotificationSender.h
	INotificationSender.h
	DeviceState.cpp
	DeviceState.h
	IDeviceState.h
	CFactory.cpp
	CFactory.h
	UrlManagerHelper.cpp
	UrlManagerHelper.h
	NotificationProperties.h
)

#########################################
## Place here all additional groups (solution folder)
#########################################
source_group(sources ./*.*)

#########################################
## Place here all additional include directories
## (by default yadoms-shared and boost are included)
#########################################
set(LAMETRICTIME_PLUGIN_INCLDIR
)

#########################################
## Place here all additional libraries to link with
## (by default yadoms-shared and boost are included)
#########################################
set(LAMETRICTIME_PLUGIN_LINK
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
PLUGIN_SOURCES(LametricTime ${LAMETRICTIME_PLUGIN_SOURCES})
PLUGIN_INCLDIR(LametricTime ${LAMETRICTIME_PLUGIN_ADDITIONAL_INC_DIR})
PLUGIN_LINK(LametricTime ${LAMETRICTIME_PLUGIN_ADDITIONAL_LIB})

