
# include common plugin definitions
include("../YadomsPluginsCommon.cmake")

message("streamdeck - LIBUSBPP_FOUND=${LIBUSBPP_FOUND}")

#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../external-libs/Libusbpp ${StreamDeck_BINARY_DIR}/Libusbpp)
#set(LIBUSBPP_INCLUDE_DIRS ${LIBUSBPP_INCLUDE_DIRECTORIES})
#set(LIBUSBPP_LIBRARIES ${LIBUSBPP_LIBRARIES})
    message(STATUS "streamdeck - LIBUSBPP_INCLUDE_DIRS=${LIBUSBPP_INCLUDE_DIRS}")
    message(STATUS "streamdeck - LIBUSBPP_LIBRARIES=${LIBUSBPP_LIBRARIES}")

#########################################
## Place here all the plugin source files
#########################################
set(STREAMDECK_PLUGIN_SOURCES
	stdafx.h
	stdafx.cpp
	StreamDeck.h
	StreamDeck.cpp 
	Configuration.h
	Configuration.cpp
	UsbService.cpp
	UsbService.h
)

#########################################
## Place here all additional groups (solution folder)
#########################################
source_group(sources ./*.*)

#########################################
## Place here all additional include directories
## (by default yadoms-shared and boost are included)
#########################################
set(STREAMDECK_PLUGIN_INCLDIR
   ${LIBUSBPP_INCLUDE_DIRS}
)

#########################################
## Place here all additional libraries to link with
## (by default yadoms-shared and boost are included)
#########################################
set(STREAMDECK_PLUGIN_LINK
   ${LIBUSBPP_LIBRARIES}
)

#########################################
## Always leave these lines to configure plugin correctly
## even if STREAMDECK_PLUGIN_INCLDIR or STREAMDECK_PLUGIN_LINK are empty
#########################################
PLUGIN_SOURCES(StreamDeck ${STREAMDECK_PLUGIN_SOURCES})
PLUGIN_INCLDIR(StreamDeck ${STREAMDECK_PLUGIN_INCLDIR})
PLUGIN_LINK(StreamDeck ${STREAMDECK_PLUGIN_LINK})

if(WIN32)
   #PLUGIN_POST_BUILD_COPY_FILE(StreamDeck ${LIBUSB_1_BINARY})
endif(WIN32)
