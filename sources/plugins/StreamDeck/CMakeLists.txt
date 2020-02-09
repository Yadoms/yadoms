
if(LIBUSBPP_FOUND)

	# include common plugin definitions
	include("../YadomsPluginsCommon.cmake")

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
		DeviceManager.cpp
		DeviceManager.h
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
   
else()
   message(WARNING "StreamDeck plugin is not available. LibUsb is Missing")
endif()