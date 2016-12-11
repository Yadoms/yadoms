
# include common plugin definitions
include("../YadomsPluginsCommon.cmake")

#########################################
## Place here all the plugin source files
#########################################

set(WUNDERGROUND_SOURCES
   Astronomy.cpp
   Astronomy.h
   deviceConfiguration.cpp
   deviceConfiguration.h
   ErrorAnswerHandler.cpp
   ErrorAnswerHandler.h
   ForecastDays.cpp
   ForecastDays.h
   IdeviceConfiguration.h
   IWUConfiguration.h
   LiveStations.cpp
   LiveStations.h
   NoStateException.hpp
   RequestErrorException.hpp
   stdafx.h
   stdafx.cpp
   WeatherUnderground.h
   WeatherUnderground.cpp
   WeatherUndergroundHelpers.h
   WeatherUndergroundHelpers.cpp
   WeatherConditions.cpp
   WeatherConditions.h
   WUConfiguration.cpp
   WUConfiguration.h
   Keywords/Condition.h
   Keywords/Condition.cpp
   Keywords/Forecast.h
   Keywords/Forecast.cpp   
   Keywords/IKeyword.h 
   Keywords/KeywordException.hpp
   Keywords/KeywordHelpers.cpp
   Keywords/KeywordHelpers.h
   Keywords/Moon.h
   Keywords/Moon.cpp
   Keywords/WeatherIcon.h
   Keywords/WeatherIcon.cpp
   specificHistorizers/ConditionHistorizer.h
   specificHistorizers/ConditionHistorizer.cpp
   specificHistorizers/ConditionFormatter.h
   specificHistorizers/ConditionFormatter.cpp   
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
   specificHistorizers/NoPeriodException.hpp
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
PLUGIN_POST_BUILD_COPY_DIRECTORY(WeatherUnderground locales)
