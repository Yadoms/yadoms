# include common plugin definitions
include("../YadomsPluginsCommon.cmake")

#########################################
## Place here all the plugin source files
#########################################
set(SomfySituo_PLUGIN_SOURCES
	stdafx.h
	stdafx.cpp
	ISomfySituoConfiguration.h
	SomfySituo.h
	SomfySituo.cpp
	SomfySituoConfiguration.h
	SomfySituoConfiguration.cpp
	SomfySituoFactory.h
	SomfySituoFactory.cpp
	SomfySituoReceiveBufferHandler.h
	SomfySituoReceiveBufferHandler.cpp
   ProtocolException.hpp
)

#########################################
## Place here all additional groups (solution folder)
#########################################
source_group(sources ./*.*)

#########################################
## Place here all additional include directories
## (by default yadoms-shared and boost are included)
#########################################
set(SomfySituo_PLUGIN_INCLDIR
)

#########################################
## Place here all additional libraries to link with
## (by default yadoms-shared and boost are included)
#########################################
set(SomfySituo_PLUGIN_LINK
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
PLUGIN_SOURCES(SomfySituo ${SomfySituo_PLUGIN_SOURCES})
PLUGIN_INCLDIR(SomfySituo ${SomfySituo_PLUGIN_ADDITIONAL_INC_DIR})
PLUGIN_LINK(SomfySituo ${SomfySituo_PLUGIN_ADDITIONAL_LIB})

#########################################
## Post-build commands
## Copy files to build directory
## "package.json" and "icon.png" are needed, but you can add extra-files
#########################################
PLUGIN_POST_BUILD_COPY_FILE(SomfySituo package.json)
PLUGIN_POST_BUILD_COPY_FILE(SomfySituo changelog.md)
PLUGIN_POST_BUILD_COPY_FILE(SomfySituo icon.png)
PLUGIN_POST_BUILD_COPY_DIRECTORY(SomfySituo locales)
