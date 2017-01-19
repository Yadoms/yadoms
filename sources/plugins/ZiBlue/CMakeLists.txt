# include common plugin definitions
include("../YadomsPluginsCommon.cmake")


set(ZIBLUE_PLUGIN_SOURCES
	stdafx.h
	stdafx.cpp
	ZiBlue.h
	ZiBlue.cpp 
	ZiBlueConfiguration.h
	ZiBlueConfiguration.cpp 	
	ZiBlueFactory.h
	ZiBlueFactory.cpp
   IZiBlueMessageHandler.h
	ZiBlueMessageHandler.h
	ZiBlueMessageHandler.cpp 	
	ZiBlueReceiveBufferHandler.h
	ZiBlueReceiveBufferHandler.cpp 	
	ZiBlueTransceiver.h
	ZiBlueTransceiver.cpp 	
   
   frames/Frame.h
   frames/Frame.cpp
   frames/AsciiFrame.h
   frames/AsciiFrame.cpp
   frames/BinaryFrame.h
   frames/FrameType.h
)

source_group(sources ./*.*)
source_group(sources\\frames ./frames/*.*)

#########################################
## Place here all additional include directories
## (by default yadoms-shared and boost are included)
#########################################
set(ZIBLUE_PLUGIN_ADDITIONAL_INC_DIR
)

#########################################
## Place here all additional libraries to link with
## (by default yadoms-shared and boost are included)
#########################################
set(ZIBLUE_PLUGIN_ADDITIONAL_LIB
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
PLUGIN_SOURCES(ZiBlue ${ZIBLUE_PLUGIN_SOURCES})
PLUGIN_INCLDIR(ZiBlue ${ZIBLUE_PLUGIN_ADDITIONAL_INC_DIR})
PLUGIN_LINK(ZiBlue ${ZIBLUE_PLUGIN_ADDITIONAL_LIB})

#########################################
## Post-build commands
## Copy files to build directory
## "package.json" and "icon.png" are needed, but you can add extra-files
#########################################
PLUGIN_POST_BUILD_COPY_FILE(ZiBlue package.json)
PLUGIN_POST_BUILD_COPY_FILE(ZiBlue icon.png)
PLUGIN_POST_BUILD_COPY_DIRECTORY(ZiBlue locales)


