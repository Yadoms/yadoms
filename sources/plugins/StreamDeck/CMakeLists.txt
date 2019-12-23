# include common plugin definitions
include("../YadomsPluginsCommon.cmake")


#########################################
## Place here all the plugin source files
#########################################
set(STREAMDECK_PLUGIN_SOURCES
	stdafx.h
	stdafx.cpp
	StreamDeck.h
	StreamDeck.cpp 
	Configuration.h
	Configuration.cpp
	${libusbppsrc}
	${libusbppheader}
)

get_target_property(LIBUSBPP_SOURCE_DIR objlibusbpp SOURCE_DIR)

set(LIBUSBPP_HEADER_DIR ${LIBUSBPP_SOURCE_DIR}/../headers/libusbpp)

IF(MSVC OR XCODE)
   SET_PROPERTY(TARGET objlibusbpp PROPERTY FOLDER "plugins/external-libs")
   SET_PROPERTY(TARGET sharedlibusbpp PROPERTY FOLDER "plugins/external-libs")
   SET_PROPERTY(TARGET libusbpp-pic-example PROPERTY FOLDER "plugins/external-libs")
   SET_PROPERTY(TARGET staticlibusbpp PROPERTY FOLDER "plugins/external-libs")
   SET_PROPERTY(TARGET uninstall PROPERTY FOLDER "plugins/external-libs")
ENDIF()
#########################################
## Place here all additional groups (solution folder)
#########################################
source_group(sources ./*.*)

message(STATUS "HELLO ${LIBUSBPP_INCLUDE_DIR}")
#########################################
## Place here all additional include directories
## (by default yadoms-shared and boost are included)
#########################################
set(STREAMDECK_PLUGIN_INCLDIR

	${LIBUSBPP_SOURCE_DIR}
	${LIBUSBPP_HEADER_DIR}
)

#########################################
## Place here all additional libraries to link with
## (by default yadoms-shared and boost are included)
#########################################
set(STREAMDECK_PLUGIN_LINK
	objlibusbpp
	sharedlibusbpp
	staticlibusbpp
)

#########################################
## Place here all additional compiler flags
## i.e. : for raspberry to link with wiringPi : use add_definition("-lwiringPi")
#########################################
##add_definitions("-lwiringPi")

#########################################
## Always leave these lines to configure plugin correctly
## even if STREAMDECK_PLUGIN_INCLDIR or STREAMDECK_PLUGIN_LINK are empty
#########################################
PLUGIN_SOURCES(StreamDeck ${STREAMDECK_PLUGIN_SOURCES})
PLUGIN_INCLDIR(StreamDeck ${STREAMDECK_PLUGIN_INCLDIR})
PLUGIN_LINK(StreamDeck ${STREAMDECK_PLUGIN_LINK})

