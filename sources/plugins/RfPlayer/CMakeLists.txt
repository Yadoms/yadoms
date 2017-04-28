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
   frames/outgoing/Factory.h
   frames/outgoing/Factory.cpp
   frames/outgoing/Blyss433.h
   frames/outgoing/Blyss433.cpp

   specificHistorizers/Type0State.h
   specificHistorizers/Type0State.cpp
   specificHistorizers/Type1State.h
   specificHistorizers/Type1State.cpp
   specificHistorizers/Type2KeyCode.h
   specificHistorizers/Type2KeyCode.cpp
   specificHistorizers/Type3ShutterCode.h
   specificHistorizers/Type3ShutterCode.cpp
   specificHistorizers/Type3RemoteCode.h
   specificHistorizers/Type3RemoteCode.cpp
   specificHistorizers/Type10OperatingMode.h
   specificHistorizers/Type10OperatingMode.cpp
   specificHistorizers/Type11State.h
   specificHistorizers/Type11State.cpp
)

source_group(sources ./*.*)
source_group(sources\\frames ./frames/*.*)
source_group(sources\\frames\\incoming ./frames/incoming/*.*)
source_group(sources\\frames\\outgoing ./frames/outgoing/*.*)
source_group(sources\\specificHistorizers ./specificHistorizers/*.*)

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
## Place here all additional compiler flags
## i.e. : for raspberry to link with wiringPi : use add_definition("-lwiringPi")
#########################################
##add_definitions("-lwiringPi")

#########################################
## Always leave these lines to configure plugin correctly
## even if FAKE_PLUGIN_INCLDIR or FAKE_PLUGIN_LINK are empty
#########################################
PLUGIN_SOURCES(RfPlayer ${RFPLAYER_PLUGIN_SOURCES})
PLUGIN_INCLDIR(RfPlayer ${RFPLAYER_PLUGIN_ADDITIONAL_INC_DIR})
PLUGIN_LINK(RfPlayer ${RFPLAYER_PLUGIN_ADDITIONAL_LIB})

#########################################
## Post-build commands
## Copy files to build directory
## "package.json" and "icon.png" are needed, but you can add extra-files
#########################################
PLUGIN_POST_BUILD_COPY_FILE(RfPlayer package.json)
PLUGIN_POST_BUILD_COPY_FILE(RfPlayer icon.png)
PLUGIN_POST_BUILD_COPY_DIRECTORY(RfPlayer locales)


