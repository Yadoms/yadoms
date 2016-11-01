# include common plugin definitions

include("../YadomsPluginsCommon.cmake")

#########################################
## Place here all the plugin source files
#########################################
set(IPX800_PLUGIN_SOURCES
    IIO.h
	IO.cpp
	IO.h
	IOManager.cpp
	IOManager.h
	IIPX800Configuration.h
    IPX800.cpp
    IPX800.h
	IPX800Configuration.cpp
	IPX800Configuration.h
	IPX800Factory.cpp
	IPX800Factory.h
	stdafx.cpp
	stdafx.h
	urlManager.cpp
	urlManager.h
	)

#########################################
## Place here all additional include directories
## (by default yadoms-shared and boost are included)
#########################################
set(IPX800_PLUGIN_INCLDIR
)

#########################################
## Place here all additional libraries to link with
## (by default yadoms-shared and boost are included)
#########################################
set(IPX800_PLUGIN_LINK
)

#########################################
## Place here all additional compiler flags
## i.e. : for raspberry to link with wiringPi : use add_definition("-lwiringPi")
#########################################

#########################################
## Always leave these lines to configure plugin correctly
## even if FAKE_PLUGIN_INCLDIR or FAKE_PLUGIN_LINK are empty
#########################################

PLUGIN_SOURCES(IPX800 ${IPX800_PLUGIN_SOURCES})
PLUGIN_INCLDIR(IPX800 ${IPX800_PLUGIN_INCLDIR})
PLUGIN_LINK   (IPX800 ${IPX800_PLUGIN_ADDITIONAL_LIB})

#########################################
## Post-build commands
## Copy files to build directory
## "package.json" and "icon.png" are needed, but you can add extra-files
#########################################
PLUGIN_POST_BUILD_COPY_FILE(IPX800 package.json)
PLUGIN_POST_BUILD_COPY_FILE(IPX800 icon.png)
PLUGIN_POST_BUILD_COPY_DIRECTORY(IPX800 locales)