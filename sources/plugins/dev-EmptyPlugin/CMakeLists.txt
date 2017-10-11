# include common plugin definitions
include("../YadomsPluginsCommon.cmake")

#########################################
## Place here all the plugin source files
#########################################
set(EMPTY_PLUGIN_SOURCES
	stdafx.h
	stdafx.cpp
	EmptyPlugin.h
	EmptyPlugin.cpp 
	EmptyPluginConfiguration.h
	EmptyPluginConfiguration.cpp 
)

#########################################
## Place here all additional groups (solution folder)
#########################################
source_group(sources ./*.*)

#########################################
## Place here all additional include directories
## (by default yadoms-shared and boost are included)
#########################################
set(EMPTY_PLUGIN_INCLDIR
)

#########################################
## Place here all additional libraries to link with
## (by default yadoms-shared and boost are included)
#########################################
set(EMPTY_PLUGIN_LINK
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
PLUGIN_SOURCES(dev-emptyPlugin ${EMPTY_PLUGIN_SOURCES})
PLUGIN_INCLDIR(dev-emptyPlugin ${EMPTY_PLUGIN_ADDITIONAL_INC_DIR})
PLUGIN_LINK(dev-emptyPlugin ${EMPTY_PLUGIN_ADDITIONAL_LIB})

#########################################
## Post-build commands
## Copy files to build directory
## "package.json" and "icon.png" are needed, but you can add extra-files
#########################################
PLUGIN_POST_BUILD_COPY_FILE(dev-emptyPlugin package.json)
PLUGIN_POST_BUILD_COPY_FILE(dev-emptyPlugin icon.png)
PLUGIN_POST_BUILD_COPY_DIRECTORY(dev-emptyPlugin locales)
