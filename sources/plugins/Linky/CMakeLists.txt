
# include common plugin definitions
include("../YadomsPluginsCommon.cmake")

#########################################
## Place here all the plugin source files
#########################################

set(LINKY_SOURCES
   Decoder.h
   Decoder.cpp
   IDecoder.h
   ILinkyConfiguration.h
   ILinkyMessage.h
   Linky.h
   Linky.cpp
   LinkyConfiguration.h
   LinkyConfiguration.cpp
   LinkyFactory.h
   LinkyFactory.cpp
   LinkyReceiveBufferHandler.h
   LinkyReceiveBufferHandler.cpp
   stdafx.h
   stdafx.cpp
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
## even if LINKY_ADDITIONAL_INC_DIR or LINKY_ADDITIONAL_LIB are empty
#########################################
PLUGIN_SOURCES(Linky ${LINKY_SOURCES})
PLUGIN_INCLDIR(Linky ${LINKY_ADDITIONAL_INC_DIR})
PLUGIN_LINK(Linky ${LINKY_ADDITIONAL_LIB})

#########################################
## Post-build commands
## Copy files to build directory
## "package.json" and "icon.png" are needed, but you can add extra-files
#########################################
PLUGIN_POST_BUILD_COPY_FILE(Linky package.json)
PLUGIN_POST_BUILD_COPY_FILE(Linky icon.png)
PLUGIN_POST_BUILD_COPY_DIRECTORY(Linky locales)
