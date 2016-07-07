
# include common plugin definitions
include("../YadomsPluginsCommon.cmake")

#########################################
## Place here all the plugin source files
#########################################
set(ENOCEAN_SOURCES
   stdafx.h
   stdafx.cpp
   IEnOceanConfiguration.h
   EnOcean.h
   EnOcean.cpp
   EnOceanReceiveBufferHandler.h
   EnOceanReceiveBufferHandler.cpp
   EnOceanConfiguration.h
   EnOceanConfiguration.cpp
   EnOceanFactory.h
   EnOceanFactory.cpp

   message/Crc8.h
   message/Crc8.cpp
   message/ReceivedMessage.h
   message/ReceivedMessage.cpp
   message/SendMessage.h
   message/SendMessage.cpp
   message/types.h
   )
   
   
source_group(message message/*.*)

#########################################
## Place here all additional include directories
## (by default yadoms-shared and boost are included)
#########################################
set(ENOCEAN_ADDITIONAL_INC_DIR
)

#########################################
## Place here all additional libraries to link with
## (by default yadoms-shared and boost are included)
#########################################
set(ENOCEAN_ADDITIONAL_LIB
)

#########################################
## Always leave this three lines to configure plugin correctly
## even if ENOCEAN_ADDITIONAL_INC_DIR or ENOCEAN_ADDITIONAL_LIB are empty
#########################################
PLUGIN_SOURCES(enOcean ${ENOCEAN_SOURCES})
PLUGIN_INCLDIR(enOcean ${ENOCEAN_ADDITIONAL_INC_DIR})
PLUGIN_LINK(enOcean ${ENOCEAN_ADDITIONAL_LIB})

#########################################
## Post-build commands
## Copy files to build directory
## "package.json" and "icon.png" are needed, but you can add extra-files
#########################################
PLUGIN_POST_BUILD_COPY_FILE(enOcean package.json)
PLUGIN_POST_BUILD_COPY_FILE(enOcean icon.png)
PLUGIN_POST_BUILD_COPY_DIRECTORY(enOcean locales)
