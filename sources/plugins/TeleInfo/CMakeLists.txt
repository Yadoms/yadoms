
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
   IFTD2XManager.h
   ftdi/ftd2xx.h
)

IF (WIN32)
    set(TELEINFO_SOURCES ${TELEINFO_SOURCES}
	# Add Windows sources
	Windows/FT2xxSerialPort.h
	Windows/FT2xxSerialPort.cpp
	)
	
	source_group(Windows Windows/*.*)
	
ENDIF()

IF (UNIX AND NOT APPLE)
    set(TELEINFO_SOURCES ${TELEINFO_SOURCES}
	# Add Common Linux and RaspberryPI sources
	Linux/FT2XManager.h
	Linux/FT2XManager.cpp
	)
	
	source_group(Linux Linux/*.*)
ENDIF()

#########################################
## Place here all additional groups (solution folder)
#########################################
source_group(sources ./*.*)
source_group(sources\\Keywords Keywords/*.*)
source_group(sources\\specificHistorizer specificHistorizer/*.*)

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
PLUGIN_POST_BUILD_COPY_FILE(teleinfo changelog.md)
PLUGIN_POST_BUILD_COPY_FILE(teleinfo icon.png)
PLUGIN_POST_BUILD_COPY_DIRECTORY(teleinfo locales)

IF(WIN32)
   PLUGIN_POST_BUILD_COPY_FILE(teleinfo ftdi/ftd2xx.dll)
ENDIF()