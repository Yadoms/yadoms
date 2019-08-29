# include common plugin definitions
include("../YadomsPluginsCommon.cmake")


set(ZWAVE_PLUGIN_SOURCES
	stdafx.h
	stdafx.cpp
   ConfigurationSchemaFactory.h
   ConfigurationSchemaFactory.cpp
	ZWave.h
	ZWave.cpp 
	ZWaveConfiguration.h
	ZWaveConfiguration.cpp 	
	IZWaveController.h
	ZWaveControllerFactory.h
	ZWaveControllerFactory.cpp 	
	ZWaveInternalState.h
	ZWaveInternalState.cpp 	
	OpenZWaveController.h
	OpenZWaveController.cpp 
	OpenZWaveControllerCache.h
	OpenZWaveControllerCache.cpp 
	OpenZWaveEnumHandler.h
	OpenZWaveEnumHandler.cpp 
	OpenZWaveCommandClass.h
	OpenZWaveCommandClass.cpp 	
	IOpenZWaveNodeKeyword.h
   IOpenZWaveNodePlugin.h
	OpenZWaveNode.h
	OpenZWaveNode.cpp 
	OpenZWaveNodeConfiguration.h
	OpenZWaveNodeConfiguration.cpp 
	OpenZWaveHelpers.h
	OpenZWaveHelpers.cpp 
	OpenZWaveNodeKeywordBase.h
	OpenZWaveNodeKeywordBase.cpp 
	OpenZWaveNodeKeywordDimmable.h
	OpenZWaveNodeKeywordDimmable.cpp 
	OpenZWaveNodeKeywordColor.h
	OpenZWaveNodeKeywordColor.cpp 	
   OpenZWaveNodeKeywordEvent.h
	OpenZWaveNodeKeywordEvent.cpp 
   OpenZWaveNodeKeywordGeneric.h
   OpenZWaveNodeKeywordGenericByType.h
	OpenZWaveNodeKeywordFactory.h
	OpenZWaveNodeKeywordFactory.cpp 
   OpenZWaveNodeRefreshPlugin.h
   OpenZWaveNodeRefreshPlugin.cpp
   OpenZWaveNodeUserCodePlugin.h
   OpenZWaveNodeUserCodePlugin.cpp
   OpenZWaveNodePluginFactory.h
   OpenZWaveNodePluginFactory.cpp
	KeywordContainer.h
   OpenZWaveSingleHistorizableData.h
   historizers/Temperature.h
   historizers/Temperature.cpp
   historizers/BatteryLevel.h
   historizers/BatteryLevel.cpp
   historizers/Energy.h
   historizers/Energy.cpp
   historizers/Counter.h
   historizers/Counter.cpp
	historizers/Power.h
   historizers/Power.cpp
	historizers/Voltage.h
   historizers/Voltage.cpp
	historizers/Current.h
   historizers/Current.cpp
	historizers/PowerFactor.h
   historizers/PowerFactor.cpp
	historizers/Illumination.h
   historizers/Illumination.cpp
	historizers/Humidity.h
   historizers/Humidity.cpp
	historizers/Direction.h
   historizers/Direction.cpp
	historizers/Speed.h
   historizers/Speed.cpp
	historizers/Pressure.h
   historizers/Pressure.cpp
	historizers/RainRate.h
   historizers/RainRate.cpp
	historizers/Weight.h
   historizers/Weight.cpp
	historizers/Distance.h
   historizers/Distance.cpp
	historizers/UserCode.h
   historizers/UserCode.cpp	
   historizers/Uv.h
   historizers/Uv.cpp
	historizers/Switch.h
   historizers/Switch.cpp
   typeinfo/BoolTypeInfo.h
   typeinfo/BoolTypeInfo.cpp
   typeinfo/DecimalTypeInfo.h
   typeinfo/DecimalTypeInfo.cpp
   typeinfo/EnumTypeInfo.h
   typeinfo/EnumTypeInfo.cpp
   typeinfo/IntegerTypeInfo.h
   typeinfo/IntegerTypeInfo.cpp
   typeinfo/StringTypeInfo.h
   typeinfo/StringTypeInfo.cpp
   typeinfo/TypeInfoFactory.h
   typeinfo/TypeInfoFactory.cpp
	${OPENZWAVE_MAIN_SOURCES}
)

source_group(sources ./*.*)
source_group(sources\\historizers historizers/*.*)
source_group(sources\\typeinfo typeinfo/*.*)

ADD_SUBDIRECTORY(OpenZWave)
IF(MSVC OR XCODE)
	SET_PROPERTY(TARGET openzwave PROPERTY FOLDER "plugins/external-libs")
ENDIF()

#########################################
## Remove specific warnings (due to OpenZWave)
#########################################
if(CMAKE_COMPILER_IS_GNUCXX)
	add_definitions ("-Wno-unknown-pragmas")
endif(CMAKE_COMPILER_IS_GNUCXX)


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
		Setupapi.lib dnsapi.lib
	)
else( WIN32 )
	set(ZWAVE_PLUGIN_ADDITIONAL_LIB ${ZWAVE_PLUGIN_ADDITIONAL_LIB})
endif( WIN32)


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
## Copy configuration files
#########################################
PLUGIN_POST_BUILD_COPY_DIRECTORY(ZWave ${OPENZWAVE_CONF_DIRECTORY})
