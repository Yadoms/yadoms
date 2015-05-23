
# include common plugin definitions
include("../YadomsPluginsCommon.cmake")

#########################################
## Place here all the plugin source files
#########################################

set(CARTELECTRONIC_TELEINFO_USB_SOURCES
   IncrementSequenceNumber.h
   ISequenceNumberProvider.h
   ITeleInfoConfiguration.h
   ITeleInfoMessage.h
   ITransceiver.h
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
   TeleInfotrxHelpers.h
   Transceiver.h
   Transceiver.cpp
   TransceiverStatus.h
   TransceiverStatus.cpp
   Keywords/ApparentPowerMeter.cpp
   Keywords/ApparentPowerMeter.h
   Keywords/CurrentMeter.cpp
   Keywords/CurrentMeter.h
   Keywords/IKeyword.h
   Keywords/PowerMeter.cpp
   Keywords/PowerMeter.h
)

source_group(Keywords Keywords/*.*)

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
## even if CARTELECTRONIC_TELEINFO_USB_ADDITIONAL_INC_DIR or CARTELECTRONIC_TELEINFO_USB_ADDITIONAL_LIB are empty
#########################################
PLUGIN_SOURCES(teleinfoUSB ${CARTELECTRONIC_TELEINFO_USB_SOURCES})
PLUGIN_INCLDIR(teleinfoUSB ${CARTELECTRONIC_TELEINFO_USB_ADDITIONAL_INC_DIR})
PLUGIN_LINK(teleinfoUSB ${CARTELECTRONIC_TELEINFO_USB_ADDITIONAL_LIB})

#########################################
## Post-build commands
## Copy files to build directory
## "package.json" and "icon.png" are needed, but you can add extra-files
#########################################
PLUGIN_POST_BUILD_COPY_FILE(teleinfoUSB package.json)
PLUGIN_POST_BUILD_COPY_FILE(teleinfoUSB icon.png)
PLUGIN_POST_BUILD_COPY_FILE(teleinfoUSB locales/fr.json)
