
# include common plugin definitions
include("../YadomsPluginsCommon.cmake")

#########################################
## Place here all the plugin source files
#########################################

set(WUNDERGROUND_SOURCES
   IWUConfiguration.h
   Wunderground.h
   Wunderground.cpp
   ForecastReceiver.cpp
   ForecastReceiver.h
   WUConfiguration.cpp
   WUConfiguration.h
   stdafx.h
   stdafx.cpp
   Keywords/IKeyword.h
   Keywords/Barometric.h
   Keywords/Barometric.cpp
   Keywords/Humidity.h
   Keywords/Humidity.cpp
   Keywords/Rain.h
   Keywords/Rain.cpp
   Keywords/Temp.h
   Keywords/Temp.cpp   
   Keywords/UV.h
   Keywords/UV.cpp   
   Keywords/Visibility.h
   Keywords/Visibility.cpp     
)

source_group(Keywords Keywords/*.*)

#########################################
## Place here all additional include directories
## (by default yadoms-shared and boost are included)
#########################################

#########################################
## Place here all additional libraries to link with
## (by default yadoms-shared and boost are included)
#########################################

#########################################
## Always leave this three lines to configure plugin correctly
## even if RFXCOM_ADDITIONAL_INC_DIR or RFXCOM_ADDITIONAL_LIB are empty
#########################################
PLUGIN_SOURCES(Wunderground ${WUNDERGROUND_SOURCES})
PLUGIN_INCLDIR(Wunderground ${WUNDERGROUND_ADDITIONAL_INC_DIR})
PLUGIN_LINK(Wunderground ${WUNDERGROUND_ADDITIONAL_LIB})

#########################################
## Post-build commands
## Copy files to build directory
## "package.json" and "icon.png" are needed, but you can add extra-files
#########################################
PLUGIN_POST_BUILD_COPY_FILE(Wunderground package.json)
PLUGIN_POST_BUILD_COPY_FILE(Wunderground icon.png)
PLUGIN_POST_BUILD_COPY_FILE(Wunderground locales/fr.json)
