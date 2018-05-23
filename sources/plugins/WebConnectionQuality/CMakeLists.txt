# include common plugin definitions
include("../YadomsPluginsCommon.cmake")

#########################################
## Place here all the plugin source files
#########################################
set(WEBCONNECTIONQUALITY_PLUGIN_SOURCES
	stdafx.h
	stdafx.cpp
	WebConnectionQuality.h
	WebConnectionQuality.cpp 
	WebConnectionQualityConfiguration.h
	WebConnectionQualityConfiguration.cpp 
)

#########################################
## Place here all additional groups (solution folder)
#########################################
source_group(sources ./*.*)

#########################################
## Place here all additional include directories
## (by default yadoms-shared and boost are included)
#########################################
set(WEBCONNECTIONQUALITY_PLUGIN_INCLDIR
)

#########################################
## Place here all additional libraries to link with
## (by default yadoms-shared and boost are included)
#########################################
set(WEBCONNECTIONQUALITY_PLUGIN_LINK
)

#########################################
## Always leave these lines to configure plugin correctly
## even if WEBCONNECTIONQUALITY_PLUGIN_INCLDIR or WEBCONNECTIONQUALITY_PLUGIN_LINK are empty
#########################################
PLUGIN_SOURCES(WebConnectionQualityPlugin ${WEBCONNECTIONQUALITY_PLUGIN_SOURCES})
PLUGIN_INCLDIR(WebConnectionQualityPlugin ${WEBCONNECTIONQUALITY_PLUGIN_ADDITIONAL_INC_DIR})
PLUGIN_LINK(WebConnectionQualityPlugin ${WEBCONNECTIONQUALITY_PLUGIN_ADDITIONAL_LIB})

#########################################
## Post-build commands
## Copy files to build directory
## "package.json" and "icon.png" are needed, but you can add extra-files
#########################################
PLUGIN_POST_BUILD_COPY_FILE(WebConnectionQualityPlugin package.json)
PLUGIN_POST_BUILD_COPY_FILE(WebConnectionQualityPlugin changelog.md)
PLUGIN_POST_BUILD_COPY_FILE(WebConnectionQualityPlugin icon.png)
PLUGIN_POST_BUILD_COPY_DIRECTORY(WebConnectionQualityPlugin locales)
