# include common plugin definitions
include("../YadomsPluginsCommon.cmake")

#########################################
## Place here all the plugin source files
#########################################
set(HTTP_SCREEN_SOURCES
	stdafx.h
	stdafx.cpp
	Configuration.h
	Configuration.cpp 
	HttpScreen.h
	HttpScreen.cpp 
	HttpController.h
	HttpController.cpp 
	ScreenDevice.h
	ScreenDevice.cpp
	IRealScreen.h
	TftScreen.h
	TftScreen.cpp
	EInkScreen.h
	EInkScreen.cpp
	Command.h
	Command.cpp
	CommandParser.h
	CommandParser.cpp
)

#########################################
## Place here all additional groups (solution folder)
#########################################
source_group(sources ./*.*)

#########################################
## Place here all additional include directories
## (by default yadoms-shared and boost are included)
#########################################
set(HTTP_SCREEN_INCLDIR
)

#########################################
## Place here all additional libraries to link with
## (by default yadoms-shared and boost are included)
#########################################
set(HTTP_SCREEN_LINK
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
PLUGIN_SOURCES(httpScreen ${HTTP_SCREEN_SOURCES})
PLUGIN_INCLDIR(httpScreen ${HTTP_SCREEN_ADDITIONAL_INC_DIR})
PLUGIN_LINK(httpScreen ${HTTP_SCREEN_ADDITIONAL_LIB})

