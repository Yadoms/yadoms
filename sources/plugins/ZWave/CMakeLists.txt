# include common plugin definitions
include("../YadomsPluginsCommon.cmake")


set(ZWAVE_PLUGIN_SOURCES
	stdafx.h
	stdafx.cpp
	ZWave.h
	ZWave.cpp 
	ZWaveConfiguration.h
	ZWaveConfiguration.cpp 	
	IZWaveController.h
	ZWaveControllerFactory.h
	ZWaveControllerFactory.cpp 	
	OpenZWaveController.h
	OpenZWaveController.cpp 
	OpenZWaveCommandClass.h
	OpenZWaveCommandClass.cpp 	
	IOpenZWaveNodeKeyword.h
	OpenZWaveNode.h
	OpenZWaveNode.cpp 
	OpenZWaveNodeCapacity.h
	OpenZWaveNodeCapacity.cpp 
	OpenZWaveNodeKeywordBase.h
	OpenZWaveNodeKeywordBase.cpp 
	OpenZWaveNodeKeywordFactory.h
	OpenZWaveNodeKeywordFactory.cpp 
	KeywordContainer.h
	${OPENZWAVE_MAIN_SOURCES}
)

ADD_SUBDIRECTORY(OpenZWave)
IF(MSVC OR XCODE)
	SET_PROPERTY(TARGET openzwave PROPERTY FOLDER "plugins/external-libs")
ENDIF()


#########################################
## Place here all additional include directories
## (by default yadoms-shared and boost are included)
#########################################
set(ZWAVE_PLUGIN_ADDITIONAL_INC_DIR
	${OPENZWAVE_INCLUDE_DIRECTORY}
)

#########################################
## Place here all additional libraries to link with
## (by default yadoms-shared and boost are included)
#########################################
set(ZWAVE_PLUGIN_ADDITIONAL_LIB
	openzwave
)

#add platform specific link libs
if ( WIN32 )
	set(ZWAVE_PLUGIN_ADDITIONAL_LIB
		${ZWAVE_PLUGIN_ADDITIONAL_LIB}
		Setupapi.lib
	)
ELSEIF(APPLE)
		set(ZWAVE_PLUGIN_ADDITIONAL_LIB
			${ZWAVE_PLUGIN_ADDITIONAL_LIB})
else()
	include(FindUDev.cmake)
	if(UDEV_FOUND)
		set(ZWAVE_PLUGIN_ADDITIONAL_LIB
			${ZWAVE_PLUGIN_ADDITIONAL_LIB}
			${UDEV_LIBRARIES}
		)
	else()
		message(error "Fail to find udev")
	endif()
	
endif()


#########################################
## Place here all additional compiler flags
## i.e. : for raspberry to link with wiringPi : use add_definition("-lwiringPi")
#########################################
##add_definitions("-lwiringPi")

#########################################
## Always leave these lines to configure plugin correctly
## even if FAKE_PLUGIN_INCLDIR or FAKE_PLUGIN_LINK are empty
#########################################
PLUGIN_SOURCES(ZWave ${ZWAVE_PLUGIN_SOURCES})
PLUGIN_INCLDIR(ZWave ${ZWAVE_PLUGIN_ADDITIONAL_INC_DIR})
PLUGIN_LINK(ZWave ${ZWAVE_PLUGIN_ADDITIONAL_LIB})

#########################################
## Post-build commands
## Copy files to build directory
## "package.json" and "icon.png" are needed, but you can add extra-files
#########################################
PLUGIN_POST_BUILD_COPY_FILE(ZWave package.json)
PLUGIN_POST_BUILD_COPY_FILE(ZWave icon.png)
PLUGIN_POST_BUILD_COPY_DIRECTORY(ZWave locales)

#########################################
## Copy configuration files
#########################################
PLUGIN_POST_BUILD_COPY_DIRECTORY(ZWave config)
