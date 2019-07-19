# include common plugin definitions
include("../YadomsPluginsCommon.cmake")

#########################################
## Place here all the plugin source files
#########################################
set(WEATHER_PLUGIN_SOURCES
   stdafx.h
   stdafx.cpp
   IWeatherService.h
   LiveWeatherDevice.h
   LiveWeatherDevice.cpp
   OpenWeatherService.h
   OpenWeatherService.cpp
   Weather.h
   Weather.cpp 
   WeatherConfiguration.h
   WeatherConfiguration.cpp 
   WeatherLocation.h
   WeatherLocation.cpp 
)

#########################################
## Place here all additional groups (solution folder)
#########################################
source_group(sources ./*.*)

#########################################
## Place here all additional include directories
## (by default yadoms-shared and boost are included)
#########################################
set(WEATHER_PLUGIN_INCLDIR
)

#########################################
## Place here all additional libraries to link with
## (by default yadoms-shared and boost are included)
#########################################
set(WEATHER_PLUGIN_LINK
)

#########################################
## Always leave these lines to configure plugin correctly
## even if WEATHER_PLUGIN_INCLDIR or WEATHER_PLUGIN_LINK are empty
#########################################
PLUGIN_SOURCES(Weather ${WEATHER_PLUGIN_SOURCES})
PLUGIN_INCLDIR(Weather ${WEATHER_PLUGIN_ADDITIONAL_INC_DIR})
PLUGIN_LINK(Weather ${WEATHER_PLUGIN_ADDITIONAL_LIB})
