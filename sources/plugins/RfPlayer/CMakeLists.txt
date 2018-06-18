# include common plugin definitions
include("../YadomsPluginsCommon.cmake")


set(RFPLAYER_PLUGIN_SOURCES
	stdafx.h
	stdafx.cpp
	RfPlayer.h
	RfPlayer.cpp 
	Configuration.h
	Configuration.cpp 	
   Dongle.h
   Dongle.cpp
	Factory.h
	Factory.cpp
   IMessageHandler.h
	ManuallyDeviceFactory.h
	ManuallyDeviceFactory.cpp 	
	MessageHandler.h
	MessageHandler.cpp 	
	ReceiveBufferHandler.h
	ReceiveBufferHandler.cpp 	
	Transceiver.h
	Transceiver.cpp 	
   
   frames/incoming/Frame.h
   frames/incoming/Frame.cpp
   frames/incoming/AsciiFrame.h
   frames/incoming/AsciiFrame.cpp
   frames/incoming/BinaryFrame.h
   frames/incoming/BinaryFrame.cpp
   frames/incoming/FrameType.h

   frames/outgoing/IOutgoingFrame.h
   frames/outgoing/AsciiCommand.h
   frames/outgoing/AsciiCommand.cpp
   frames/outgoing/Factory.h
   frames/outgoing/Factory.cpp
   frames/outgoing/OutgoingFrame.h
   frames/outgoing/OutgoingFrame.cpp
   
   rfplayerApi/rfplayerApi.h
   rfplayerApi/usb_frame_api.h

   specificHistorizers/incoming/Type0State.h
   specificHistorizers/incoming/Type0State.cpp
   specificHistorizers/incoming/Type1State.h
   specificHistorizers/incoming/Type1State.cpp
   specificHistorizers/incoming/Type2KeyCode.h
   specificHistorizers/incoming/Type2KeyCode.cpp
   specificHistorizers/incoming/Type3ShutterCode.h
   specificHistorizers/incoming/Type3ShutterCode.cpp
   specificHistorizers/incoming/Type3RemoteCode.h
   specificHistorizers/incoming/Type3RemoteCode.cpp
   specificHistorizers/incoming/Type10OperatingMode.h
   specificHistorizers/incoming/Type10OperatingMode.cpp
   specificHistorizers/incoming/Type11State.h
   specificHistorizers/incoming/Type11State.cpp

   specificHistorizers/outgoing/Blyss.h
   specificHistorizers/outgoing/Blyss.cpp
   specificHistorizers/outgoing/Chacon.h
   specificHistorizers/outgoing/Chacon.cpp
   specificHistorizers/outgoing/Domia.h
   specificHistorizers/outgoing/Domia.cpp
   specificHistorizers/outgoing/Kd101.h
   specificHistorizers/outgoing/Kd101.cpp
   specificHistorizers/outgoing/Parrot.h
   specificHistorizers/outgoing/Parrot.cpp
   specificHistorizers/outgoing/Rts.h
   specificHistorizers/outgoing/Rts.cpp
   specificHistorizers/outgoing/Visonic.h
   specificHistorizers/outgoing/Visonic.cpp
   specificHistorizers/outgoing/X10.h
   specificHistorizers/outgoing/X10.cpp
   specificHistorizers/outgoing/X2D.h
   specificHistorizers/outgoing/X2D.cpp
)

source_group(sources ./*.*)
source_group(sources\\frames ./frames/*.*)
source_group(sources\\frames\\incoming ./frames/incoming/*.*)
source_group(sources\\frames\\outgoing ./frames/outgoing/*.*)
source_group(sources\\rfplayerApi ./rfplayerApi/*.*)
source_group(sources\\specificHistorizers ./specificHistorizers/*.*)
source_group(sources\\specificHistorizers\\incoming ./specificHistorizers/incoming/*.*)
source_group(sources\\specificHistorizers\\outgoing ./specificHistorizers/outgoing/*.*)

#########################################
## Place here all additional include directories
## (by default yadoms-shared and boost are included)
#########################################
set(RFPLAYER_PLUGIN_ADDITIONAL_INC_DIR
)

#########################################
## Place here all additional libraries to link with
## (by default yadoms-shared and boost are included)
#########################################
set(RFPLAYER_PLUGIN_ADDITIONAL_LIB
)

#########################################
## Always leave these lines to configure plugin correctly
## even if FAKE_PLUGIN_INCLDIR or FAKE_PLUGIN_LINK are empty
#########################################
PLUGIN_SOURCES(RfPlayer ${RFPLAYER_PLUGIN_SOURCES})
PLUGIN_INCLDIR(RfPlayer ${RFPLAYER_PLUGIN_ADDITIONAL_INC_DIR})
PLUGIN_LINK(RfPlayer ${RFPLAYER_PLUGIN_ADDITIONAL_LIB})


