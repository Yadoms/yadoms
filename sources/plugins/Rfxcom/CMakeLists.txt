
# include common plugin definitions
include("../YadomsPluginsCommon.cmake")

#########################################
## Place here all the plugin source files
#########################################
set(RFXCOM_MESSAGES_SOURCES
   rfxcomMessages/Ack.h
   rfxcomMessages/Ack.cpp
   rfxcomMessages/Barometric.h
   rfxcomMessages/Barometric.cpp
   rfxcomMessages/Bbq.h
   rfxcomMessages/Bbq.cpp
   rfxcomMessages/Blinds1.h
   rfxcomMessages/Blinds1.cpp
   rfxcomMessages/Chime.h
   rfxcomMessages/Chime.cpp
   rfxcomMessages/Current.h
   rfxcomMessages/Current.cpp
   rfxcomMessages/CurrentEnergy.h
   rfxcomMessages/CurrentEnergy.cpp
   rfxcomMessages/Curtain1.h
   rfxcomMessages/Curtain1.cpp
   rfxcomMessages/IRfxcomMessage.h
   rfxcomMessages/Fan.h
   rfxcomMessages/Fan.cpp
   rfxcomMessages/FS20.h
   rfxcomMessages/FS20.cpp
   rfxcomMessages/Energy.h
   rfxcomMessages/Energy.cpp
   rfxcomMessages/Humidity.h
   rfxcomMessages/Humidity.cpp
   rfxcomMessages/ILighting2Subtype.h
   rfxcomMessages/ILighting5Subtype.h
   rfxcomMessages/IThermostat1Subtype.h
   rfxcomMessages/Lighting1.h
   rfxcomMessages/Lighting1.cpp
   rfxcomMessages/Lighting2.h
   rfxcomMessages/Lighting2.cpp
   rfxcomMessages/Lighting2Dimmable.h
   rfxcomMessages/Lighting2Dimmable.cpp
   rfxcomMessages/Lighting2OnOff.h
   rfxcomMessages/Lighting2OnOff.cpp
   rfxcomMessages/Lighting3.h
   rfxcomMessages/Lighting3.cpp
   rfxcomMessages/Lighting4.h
   rfxcomMessages/Lighting4.cpp
   rfxcomMessages/Lighting5.h
   rfxcomMessages/Lighting5.cpp
   rfxcomMessages/Lighting5LightwaveRf.h
   rfxcomMessages/Lighting5LightwaveRf.cpp
   rfxcomMessages/Lighting5Livolo.h
   rfxcomMessages/Lighting5Livolo.cpp
   rfxcomMessages/Lighting5MdRemote.h
   rfxcomMessages/Lighting5MdRemote.cpp
   rfxcomMessages/Lighting5OnOff.h
   rfxcomMessages/Lighting5OnOff.cpp
   rfxcomMessages/Lighting6.h
   rfxcomMessages/Lighting6.cpp
   rfxcomMessages/Power.h
   rfxcomMessages/Power.cpp
   rfxcomMessages/Rain.h
   rfxcomMessages/Rain.cpp
   rfxcomMessages/RFXtrx.h
   rfxcomMessages/RFXtrxHelpers.h
   rfxcomMessages/RFXtrxHelpers.cpp
   rfxcomMessages/RFXMeter.h
   rfxcomMessages/RFXMeter.cpp
   rfxcomMessages/RFXSensor.h
   rfxcomMessages/RFXSensor.cpp
   rfxcomMessages/Rfy.h
   rfxcomMessages/Rfy.cpp
   rfxcomMessages/Security1.h
   rfxcomMessages/Security1.cpp
   rfxcomMessages/Thermostat1.h
   rfxcomMessages/Thermostat1.cpp
   rfxcomMessages/Thermostat2.h
   rfxcomMessages/Thermostat2.cpp
   rfxcomMessages/Thermostat1Digimax.h
   rfxcomMessages/Thermostat1Digimax.cpp
   rfxcomMessages/Thermostat1DigimaxShort.h
   rfxcomMessages/Thermostat1DigimaxShort.cpp
   rfxcomMessages/Temp.h
   rfxcomMessages/Temp.cpp
   rfxcomMessages/TempHumidity.h
   rfxcomMessages/TempHumidity.cpp
   rfxcomMessages/TempHumidityBarometric.h
   rfxcomMessages/TempHumidityBarometric.cpp
   rfxcomMessages/TransceiverStatus.h
   rfxcomMessages/TransceiverStatus.cpp
   rfxcomMessages/UV.h
   rfxcomMessages/UV.cpp
   rfxcomMessages/Weight.h
   rfxcomMessages/Weight.cpp
   rfxcomMessages/Wind.h
   rfxcomMessages/Wind.cpp
)

set(RFXCOM_SOURCES
   stdafx.h
   stdafx.cpp
   IncrementSequenceNumber.h
   IRfxcomConfiguration.h
   ISequenceNumberProvider.h
   ITransceiver.h
   NullSequenceNumber.h
   ProtocolException.hpp
   Rfxcom.h
   Rfxcom.cpp
   RfxComReceiveBufferHandler.h
   RfxComReceiveBufferHandler.cpp
   RfxcomConfiguration.h
   RfxcomConfiguration.cpp
   RfxcomFactory.h
   RfxcomFactory.cpp
   Transceiver.h
   Transceiver.cpp
   ${RFXCOM_MESSAGES_SOURCES}
)

source_group(rfxcomMessages rfxcomMessages/*.*)

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
