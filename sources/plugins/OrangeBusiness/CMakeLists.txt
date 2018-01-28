# include common plugin definitions

include("../YadomsPluginsCommon.cmake")

#########################################
## Place here all the plugin source files
#########################################
set(ORANGE_BUSINESS_PLUGIN_SOURCES
   Decoder.cpp
   Decoder.h
   deviceManager.cpp
   deviceManager.h
   DefaultEquipment.cpp
   DefaultEquipment.h
   IDecoder.h
   IEquipment.h
   IOrangeBusinessConfiguration.h
   IurlManager.h
   OrangeBusiness.cpp
   OrangeBusiness.h
   OrangeBusinessConfiguration.cpp
   OrangeBusinessConfiguration.h
   stdafx.cpp
   stdafx.h
   UnauthorizedException.hpp
   urlManager.cpp
   urlManager.h
   specificHistorizer/SNR.cpp
   specificHistorizer/SNR.h
)

#########################################
## Place here all additional groups (solution folder)
#########################################
source_group(sources ./*.*)
source_group(sources\\specificHistorizer specificHistorizer/*.*)
	
#########################################
## Place here all additional include directories
## (by default yadoms-shared and boost are included)
#########################################
set(ORANGE_BUSINESS_PLUGIN_INCLDIR
)

#########################################
## Place here all additional libraries to link with
## (by default yadoms-shared and boost are included)
#########################################
set(ORANGE_BUSINESS_PLUGIN_LINK
)

#########################################
## Place here all additional compiler flags
## i.e. : for raspberry to link with wiringPi : use add_definition("-lwiringPi")
#########################################

#########################################
## Always leave these lines to configure plugin correctly
## even if FAKE_PLUGIN_INCLDIR or FAKE_PLUGIN_LINK are empty
#########################################

PLUGIN_SOURCES(OrangeBusiness ${ORANGE_BUSINESS_PLUGIN_SOURCES})
PLUGIN_INCLDIR(OrangeBusiness ${ORANGE_BUSINESS_INCLDIR})
PLUGIN_LINK   (OrangeBusiness ${ORANGE_BUSINESS_ADDITIONAL_LIB})

#########################################
## Post-build commands
## Copy files to build directory
## "package.json" and "icon.png" are needed, but you can add extra-files
#########################################
PLUGIN_POST_BUILD_COPY_FILE(OrangeBusiness package.json)
PLUGIN_POST_BUILD_COPY_FILE(OrangeBusiness icon.png)
PLUGIN_POST_BUILD_COPY_DIRECTORY(OrangeBusiness locales)