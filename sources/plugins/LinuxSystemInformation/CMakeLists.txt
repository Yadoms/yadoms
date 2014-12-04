IF(UNIX AND NOT APPLE AND NOT RASPBERRYPI)
	# include common plugin definitions
	include("../YadomsPluginsCommon.cmake")

	#########################################
	## Place here all the plugin source files
	#########################################
	set(LINUXSYSTEMINFORMATION_PLUGIN_SOURCES
		stdafx.h
		stdafx.cpp
		LinuxSystemInformation.h
		LinuxSystemInformation.cpp 
		LinuxSystemMemoryLoad.h
		LinuxSystemMemoryLoad.cpp
		LinuxSystemCPULoad.h
		LinuxSystemCPULoad.cpp
		LinuxSystemYadomsCPULoad.h
		LinuxSystemYadomsCPULoad.cpp
		LinuxSystemDiskUsage.h
		LinuxSystemDiskUsage.cpp
		LinuxSystemDisksList.h
		LinuxSystemDisksList.cpp
		ILoad.h
	)

	#########################################
	## Place here all additional include directories
	## (by default yadoms-shared and boost are included)
	#########################################
	set(LINUXSYSTEMINFORMATION_PLUGIN_INCLDIR
	)

	#########################################
	## Place here all additional libraries to link with
	## (by default yadoms-shared and boost are included)
	#########################################
	set(LINUXSYSTEMINFORMATION_PLUGIN_LINK
	)

	#########################################
	## Place here all additional compiler flags
	## i.e. : for raspberry to link with wiringPi : use add_definition("-lwiringPi")
	#########################################

	#########################################
	## Always leave these lines to configure plugin correctly
	## even if FAKE_PLUGIN_INCLDIR or FAKE_PLUGIN_LINK are empty
	#########################################
	PLUGIN_SOURCES(LinuxSystemInformation ${LINUXSYSTEMINFORMATION_PLUGIN_SOURCES})
	PLUGIN_INCLDIR(LinuxSystemInformation ${LINUXSYSTEMINFORMATION_PLUGIN_ADDITIONAL_INC_DIR})
	PLUGIN_LINK   (LinuxSystemInformation ${LINUXSYSTEMINFORMATION_PLUGIN_ADDITIONAL_LIB})

	#########################################
	## Post-build commands
	## Copy files to build directory
	## "package.json" and "icon.png" are needed, but you can add extra-files
	#########################################
	PLUGIN_POST_BUILD_COPY_FILE(LinuxSystemInformation package.json)
	PLUGIN_POST_BUILD_COPY_FILE(LinuxSystemInformation icon.png)
	PLUGIN_POST_BUILD_COPY_FILE(LinuxSystemInformation locales/fr.json)
ENDIF()
