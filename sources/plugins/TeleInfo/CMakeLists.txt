
# include common plugin definitions
include("../YadomsPluginsCommon.cmake")

#########################################
## Place here all the plugin source files
#########################################

set(TELEINFO_SOURCES
   Decoder.h
   Decoder.cpp
   IDecoder.h
   ITeleInfoConfiguration.h
   ITeleInfoMessage.h
   stdafx.h
   stdafx.cpp
   TeleInfo.h
   TeleInfo.cpp
   TeleInfoConfiguration.h
   TeleInfoConfiguration.cpp
   TeleInfoFactory.h
   TeleInfoFactory.cpp
   TeleInfoReceiveBufferHandler.h
   TeleInfoReceiveBufferHandler.cpp
   Keywords/ForecastTomorrow.cpp
   Keywords/ForecastTomorrow.h
   Keywords/IKeyword.h
   Keywords/KeywordException.hpp
   Keywords/RunningPeriod.cpp
   Keywords/RunningPeriod.h
   specificHistorizer/Color.h
   specificHistorizer/Color.cpp   
   specificHistorizer/Period.h
   specificHistorizer/Period.cpp
)

source_group(Keywords Keywords/*.*)
source_group(specificHistorizer specificHistorizer/*.*)

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
## even if TELEINFO_ADDITIONAL_INC_DIR or TELEINFO_ADDITIONAL_LIB are empty
#########################################
PLUGIN_SOURCES(teleinfo ${TELEINFO_SOURCES})
PLUGIN_INCLDIR(teleinfo ${TELEINFO_ADDITIONAL_INC_DIR})
PLUGIN_LINK(teleinfo ${TELEINFO_ADDITIONAL_LIB})

#########################################
## Post-build commands
## Copy files to build directory
## "package.json" and "icon.png" are needed, but you can add extra-files
#########################################
PLUGIN_POST_BUILD_COPY_FILE(teleinfo package.json)
PLUGIN_POST_BUILD_COPY_FILE(teleinfo icon.png)
PLUGIN_POST_BUILD_COPY_DIRECTORY(teleinfo locales)
