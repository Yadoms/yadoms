
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
   rfxcomMessages/Camera1.h
   rfxcomMessages/Camera1.cpp
   rfxcomMessages/Cartelectronic.h
   rfxcomMessages/Cartelectronic.cpp
   rfxcomMessages/CartelectronicEncoder.h
   rfxcomMessages/CartelectronicEncoder.cpp
   rfxcomMessages/CartelectronicLinky.h
   rfxcomMessages/CartelectronicLinky.cpp
   rfxcomMessages/CartelectronicTIC.h
   rfxcomMessages/CartelectronicTIC.cpp
   rfxcomMessages/Chime.h
   rfxcomMessages/Chime.cpp
   rfxcomMessages/Current.h
   rfxcomMessages/ChimeByronSx.h
   rfxcomMessages/ChimeByronSx.cpp
   rfxcomMessages/ChimeByronMp001.h
   rfxcomMessages/ChimeByronMp001.cpp
   rfxcomMessages/Current.cpp
   rfxcomMessages/CurrentEnergy.h
   rfxcomMessages/CurrentEnergy.cpp
   rfxcomMessages/Curtain1.h
   rfxcomMessages/Curtain1.cpp
   rfxcomMessages/DateTime.h
   rfxcomMessages/DateTime.cpp
   rfxcomMessages/IRfxcomMessage.h
   rfxcomMessages/Fan.h
   rfxcomMessages/Fan.cpp
   rfxcomMessages/FS20.h
   rfxcomMessages/FS20.cpp
   rfxcomMessages/Energy.h
   rfxcomMessages/Energy.cpp
   rfxcomMessages/HomeConfort.h
   rfxcomMessages/HomeConfort.cpp
   rfxcomMessages/Humidity.h
   rfxcomMessages/Humidity.cpp
   rfxcomMessages/ICartelectronicSubtype.h
   rfxcomMessages/IChimeSubtype.h
   rfxcomMessages/ILighting2Subtype.h
   rfxcomMessages/ILighting5Subtype.h
   rfxcomMessages/IRemoteSubtype.h
   rfxcomMessages/ISecurity1Subtype.h
   rfxcomMessages/ISecurity2Subtype.h
   rfxcomMessages/IThermostat1Subtype.h
   rfxcomMessages/IThermostat3Subtype.h
   rfxcomMessages/Lighting1.h
   rfxcomMessages/Lighting1.cpp
   rfxcomMessages/Lighting2.h
   rfxcomMessages/Lighting2.cpp
   rfxcomMessages/Lighting2OnOffOrDimmable.h
   rfxcomMessages/Lighting2OnOffOrDimmable.cpp
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
   rfxcomMessages/Radiator1.h
   rfxcomMessages/Radiator1.cpp
   rfxcomMessages/RareDeviceIdFilter.h
   rfxcomMessages/RareDeviceIdFilter.cpp
   rfxcomMessages/Remote.h
   rfxcomMessages/Remote.cpp
   rfxcomMessages/RemoteAtiWonder2.h
   rfxcomMessages/RemoteAtiWonder2.cpp
   rfxcomMessages/RemoteStandard.hpp
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
   rfxcomMessages/Security1KD101_SA30.h
   rfxcomMessages/Security1KD101_SA30.cpp
   rfxcomMessages/Security1Meiantech.h
   rfxcomMessages/Security1Meiantech.cpp
   rfxcomMessages/Security1PowerCodeSensor.h
   rfxcomMessages/Security1PowerCodeSensor.cpp
   rfxcomMessages/Security1PowerCodeMotion.h
   rfxcomMessages/Security1PowerCodeMotion.cpp
   rfxcomMessages/Security1X10.h
   rfxcomMessages/Security1X10.cpp
   rfxcomMessages/Security1X10M.h
   rfxcomMessages/Security1X10M.cpp
   rfxcomMessages/Security1X10R.h
   rfxcomMessages/Security1X10R.cpp
   rfxcomMessages/Security2.h
   rfxcomMessages/Security2.cpp
   rfxcomMessages/Security2ClassicKeyLoq.h
   rfxcomMessages/Security2ClassicKeyLoq.cpp
   rfxcomMessages/Thermostat1.h
   rfxcomMessages/Thermostat1.cpp
   rfxcomMessages/Thermostat1Digimax.h
   rfxcomMessages/Thermostat1Digimax.cpp
   rfxcomMessages/Thermostat1DigimaxShort.h
   rfxcomMessages/Thermostat1DigimaxShort.cpp
   rfxcomMessages/Thermostat2.h
   rfxcomMessages/Thermostat2.cpp
   rfxcomMessages/Thermostat3.h
   rfxcomMessages/Thermostat3.cpp
   rfxcomMessages/Thermostat3MertikG6RH4S.h
   rfxcomMessages/Thermostat3MertikG6RH4S.cpp
   rfxcomMessages/Thermostat3MertikG6RH4T1.h
   rfxcomMessages/Thermostat3MertikG6RH4T1.cpp
   rfxcomMessages/Thermostat3MertikG6RH4TB.h
   rfxcomMessages/Thermostat3MertikG6RH4TB.cpp
   rfxcomMessages/Thermostat3MertikG6RH4TD.h
   rfxcomMessages/Thermostat3MertikG6RH4TD.cpp
   rfxcomMessages/Thermostat4.h
   rfxcomMessages/Thermostat4.cpp
   rfxcomMessages/Temp.h
   rfxcomMessages/Temp.cpp
   rfxcomMessages/TempHumidity.h
   rfxcomMessages/TempHumidity.cpp
   rfxcomMessages/TempHumidityBarometric.h
   rfxcomMessages/TempHumidityBarometric.cpp
   rfxcomMessages/TempRain.h
   rfxcomMessages/TempRain.cpp
   rfxcomMessages/TransceiverStatus.h
   rfxcomMessages/TransceiverStatus.cpp
   rfxcomMessages/UV.h
   rfxcomMessages/UV.cpp
   rfxcomMessages/Weight.h
   rfxcomMessages/Weight.cpp
   rfxcomMessages/Wind.h
   rfxcomMessages/Wind.cpp
)

set(SPECIFIC_HISTORIZERS_SOURCES
   rfxcomMessages/specificHistorizers/Color.h
   rfxcomMessages/specificHistorizers/Color.cpp
   rfxcomMessages/specificHistorizers/Period.h
   rfxcomMessages/specificHistorizers/Period.cpp
   rfxcomMessages/specificHistorizers/RemoteAtiWonderHistorizer.h
   rfxcomMessages/specificHistorizers/RemoteAtiWonderHistorizer.cpp
   rfxcomMessages/specificHistorizers/RemoteAtiWonder2Capacity.h
   rfxcomMessages/specificHistorizers/RemoteAtiWonder2Capacity.cpp
   rfxcomMessages/specificHistorizers/RemoteAtiWonder2CmdHistorizer.h
   rfxcomMessages/specificHistorizers/RemoteAtiWonder2CmdHistorizer.cpp
   rfxcomMessages/specificHistorizers/RemoteAtiWonder2CmdTypeHistorizer.h
   rfxcomMessages/specificHistorizers/RemoteAtiWonder2CmdTypeHistorizer.cpp
   rfxcomMessages/specificHistorizers/RemoteAtiWonderPlusHistorizer.h
   rfxcomMessages/specificHistorizers/RemoteAtiWonderPlusHistorizer.cpp
   rfxcomMessages/specificHistorizers/RemoteMedionHistorizer.h
   rfxcomMessages/specificHistorizers/RemoteMedionHistorizer.cpp
   rfxcomMessages/specificHistorizers/RemotePCHistorizer.h
   rfxcomMessages/specificHistorizers/RemotePCHistorizer.cpp
   rfxcomMessages/specificHistorizers/TeleInfoStatus.h
   rfxcomMessages/specificHistorizers/TeleInfoStatus.cpp   
)

set(PIC_CONFIGURATIONS_SOURCES
   picConfigurations/IPicConfiguration.h
   picConfigurations/RFXtrx.h
   picConfigurations/RFXtrx.cpp
)

set(RFXCOM_SOURCES
   stdafx.h
   stdafx.cpp
   IncrementSequenceNumber.h
   IRfxcomConfiguration.h
   IRfxcomFirmwareUpdater.h
   ISequenceNumber.h
   ITransceiver.h
   IUnsecuredProtocolFilter.h
   ManuallyDeviceCreationException.hpp
   MessageFilteredException.hpp
   NullSequenceNumber.h
   PicBoot.h
   PicBoot.cpp
   PicBootPacketControlCharacters.h
   PicBootReceiveBufferHandler.h
   PicBootReceiveBufferHandler.cpp
   ProtocolException.hpp
   Rfxcom.h
   Rfxcom.cpp
   RfxComReceiveBufferHandler.h
   RfxComReceiveBufferHandler.cpp
   RfxcomConfiguration.h
   RfxcomConfiguration.cpp
   RfxcomFactory.h
   RfxcomFactory.cpp
   RfxcomFirmwareUpdater.h
   RfxcomFirmwareUpdater.cpp
   Transceiver.h
   Transceiver.cpp
   ${SPECIFIC_HISTORIZERS_SOURCES}
   ${PIC_CONFIGURATIONS_SOURCES}
   ${RFXCOM_MESSAGES_SOURCES}
)

#########################################
## Place here all additional groups (solution folder)
#########################################
source_group(sources ./*.*)
source_group(sources\\rfxcomMessages rfxcomMessages/*.*)
source_group(sources\\rfxcomMessages\\specificHistorizers rfxcomMessages/specificHistorizers/*.*)
source_group(sources\\rfxcomMessages\\Keywords rfxcomMessages/Keywords/*.*)
source_group(sources\\picConfigurations picConfigurations/*.*)

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
PLUGIN_POST_BUILD_COPY_DIRECTORY(rfxcom locales)
