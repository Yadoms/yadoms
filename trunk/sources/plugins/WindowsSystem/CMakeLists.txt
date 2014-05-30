IF(WIN32)
	# include common plugin definitions
	include("../YadomsPluginsCommon.cmake")

	#########################################
	## Place here all the plugin source files
	#########################################
	set(WINDOWSSYSTEM_PLUGIN_SOURCES
		stdafx.h
		stdafx.cpp
		WindowsSystem.h
		WindowsSystem.cpp 
		WindowsSystemMemoryLoad.h
		WindowsSystemMemoryLoad.cpp
		WindowsSystemCPULoad.h
		WindowsSystemCPULoad.cpp
		WindowsSystemYadomsCPULoad.h
		WindowsSystemYadomsCPULoad.cpp
		WindowsSystemDiskUsage.h
		WindowsSystemDiskUsage.cpp
		WindowsSystemDisksList.h
		WindowsSystemDisksList.cpp
		ILoad.h
	)

	#########################################
	## Place here all additional include directories
	## (by default yadoms-shared and boost are included)
	#########################################
	set(WINDOWSSYSTEM_PLUGIN_INCLDIR
	)

	#########################################
	## Place here all additional libraries to link with
	## (by default yadoms-shared and boost are included)
	#########################################
	set(WINDOWSSYSTEM_PLUGIN_LINK
	)

	#########################################
	## Place here all additional compiler flags
	## i.e. : for raspberry to link with wiringPi : use add_definition("-lwiringPi")
	#########################################
	##add_definitions("-lwiringPi")

	#########################################
	## Always leave these lines to configure plugin correctly
	## even if FAKE_PLUGIN_INCLDIR or FAKE_PLUGIN_LINK are empty
	#########################################
	PLUGIN_SOURCES(WindowsSystem ${WINDOWSSYSTEM_PLUGIN_SOURCES})
	PLUGIN_INCLDIR(WindowsSystem ${WINDOWSSYSTEM_PLUGIN_ADDITIONAL_INC_DIR})
	PLUGIN_LINK   (WindowsSystem ${WINDOWSSYSTEM_PLUGIN_ADDITIONAL_LIB})

	#########################################
	## Post-build commands
	## Copy files to build directory
	## "package.json" and "icon.png" are needed, but you can add extra-files
	#########################################
	PLUGIN_POST_BUILD_COPY_FILE(WindowsSystem package.json)
	PLUGIN_POST_BUILD_COPY_FILE(WindowsSystem icon.png)
	PLUGIN_POST_BUILD_COPY_FILE(WindowsSystem locales/fr.json)
ENDIF()
