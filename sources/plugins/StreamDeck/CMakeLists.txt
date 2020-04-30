find_package(OpenCV)

if(OpenCV_FOUND)
		# include common plugin definitions
		include("../YadomsPluginsCommon.cmake")
		#########################################
		## Place here all the plugin source files
		#########################################
		if(ImageMagick_FOUND)
		
				message(STATUS "ImageMagick_INCLUDE_DIRS = " ${ImageMagick_INCLUDE_DIRS})
				message(STATUS "ImageMagick_LIBRARIES = " ${ImageMagick_LIBRARIES})
		
		else()
		   message(WARNING "ImageMagick is Missing")
		endif()
		set(STREAMDECK_PLUGIN_SOURCES
			stdafx.h
			stdafx.cpp
			StreamDeck.h
			StreamDeck.cpp 
			Configuration.h
			Configuration.cpp
			DeviceManager.cpp
			DeviceManager.h
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
				${CMAKE_CURRENT_SOURCE_DIR}/platforms/windows/DeviceManagerHelper.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/platforms/windows/DeviceManagerHelper.h
			)
		elseif(UNIX AND NOT APPLE)
			include(FindLibUdev.cmake)
			set(STREAMDECK_PLUGIN_SOURCES ${STREAMDECK_PLUGIN_SOURCES}
			 	${CMAKE_CURRENT_SOURCE_DIR}/hidapi/sources/linux/hid.c
				${CMAKE_CURRENT_SOURCE_DIR}/platforms/linux/DeviceManagerHelper.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/platforms/linux/DeviceManagerHelper.h
			)
			if(UDEV_FOUND)
				set(STREAMDECK_PLUGIN_INCLDIR ${UDEV_INCLUDE_DIR})
				set(STREAMDECK_PLUGIN_LINK ${UDEV_LIBRARIES})
			else()
				message(WARNING "StreamDeck plugin is not available. LIBUDEV is Missing")
			endif()
	
		elseif(APPLE)
			set(STREAMDECK_PLUGIN_SOURCES ${STREAMDECK_PLUGIN_SOURCES}
				 ${CMAKE_CURRENT_SOURCE_DIR}/hidapi/sources/mac/hid.c
				 ${CMAKE_CURRENT_SOURCE_DIR}/platforms/linux/DeviceManagerHelper.cpp
				 ${CMAKE_CURRENT_SOURCE_DIR}/platforms/linux/DeviceManagerHelper.h
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
			${STREAMDECK_PLUGIN_INCLDIR}
	  		${OpenCV_INCLUDE_DIRS}
	  		${HIDAPI_INCLUDE_DIR}
			${ImageMagick_INCLUDE_DIRS}
		)

		#########################################
		## Place here all additional libraries to link with
		## (by default yadoms-shared and boost are included)
		#########################################
		set(STREAMDECK_PLUGIN_LINK
			${STREAMDECK_PLUGIN_LINK}
	  		${OpenCV_LIBS}
			${ImageMagick_LIBRARIES}
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