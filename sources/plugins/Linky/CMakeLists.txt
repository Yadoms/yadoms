
# include common plugin definitions
include("../YadomsPluginsCommon.cmake")

#########################################
## Place here all the plugin source files
#########################################

set(LINKY_SOURCES
   HistoricDecoder.h
   HistoricDecoder.cpp
   IDecoder.h
   ILinkyConfiguration.h
   ILinkyMessage.h
   Linky.h
   Linky.cpp
   LinkyDecoder.h
   LinkyDecoder.cpp
   LinkyHelpers.hpp
   LinkyReceiveBufferHandler.h
   LinkyReceiveBufferHandler.cpp
   ProtocolManager.cpp
   ProtocolManager.h
   stdafx.h
   stdafx.cpp
   Keywords/KeywordException.hpp
   Keywords/RunningPeriod.cpp
   Keywords/RunningPeriod.h
   Keywords/IKeyword.h
   specificHistorizer/Color.h
   specificHistorizer/Color.cpp
   specificHistorizer/Period.h
   specificHistorizer/Period.cpp   
)

IF (WIN32)
    set(LINKY_SOURCES ${LINKY_SOURCES}
	# Add Windows sources
        Windows/FT2xxSerialPort.h
        Windows/FT2xxSerialPort.cpp
        Windows/LinkyFactory.h
        Windows/LinkyFactory.cpp
        Windows/LinkyConfiguration.h
        Windows/LinkyConfiguration.cpp
	)
	
	source_group(sources\\Windows Windows/*.*)
	
ENDIF()

IF (UNIX AND NOT APPLE)
    set(LINKY_SOURCES ${LINKY_SOURCES}
	     # Add Common Linux and RaspberryPI sources
        Linux/LinkyFactory.h
        Linux/LinkyFactory.cpp
        Linux/LinkyConfiguration.h
        Linux/LinkyConfiguration.cpp        
	)
	
	source_group(sources\\Linux Linux/*.*)
ENDIF()

#########################################
## Place here all additional groups (solution folder)
#########################################

source_group(sources ./*.*)
source_group(sources\\specificHistorizer specificHistorizer/*.*)
source_group(sources\\Keywords Keywords/*.*)

#########################################
## Place here all additional include directories
## (by default yadoms-shared and boost are included)
#########################################

IF(WIN32)
   set(TELEINFO_ADDITIONAL_INC_DIR
	   ${CMAKE_CURRENT_SOURCE_DIR}/Windows
   )
ENDIF()
	
IF (UNIX AND NOT APPLE)
   set(TELEINFO_ADDITIONAL_INC_DIR
      ${CMAKE_CURRENT_SOURCE_DIR}/Linux
   )
ENDIF()

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
PLUGIN_POST_BUILD_COPY_FILE(Linky changelog.md)
PLUGIN_POST_BUILD_COPY_FILE(Linky icon.png)
PLUGIN_POST_BUILD_COPY_DIRECTORY(Linky locales)

IF(WIN32)
   PLUGIN_POST_BUILD_COPY_FILE(Linky ftdi/ftd2xx.dll)
ENDIF()