# include common plugin definitions

include("../YadomsPluginsCommon.cmake")

#########################################
## Place here all the plugin source files
#########################################
set(SYSTEMINFORMATION_PLUGIN_SOURCES
	stdafx.h
	stdafx.cpp
        Helpers/Helpers.h
        Helpers/Helpers.cpp
	ILoad.h
        ISIConfiguration.h
        SIConfiguration.h
        SIConfiguration.cpp
        SystemInformation.h
        SystemInformation.cpp
        specificHistorizers/Byte.cpp
        specificHistorizers/Byte.h
        specificHistorizers/KByte.cpp
        specificHistorizers/KByte.h
	)

   source_group(specificHistorizers specificHistorizers/*.*)

IF (WIN32)
    set(SYSTEMINFORMATION_PLUGIN_SOURCES ${SYSTEMINFORMATION_PLUGIN_SOURCES}
	# Add Windows sources
	Windows/CPULoad.h
	Windows/CPULoad.cpp
	Windows/DisksList.h
	Windows/DisksList.cpp
	Windows/DiskUsage.h
	Windows/DiskUsage.cpp
	Windows/MemoryLoad.h
	Windows/MemoryLoad.cpp
	Windows/SystemFactory.h
	Windows/SystemFactory.cpp
	)
	
	source_group(Windows Windows/*.*)
	
ENDIF()
	
IF (UNIX AND NOT APPLE)
    set(SYSTEMINFORMATION_PLUGIN_SOURCES ${SYSTEMINFORMATION_PLUGIN_SOURCES}
	# Add Common Linux and RaspberryPI sources
	CommonLinux/CPULoad.h
	CommonLinux/CPULoad.cpp
	CommonLinux/DisksList.h
	CommonLinux/DisksList.cpp
	CommonLinux/DiskUsage.h
	CommonLinux/DiskUsage.cpp
        CommonLinux/LinuxHelpers.h
        CommonLinux/LinuxHelpers.cpp
        CommonLinux/MemoryLoad.h
        CommonLinux/MemoryLoad.cpp	
	)
	
	source_group(CommonLinux CommonLinux/*.*)
ENDIF()

IF (UNIX AND NOT APPLE AND NOT (CMAKE_YADOMS_PLATFORM STREQUAL "Raspberry"))
   set(SYSTEMINFORMATION_PLUGIN_SOURCES ${SYSTEMINFORMATION_PLUGIN_SOURCES}
      Linux/SystemFactory.h
      Linux/SystemFactory.cpp
      )

    source_group(Linux Linux/*.*)
ENDIF()

IF (APPLE)
   set(SYSTEMINFORMATION_PLUGIN_SOURCES ${SYSTEMINFORMATION_PLUGIN_SOURCES}
      Mac/SystemFactory.h
      Mac/SystemFactory.cpp
      )

    source_group(Mac Mac/*.*)
ENDIF(APPLE)

IF(CMAKE_YADOMS_PLATFORM STREQUAL "Raspberry")
   set(SYSTEMINFORMATION_PLUGIN_SOURCES ${SYSTEMINFORMATION_PLUGIN_SOURCES}
      # Add RaspberryPi Specific sources
      RaspberryPI/SystemFactory.h
      RaspberryPI/SystemFactory.cpp
      RaspberryPI/TemperatureSensor.cpp
      RaspberryPI/TemperatureSensor.h
      )
	
	source_group(RaspberryPI RaspberryPI/*.*)
ENDIF()

#########################################
## Place here all additional include directories
## (by default yadoms-shared and boost are included)
#########################################

IF(WIN32)
   set(SYSTEMINFORMATION_PLUGIN_INCLDIR
	   ${CMAKE_CURRENT_SOURCE_DIR}/Windows
           ${CMAKE_CURRENT_SOURCE_DIR}/Helpers
   )
ENDIF()
	
IF (UNIX AND NOT APPLE)
   set(SYSTEMINFORMATION_PLUGIN_INCLDIR
      ${CMAKE_CURRENT_SOURCE_DIR}/CommonLinux
      ${CMAKE_CURRENT_SOURCE_DIR}/Linux
      ${CMAKE_CURRENT_SOURCE_DIR}/Helpers
   )
ENDIF()

IF (APPLE)
   set(SYSTEMINFORMATION_PLUGIN_INCLDIR
      ${CMAKE_CURRENT_SOURCE_DIR}/Mac
      ${CMAKE_CURRENT_SOURCE_DIR}/Helpers
   )
ENDIF()


IF (CMAKE_YADOMS_PLATFORM STREQUAL "Raspberry")
   set(SYSTEMINFORMATION_PLUGIN_INCLDIR
      ${CMAKE_CURRENT_SOURCE_DIR}/CommonLinux
      ${CMAKE_CURRENT_SOURCE_DIR}/RaspberryPI
   )
ENDIF()	

#########################################
## Place here all additional libraries to link with
## (by default yadoms-shared and boost are included)
#########################################
set(SYSTEMINFORMATION_PLUGIN_LINK
)

#########################################
## Place here all additional compiler flags
## i.e. : for raspberry to link with wiringPi : use add_definition("-lwiringPi")
#########################################

#########################################
## Always leave these lines to configure plugin correctly
## even if FAKE_PLUGIN_INCLDIR or FAKE_PLUGIN_LINK are empty
#########################################

PLUGIN_SOURCES(SystemInformation ${SYSTEMINFORMATION_PLUGIN_SOURCES})
PLUGIN_INCLDIR(SystemInformation ${SYSTEMINFORMATION_PLUGIN_INCLDIR})
PLUGIN_LINK   (SystemInformation ${SYSTEMINFORMATION_PLUGIN_ADDITIONAL_LIB})

#########################################
## Post-build commands
## Copy files to build directory
## "package.json" and "icon.png" are needed, but you can add extra-files
#########################################
PLUGIN_POST_BUILD_COPY_FILE(SystemInformation package.json)
PLUGIN_POST_BUILD_COPY_FILE(SystemInformation icon.png)
PLUGIN_POST_BUILD_COPY_DIRECTORY(SystemInformation locales)