# include common plugin definitions
include("../YadomsPluginsCommon.cmake")

#########################################
## Place here all the plugin source files
#########################################
set(MQTT_SOURCES
	stdafx.h
	stdafx.cpp
	Mqtt.h
	Mqtt.cpp 
	MqttConfiguration.h
	MqttConfiguration.cpp 
	MosquittoServer.h
	MosquittoServer.cpp 
)

#########################################
## Place here all additional groups (solution folder)
#########################################
source_group(sources ./*.*)

ADD_SUBDIRECTORY(server)

#########################################
## Place here all additional include directories
## (by default yadoms-shared and boost are included)
#########################################
set(MQTT_INCLDIR
   ${MOSQUITTO_INCLUDE_DIRECTORY}
)

#########################################
## Place here all additional libraries to link with
## (by default yadoms-shared and boost are included)
#########################################
set(MQTT_ADDITIONAL_LIB
   mosquitto
)

#########################################
## Place here all additional compiler flags
## i.e. : for raspberry to link with wiringPi : use add_definition("-lwiringPi")
#########################################
##add_definitions("-lwiringPi")

#########################################
## Always leave these lines to configure plugin correctly
## even if MQTT_INCLDIR or MQTT_LINK are empty
#########################################
PLUGIN_SOURCES(mqtt ${MQTT_SOURCES})
PLUGIN_INCLDIR(mqtt ${MQTT_ADDITIONAL_INC_DIR})
PLUGIN_LINK(mqtt ${MQTT_ADDITIONAL_LIB})

#########################################
## Post-build commands
## Copy files to build directory
## "package.json" and "icon.png" are needed, but you can add extra-files
#########################################
PLUGIN_POST_BUILD_COPY_FILE(mqtt package.json)
PLUGIN_POST_BUILD_COPY_FILE(mqtt changelog.md)
PLUGIN_POST_BUILD_COPY_FILE(mqtt icon.png)
PLUGIN_POST_BUILD_COPY_DIRECTORY(mqtt locales)
