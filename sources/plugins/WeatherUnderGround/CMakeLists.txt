
# include common plugin definitions
include("../YadomsPluginsCommon.cmake")

#########################################
## Place here all the plugin source files
#########################################

set(WUNDERGROUND_SOURCES
   ErrorAnswerHandler.cpp
   ErrorAnswerHandler.h
   IWUConfiguration.h
   noLocationException.hpp
   noStateException.hpp
   RequestErrorException.hpp
   stdafx.h
   stdafx.cpp
   WeatherUnderground.h
   WeatherUnderground.cpp
   webSiteErrorException.hpp
   WUConfiguration.cpp
   WUConfiguration.h
   WUFactory.cpp
   WUFactory.h
   Features/Astronomy.cpp
   Features/Astronomy.h
   Features/ForecastDays.cpp
   Features/ForecastDays.h   
   Features/IFeature.h
   Features/LiveStations.cpp
   Features/LiveStations.h
   Features/Location.cpp
   Features/Location.h   
   Features/WeatherConditions.cpp
   Features/WeatherConditions.h
   Features/Keywords/Condition.h
   Features/Keywords/Condition.cpp
   Features/Keywords/Forecast.h
   Features/Keywords/Forecast.cpp   
   Features/Keywords/IKeyword.h 
   Features/Keywords/KeywordException.hpp
   Features/Keywords/KeywordHelpers.cpp
   Features/Keywords/KeywordHelpers.h
   Features/Keywords/Moon.h
   Features/Keywords/Moon.cpp
   Features/Keywords/WeatherIcon.h
   Features/Keywords/WeatherIcon.cpp
   Features/Keywords/specificHistorizers/ConditionHistorizer.h
   Features/Keywords/specificHistorizers/ConditionHistorizer.cpp
   Features/Keywords/specificHistorizers/ConditionFormatter.h
   Features/Keywords/specificHistorizers/ConditionFormatter.cpp   
   Features/Keywords/specificHistorizers/ForecastHelpers.h
   Features/Keywords/specificHistorizers/ForecastHelpers.cpp
   Features/Keywords/specificHistorizers/ForecastHistorizer.h
   Features/Keywords/specificHistorizers/ForecastHistorizer.cpp
   Features/Keywords/specificHistorizers/ForecastFormatter.h
   Features/Keywords/specificHistorizers/ForecastFormatter.cpp
   Features/Keywords/specificHistorizers/MoonFormatter.h
   Features/Keywords/specificHistorizers/MoonFormatter.cpp
   Features/Keywords/specificHistorizers/MoonHistorizer.h
   Features/Keywords/specificHistorizers/MoonHistorizer.cpp
   Features/Keywords/specificHistorizers/NoPeriodException.hpp
)

source_group(Features Features/*.*)
source_group(Features\\Keywords Features/Keywords/*.*)
source_group(Features\\Keywords\\specificHistorizers Features/Keywords/specificHistorizers/*.*)

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
