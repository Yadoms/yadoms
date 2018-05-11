
# include common plugin definitions
include("../YadomsPluginsCommon.cmake")

#########################################
## Place here all the plugin source files
#########################################
set(NITRAM_MESSAGES_SOURCES
   NitramMessages/Ack.cpp
   NitramMessages/Ack.h
   NitramMessages/BatteryCapacityAndVoltage.cpp
   NitramMessages/BatteryCapacityAndVoltage.h
   NitramMessages/INitramMessage.h
   NitramMessages/NitramHelpers.cpp
   NitramMessages/NitramHelpers.h
   NitramMessages/Runtime.cpp
   NitramMessages/Runtime.h
   NitramMessages/SetupShutdownDelay.cpp
   NitramMessages/SetupShutdownDelay.h
   NitramMessages/SetupStartupDelay.cpp
   NitramMessages/SetupStartupDelay.h
   NitramMessages/SummaryStatus.cpp
   NitramMessages/SummaryStatus.h
   NitramMessages/MessagesDefinition.cpp
   NitramMessages/MessagesDefinition.h
   NitramMessages/specificHistorizer/UPSStatus.cpp
   NitramMessages/specificHistorizer/UPSStatus.h
   )

set(NITRAM_SOURCES
   stdafx.h
   stdafx.cpp
   INitramConfiguration.h
   Nitram.cpp
   Nitram.h
   NitramConfiguration.cpp
   NitramConfiguration.h
   NitramFactory.cpp
   NitramFactory.h
   NitramReceiveBufferHandler.cpp
   NitramReceiveBufferHandler.h
   ProtocolException.hpp
   ${NITRAM_MESSAGES_SOURCES}
)

#########################################
## Place here all additional groups (solution folder)
#########################################
source_group(sources ./*.*)
source_group(sources\\NitramMessages NitramMessages/*.*)
source_group(sources\\NitramMessages\\specificHistorizer NitramMessages/specificHistorizer/*.*)

#########################################
## Place here all additional include directories
## (by default yadoms-shared and boost are included)
#########################################
set(NITRAM_ADDITIONAL_INC_DIR
)

#########################################
## Place here all additional libraries to link with
## (by default yadoms-shared and boost are included)
#########################################
set(NITRAM_ADDITIONAL_LIB
)

#########################################
## Always leave this three lines to configure plugin correctly
## even if MEGATECUPS_ADDITIONAL_INC_DIR or MEGATECUPS_ADDITIONAL_LIB are empty
#########################################
PLUGIN_SOURCES(Nitram ${NITRAM_SOURCES})
PLUGIN_INCLDIR(Nitram ${NITRAM_ADDITIONAL_INC_DIR})
PLUGIN_LINK(Nitram ${NITRAM_ADDITIONAL_LIB})

#########################################
## Post-build commands
## Copy files to build directory
## "package.json" and "icon.png" are needed, but you can add extra-files
#########################################
PLUGIN_POST_BUILD_COPY_FILE(Nitram package.json)
PLUGIN_POST_BUILD_COPY_FILE(Nitram icon.png)
PLUGIN_POST_BUILD_COPY_DIRECTORY(Nitram locales)
