# include common plugin definitions
include("../YadomsPluginsCommon.cmake")

#########################################
## Place here all the plugin source files
#########################################
set(SOMFYIOCONTROLLER_PLUGIN_SOURCES
	stdafx.h
	stdafx.cpp
	ISomfyIOControllerConfiguration.h
	SomfyIOController.h
	SomfyIOController.cpp
	SomfyIOControllerConfiguration.h
	SomfyIOControllerConfiguration.cpp
	SomfyIOControllerFactory.h
	SomfyIOControllerFactory.cpp
	SomfyIOControllerReceiveBufferHandler.h
	SomfyIOControllerReceiveBufferHandler.cpp
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
set(SOMFYIOCONTROLLER_PLUGIN_INCLDIR
)

#########################################
## Place here all additional libraries to link with
## (by default yadoms-shared and boost are included)
#########################################
set(SOMFYIOCONTROLLER_PLUGIN_LINK
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
PLUGIN_SOURCES(SomfyIOController ${SOMFYIOCONTROLLER_PLUGIN_SOURCES})
PLUGIN_INCLDIR(SomfyIOController ${SOMFYIOCONTROLLER_PLUGIN_ADDITIONAL_INC_DIR})
PLUGIN_LINK(SomfyIOController ${SOMFYIOCONTROLLER_PLUGIN_ADDITIONAL_LIB})

#########################################
## Post-build commands
## Copy files to build directory
## "package.json" and "icon.png" are needed, but you can add extra-files
#########################################
PLUGIN_POST_BUILD_COPY_FILE(SomfyIOController package.json)
PLUGIN_POST_BUILD_COPY_FILE(SomfyIOController changelog.md)
PLUGIN_POST_BUILD_COPY_FILE(SomfyIOController icon.png)
PLUGIN_POST_BUILD_COPY_DIRECTORY(SomfyIOController locales)
