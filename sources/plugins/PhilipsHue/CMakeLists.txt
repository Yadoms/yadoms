# include common plugin definitions
include("../YadomsPluginsCommon.cmake")

#########################################
## Place here all the plugin source files
#########################################
set(PHILIPS_HUE_PLUGIN_SOURCES
	stdafx.cpp
	PhilipsHue.cpp 
	Configuration.cpp
	HueBridgeDiscovery.cpp
	HueService.cpp
	Factory.cpp
	UrlManager.cpp
	Entities/HueInformations.cpp
	Entities/Rgb.cpp
	Entities/Hsv.cpp
	Entities/Xy.cpp
	Entities/HueLightInformations.cpp
	Entities/HueState.cpp
	Entities/HueSwUpdate.cpp
	Entities/HueCapabilities.cpp
	Entities/HueControl.cpp
	Entities/HueColorGamut.cpp
	Entities/HueCt.cpp
	Entities/HueStreaming.cpp
	Entities/HueConfig.cpp
	Entities/HueStartUp.cpp
	stdafx.h
	PhilipsHue.h
	Configuration.h
	HueBridgeDiscovery.h
	IHueBridgeDiscovery.h
	HueService.h
	Factory.h
	IHueService.h
	UrlManager.h
	Entities/HueInformations.h
	Entities/Rgb.h
	Entities/Hsv.h
	Entities/Xy.h
	Entities/HueLightInformations.h
	Entities/HueState.h
	Entities/HueSwUpdate.h
	Entities/HueCapabilities.h
	Entities/HueControl.h
	Entities/HueColorGamut.h
	Entities/HueCt.h
	Entities/HueStreaming.h
	Entities/HueConfig.h
	Entities/HueStartUp.h
)

set(PHILIPS_HUE_PLUGIN_LIGHTS_MODELS_SOURCES
	Devices/Lights/Models/GamutC.cpp
	Devices/Lights/Models/GamutC.h	
	Devices/Lights/Models/GamutA.cpp
	Devices/Lights/Models/GamutA.h	
	Devices/Lights/Models/GamutB.cpp
	Devices/Lights/Models/GamutB.h
	Devices/Lights/Models/GamutOther.cpp
	Devices/Lights/Models/GamutOther.h
	Devices/Lights/Models/GamutNone.cpp
	Devices/Lights/Models/GamutNone.h


)

set(PHILIPS_HUE_PLUGIN_LIGHTS_SERVICE_SOURCES
	Devices/Lights/Service/LightsService.h
	Devices/Lights/Service/LightsService.cpp
)

set(PHILIPS_HUE_PLUGIN_DEVICES_INTERFACES_SOURCES
	Devices/Interfaces/ILightsService.h
	Devices/Interfaces/ILight.h
)

set(PHILIPS_HUE_PLUGIN_DEVICES_UTILS_SOURCES
	Devices/Utils/HueLightsResponseType.cpp
	Devices/Utils/HueLightsResponseType.h
	Devices/Utils/ColorConverter.h
	Devices/Utils/ColorConverter.cpp
	Devices/Utils/LightModel.cpp
	Devices/Utils/LightModel.h
	Devices/Utils/LightType.cpp
	Devices/Utils/LightType.h
	Devices/Utils/ColorType.h
	Devices/Utils/ColorType.cpp
	Devices/Utils/ColorTypeHelper.h
	Devices/Utils/ColorTypeHelper.cpp
)

#########################################
## Place here all additional groups (solution folder)
#########################################
source_group(sources\\Entities Entities/*.*)
source_group(sources\\Devices\\Lights\\Models FILES ${PHILIPS_HUE_PLUGIN_LIGHTS_MODELS_SOURCES})
source_group(sources\\Devices\\Lights\\Service FILES ${PHILIPS_HUE_PLUGIN_LIGHTS_SERVICE_SOURCES})
source_group(sources\\Devices\\Interfaces FILES ${PHILIPS_HUE_PLUGIN_DEVICES_INTERFACES_SOURCES})
source_group(sources\\Devices\\Utils FILES ${PHILIPS_HUE_PLUGIN_DEVICES_UTILS_SOURCES})
source_group(sources ./*.*)

#########################################
## Place here all additional include directories
## (by default yadoms-shared and boost are included)
#########################################
set(PHILIPS_HUE_PLUGIN_INCLDIR
)

#########################################
## Place here all additional libraries to link with
## (by default yadoms-shared and boost are included)
#########################################
set(PHILIPS_HUE_PLUGIN_LINK
)

#########################################
## Place here all additional compiler flags
## i.e. : for raspberry to link with wiringPi : use add_definition("-lwiringPi")
#########################################
##add_definitions("-lwiringPi")

#########################################
## Always leave these lines to configure plugin correctly
## even if EMPTY_PLUGIN_INCLDIR or EMPTY_PLUGIN_LINK are empty
#########################################
PLUGIN_SOURCES(PhilipsHue ${PHILIPS_HUE_PLUGIN_SOURCES} ${PHILIPS_HUE_PLUGIN_LIGHTS_MODELS_SOURCES} ${PHILIPS_HUE_PLUGIN_LIGHTS_SERVICE_SOURCES} ${PHILIPS_HUE_PLUGIN_DEVICES_INTERFACES_SOURCES} ${PHILIPS_HUE_PLUGIN_DEVICES_UTILS_SOURCES})
PLUGIN_INCLDIR(PhilipsHue ${PHILIPS_HUE_PLUGIN_ADDITIONAL_INC_DIR})
PLUGIN_LINK(PhilipsHue ${PHILIPS_HUE_PLUGIN_ADDITIONAL_LIB})