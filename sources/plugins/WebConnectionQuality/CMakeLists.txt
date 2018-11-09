# include common plugin definitions
include("../YadomsPluginsCommon.cmake")

#########################################
## Place here all the plugin source files
#########################################
set(WEBCONNECTIONQUALITY_PLUGIN_SOURCES
	stdafx.h
	stdafx.cpp
   SpeedTestEventData.h
   SpeedTestEventData.cpp
   SpeedTestProcessLogger.h
   SpeedTestProcessLogger.cpp
   SpeedTestProcessObserver.h
   SpeedTestProcessObserver.cpp
	WebConnectionQuality.h
	WebConnectionQuality.cpp 
	WebConnectionQualityConfiguration.h
	WebConnectionQualityConfiguration.cpp 
   
   specificHistorizers/NetworkBandwithHistorizer.h
   specificHistorizers/NetworkBandwithHistorizer.cpp
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
PLUGIN_SOURCES(WebConnectionQuality ${WEBCONNECTIONQUALITY_PLUGIN_SOURCES})
PLUGIN_INCLDIR(WebConnectionQuality ${WEBCONNECTIONQUALITY_PLUGIN_ADDITIONAL_INC_DIR})
PLUGIN_LINK(WebConnectionQuality ${WEBCONNECTIONQUALITY_PLUGIN_ADDITIONAL_LIB})

#########################################
## post-build commands
## copy files to build directory
## Use PLUGIN_POST_BUILD_COPY_FILE and PLUGIN_POST_BUILD_COPY_DIRECTORY macros
## to copy extra-files to the binary folder
#########################################
PLUGIN_POST_BUILD_COPY_FILE(WebConnectionQuality speedtest.py)
