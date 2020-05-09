find_package(OpenCV)

if(OpenCV_FOUND)
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
			 	hidapi/sources/windows/hid.c
				windows/DeviceManagerHelper.cpp
				windows/DeviceManagerHelper.h
			)
			source_group(sources\\windows windows/*.*)

		elseif(UNIX AND NOT APPLE)
			include(FindLibUdev.cmake)
			set(STREAMDECK_PLUGIN_SOURCES ${STREAMDECK_PLUGIN_SOURCES}
			 	hidapi/sources/linux/hid.c
				linux/DeviceManagerHelper.cpp
				linux/DeviceManagerHelper.h
			)
			source_group(sources\\linux linux/*.*)

			if(UDEV_FOUND)
				set(STREAMDECK_PLUGIN_INCLDIR ${UDEV_INCLUDE_DIR})
				set(STREAMDECK_PLUGIN_LINK ${UDEV_LIBRARIES})
			else()
				message(WARNING "StreamDeck plugin is not available. LIBUDEV is Missing")
			endif()
	
		elseif(APPLE)
			set(STREAMDECK_PLUGIN_SOURCES ${STREAMDECK_PLUGIN_SOURCES}
				 ${CMAKE_CURRENT_SOURCE_DIR}/hidapi/sources/mac/hid.c
				 ${CMAKE_CURRENT_SOURCE_DIR}/linux/DeviceManagerHelper.cpp
				 ${CMAKE_CURRENT_SOURCE_DIR}/linux/DeviceManagerHelper.h
			)
			source_group(sources\\linux linux/*.*)
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
		if(WIN32)
			set(STREAMDECK_ADDITIONAL_INC_DIR
				${CMAKE_CURRENT_SOURCE_DIR}/windows
		)
		elseif(UNIX)
			set(STREAMDECK_ADDITIONAL_INC_DIR
				${CMAKE_CURRENT_SOURCE_DIR}/linux
			)
		endif()

		set(STREAMDECK_PLUGIN_INCLDIR
			${STREAMDECK_PLUGIN_INCLDIR}
			${STREAMDECK_ADDITIONAL_INC_DIR}
	  		${OpenCV_INCLUDE_DIRS}
	  		${HIDAPI_INCLUDE_DIR}
		)

		#########################################
		## Place here all additional libraries to link with
		## (by default yadoms-shared and boost are included)
		#########################################
		set(STREAMDECK_PLUGIN_LINK
			${STREAMDECK_PLUGIN_LINK}
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
   message(WARNING "StreamDeck plugin is not available. OpenCV is Missing")
endif()