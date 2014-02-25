# include common plugin definitions
include("../YadomsPluginsCommon.cmake")

#########################################
## Place here all the plugin source files
#########################################
set(FAKE_PLUGIN_SOURCES
	stdafx.h
	stdafx.cpp
	FakePlugin.h
	FakePlugin.cpp 
	FakePluginConfiguration.h
	FakePluginConfiguration.cpp 
)

#########################################
## Place here all additional include directories
## (by default yadoms-shared and boost are included)
#########################################
set(FAKE_PLUGIN_INCLDIR
)

#########################################
## Place here all additional libraries to link with
## (by default yadoms-shared and boost are included)
#########################################
set(FAKE_PLUGIN_LINK
)

#########################################
## Place here all additional compiler flags
## i.e. : for raspberry to link with wiringPi : use add_definition("-lwiringPi")
#########################################
##add_definitions("-lwiringPi")

#########################################
## Always leave this three lines to configure plugin correctly
## even if FAKE_PLUGIN_INCLDIR or FAKE_PLUGIN_LINK are empty
#########################################
PLUGIN_SOURCES(fakePlugin ${FAKE_PLUGIN_SOURCES})
PLUGIN_INCLDIR(fakePlugin ${FAKE_PLUGIN_ADDITIONAL_INC_DIR})
PLUGIN_LINK(fakePlugin ${FAKE_PLUGIN_ADDITIONAL_LIB})
