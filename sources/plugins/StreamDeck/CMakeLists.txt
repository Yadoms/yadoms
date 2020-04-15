find_package(OpenCV)

if(OpenCV_FOUND)

	# include common plugin definitions
	include("../YadomsPluginsCommon.cmake")
	# TODO : HANDLE Mac & linux platforms
	if(WIN32)	
		set(LIBUSB1_INCLUDE_DIR ${HIDAPI_ROOT}/hidapi)
		set(HIDAPI_INC_DIR ${HIDAPI_ROOT}/windows/Release)
		set(HIDAPI_LIBRARIES ${HIDAPI_ROOT}/windows/Release/hidapi.lib)
	endif()	
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
		DeviceManagerHelper.cpp
		DeviceManagerHelper.h
		CFactory.cpp
		CFactory.h
		StreamDeckOriginal.cpp
		StreamDeckOriginal.h
		StreamDeckOriginalV2.cpp
		StreamDeckOriginalV2.h
		StreamDeckMini.cpp
		StreamDeckMini.h
		StreamDeckXL.cpp
		StreamDeckXL.h
		ImageHelper.cpp
		ImageHelper.h
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
	  ${LIBUSB1_INCLUDE_DIR}
	  ${OpenCV_INCLUDE_DIRS}
	)

	#########################################
	## Place here all additional libraries to link with
	## (by default yadoms-shared and boost are included)
	#########################################
	set(STREAMDECK_PLUGIN_LINK
      ${LIBUSBPP_LIBRARIES}
	  ${HIDAPI_LIBRARIES}
	  ${OpenCV_LIBS}
	)

	#########################################
	## Always leave these lines to configure plugin correctly
	## even if STREAMDECK_PLUGIN_INCLDIR or STREAMDECK_PLUGIN_LINK are empty
	#########################################
	PLUGIN_SOURCES(StreamDeck ${STREAMDECK_PLUGIN_SOURCES})
	PLUGIN_INCLDIR(StreamDeck ${STREAMDECK_PLUGIN_INCLDIR})
	PLUGIN_LINK(StreamDeck ${STREAMDECK_PLUGIN_LINK})
   
else()
   message(WARNING "StreamDeck plugin is not available. LibUsb or OpenCV are Missing")
endif()