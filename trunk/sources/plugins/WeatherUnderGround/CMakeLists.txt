
# include common plugin definitions
include("../YadomsPluginsCommon.cmake")

#########################################
## Place here all the plugin source files
#########################################

set(WUNDERGROUND_SOURCES
   Astronomy.cpp
   Astronomy.h
   Forecast3Days.cpp
   Forecast3Days.h
   IWUConfiguration.h
   WeatherUnderground.h
   WeatherUnderground.cpp
   WeatherConditions.cpp
   WeatherConditions.h
   WUConfiguration.cpp
   WUConfiguration.h
   stdafx.h
   stdafx.cpp
   Keywords/Barometric.h
   Keywords/Barometric.cpp
   Keywords/Duration.h
   Keywords/Duration.cpp
   Keywords/Forecast.h
   Keywords/Forecast.cpp   
   Keywords/Humidity.h
   Keywords/Humidity.cpp
   Keywords/IKeyword.h   
   Keywords/Load.h
   Keywords/Load.cpp
   Keywords/Moon.h
   Keywords/Moon.cpp
   Keywords/Rain.h
   Keywords/Rain.cpp
   Keywords/Temp.h
   Keywords/Temp.cpp   
   Keywords/UV.h
   Keywords/UV.cpp
   Keywords/Visibility.h
   Keywords/Visibility.cpp
   Keywords/WeatherIcon.h
   Keywords/WeatherIcon.cpp   
   Keywords/WindDirection.h
   Keywords/WindDirection.cpp
   Keywords/WindSpeed.h
   Keywords/WindSpeed.cpp
   specificHistorizers/ForecastHelpers.h
   specificHistorizers/ForecastHelpers.cpp
   specificHistorizers/ForecastHistorizer.h
   specificHistorizers/ForecastHistorizer.cpp
   specificHistorizers/ForecastFormatter.h
   specificHistorizers/ForecastFormatter.cpp
   specificHistorizers/MoonFormatter.h
   specificHistorizers/MoonFormatter.cpp
   specificHistorizers/MoonHistorizer.h
   specificHistorizers/MoonHistorizer.cpp
)

source_group(Keywords Keywords/*.*)
source_group(specificHistorizers specificHistorizers/*.*)

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
PLUGIN_SOURCES(WeatherUnderground ${WUNDERGROUND_SOURCES})
PLUGIN_INCLDIR(WeatherUnderground ${WUNDERGROUND_ADDITIONAL_INC_DIR})
PLUGIN_LINK(WeatherUnderground ${WUNDERGROUND_ADDITIONAL_LIB})

#########################################
## Post-build commands
## Copy files to build directory
## "package.json" and "icon.png" are needed, but you can add extra-files
#########################################
PLUGIN_POST_BUILD_COPY_FILE(WeatherUnderground package.json)
PLUGIN_POST_BUILD_COPY_FILE(WeatherUnderground icon.png)
PLUGIN_POST_BUILD_COPY_FILE(WeatherUnderground locales/fr.json)
