
# include common plugin definitions
include("../YadomsPluginsCommon.cmake")

#########################################
## Place here all the plugin source files
#########################################
set(RFXCOM_SOURCES
   stdafx.h
   stdafx.cpp
   IPort.h
   IRfxcomConfiguration.h
   ISequenceNumberProvider.h
   ITransceiver.h
   NullSequenceNumber.h
   PortException.hpp
   rfxcomMessages/IRfxcomMessage.h
   rfxcomMessages/Lighting1.h
   rfxcomMessages/Lighting1.cpp
   rfxcomMessages/Lighting3.h
   rfxcomMessages/Lighting3.cpp
   rfxcomMessages/Lighting6.h
   rfxcomMessages/Lighting6.cpp
   rfxcomMessages/Curtain1.h
   rfxcomMessages/Curtain1.cpp
   rfxcomMessages/RFXtrx.h
   rfxcomMessages/RFXtrxDefinitions.h
   xplMessages/IXplMessage.h
   xplMessages/X10Basic.h
   xplMessages/X10Basic.cpp
   Rfxcom.h
   Rfxcom.cpp 
   RfxcomConfiguration.h
   RfxcomConfiguration.cpp
   RfxcomFactory.h
   RfxcomFactory.cpp
   SerialPort.h
   SerialPort.cpp
   SerialPortSubscription.hpp
   Transceiver.h
   Transceiver.cpp
)

#########################################
## Place here all additional include directories
## (by default yadoms-shared and boost are included)
#########################################
set(RFXCOM_ADDITIONAL_INC_DIR
)

#########################################
## Place here all additional libraries to link with
## (by default yadoms-shared and boost are included)
#########################################
#todo gérer les différentes config et PF
set(RFXCOM_ADDITIONAL_LIB
)

#########################################
## Always leave this three lines to configure plugin correctly
## even if RFXCOM_ADDITIONAL_INC_DIR or RFXCOM_ADDITIONAL_LIB are empty
#########################################
PLUGIN_SOURCES(rfxcom ${RFXCOM_SOURCES})
PLUGIN_INCLDIR(rfxcom ${RFXCOM_ADDITIONAL_INC_DIR})
PLUGIN_LINK(rfxcom ${RFXCOM_ADDITIONAL_LIB})

#########################################
## Post-build commands
## Copy files to build directory
## "package.json" and "icon.png" are needed, but you can add extra-files
#########################################
PLUGIN_POST_BUILD_COPY_FILE(rfxcom package.json)
PLUGIN_POST_BUILD_COPY_FILE(rfxcom icon.png)
PLUGIN_POST_BUILD_COPY_FILE(rfxcom locales/fr.json)
