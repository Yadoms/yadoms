# include common plugin definitions

include("../YadomsPluginsCommon.cmake")

#########################################
## Place here all the plugin source files
#########################################
set(SIGFOX_PLUGIN_SOURCES
   ISigfoxConfiguration.h
   Sigfox.cpp
   Sigfox.h
   SigfoxConfiguration.cpp
   SigfoxConfiguration.h
   stdafx.cpp
   stdafx.h
   specificHistorizer/SNR.cpp
   specificHistorizer/SNR.h
   webServer/sigfoxHTTPServer.cpp
   webServer/sigfoxHTTPServer.h
   webServer/sigfoxRequestHandler.cpp
   webServer/sigfoxRequestHandler.h
   webServer/sigfoxRequestHandlerFactory.cpp
   webServer/sigfoxRequestHandlerFactory.h
)

#########################################
## Place here all additional groups (solution folder)
#########################################
source_group(sources ./*.*)
source_group(sources\\specificHistorizer specificHistorizer/*.*)
source_group(sources\\webServer webServer/*.*)
	
#########################################
## Place here all additional include directories
## (by default yadoms-shared and boost are included)
#########################################
set(SIGFOX_PLUGIN_INCLDIR
)

#########################################
## Place here all additional libraries to link with
## (by default yadoms-shared and boost are included)
#########################################
set(SIGFOX_PLUGIN_LINK
)

#########################################
## Place here all additional compiler flags
## i.e. : for raspberry to link with wiringPi : use add_definition("-lwiringPi")
#########################################

#########################################
## Always leave these lines to configure plugin correctly
## even if FAKE_PLUGIN_INCLDIR or FAKE_PLUGIN_LINK are empty
#########################################
PLUGIN_SOURCES(Sigfox ${SIGFOX_PLUGIN_SOURCES})
PLUGIN_INCLDIR(Sigfox ${SIGFOX_PLUGIN_INCLDIR})
PLUGIN_LINK   (Sigfox ${SIGFOX_PLUGIN_ADDITIONAL_LIB})
