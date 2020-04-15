find_package(OpenCV)
include(FindLibUdev.cmake)

if(OpenCV_FOUND)
	if(UNIX AND NOT LIBUDEV_FOUND)
		message(WARNING "StreamDeck plugin is not available. LIBUDEV is Missing")
	else()
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
			hidapi/header/hidapi.h

		)
		if(WIN32)
			set(STREAMDECK_PLUGIN_SOURCES ${STREAMDECK_PLUGIN_SOURCES}
			 	${CMAKE_CURRENT_SOURCE_DIR}/hidapi/sources/windows/hid.c
			)
		elseif(UNIX AND NOT APPLE)
			set(STREAMDECK_PLUGIN_SOURCES ${STREAMDECK_PLUGIN_SOURCES}
			 	${CMAKE_CURRENT_SOURCE_DIR}/hidapi/sources/linux/hid.c
			)
		elseif(APPLE)
			set(STREAMDECK_PLUGIN_SOURCES ${STREAMDECK_PLUGIN_SOURCES}
				 ${CMAKE_CURRENT_SOURCE_DIR}/hidapi/sources/mac/hid.c
			)
		else()
			message(WARNING "Platform not handled")
		endif()

		set(HIDAPI_INCLUDE_DIR 
			${CMAKE_CURRENT_SOURCE_DIR}/hidapi/header
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
	  		${OpenCV_INCLUDE_DIRS}
	  		${HIDAPI_INCLUDE_DIR}
	  		${LIBUDEV_INCLUDE_DIR}
		)

		#########################################
		## Place here all additional libraries to link with
		## (by default yadoms-shared and boost are included)
		#########################################
		set(STREAMDECK_PLUGIN_LINK
	  		${OpenCV_LIBS}
	  		${LIBUDEV_LIBRARY}
		)

		#########################################
		## Always leave these lines to configure plugin correctly
		## even if STREAMDECK_PLUGIN_INCLDIR or STREAMDECK_PLUGIN_LINK are empty
		#########################################
		PLUGIN_SOURCES(StreamDeck ${STREAMDECK_PLUGIN_SOURCES})
		PLUGIN_INCLDIR(StreamDeck ${STREAMDECK_PLUGIN_INCLDIR})
		PLUGIN_LINK(StreamDeck ${STREAMDECK_PLUGIN_LINK})
	endif()	
else()
   message(WARNING "StreamDeck plugin is not available. OpenCV is Missing")
endif()