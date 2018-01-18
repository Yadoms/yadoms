
# include common plugin definitions
include("../YadomsPluginsCommon.cmake")

#########################################
## Place here all the plugin source files
#########################################
set(FREEMOBILESMS_SOURCES
   stdafx.h
   stdafx.cpp
   FreeMobileSMS.h
   FreeMobileSMS.cpp 	
   SmsKeyword.h
   SmsKeyword.cpp 
)


#########################################
## Place here all additional groups (solution folder)
#########################################
source_group(sources ./*.*)

#########################################
## Place here all additional include directories
## (by default yadoms-shared and boost are included)
#########################################
set(FREEMOBILESMS_INCLDIR
)

#########################################
## Place here all additional libraries to link with
## (by default yadoms-shared and boost are included)
#########################################
set(FREEMOBILESMS_LINK
)

#########################################
## Place here all additional compiler flags
## i.e. : for raspberry to link with wiringPi : use add_definition("-lwiringPi")
#########################################
##add_definitions("-lwiringPi")

#########################################
## Always leave these lines to configure plugin correctly
## even if FREEMOBILESMS_INCLDIR or FREEMOBILESMS_LINK are empty
#########################################
PLUGIN_SOURCES(freeMobileSms ${FREEMOBILESMS_SOURCES})
PLUGIN_INCLDIR(freeMobileSms ${FREEMOBILESMS_INCLDIR})
PLUGIN_LINK(freeMobileSms ${FREEMOBILESMS_LINK})

#########################################
## Post-build commands
## Copy files to build directory
## "package.json" and "icon.png" are needed, but you can add extra-files
#########################################
PLUGIN_POST_BUILD_COPY_FILE(freeMobileSms package.json)
PLUGIN_POST_BUILD_COPY_FILE(freeMobileSms changelog.md)
PLUGIN_POST_BUILD_COPY_FILE(freeMobileSms icon.png)
PLUGIN_POST_BUILD_COPY_DIRECTORY(freeMobileSms locales)
