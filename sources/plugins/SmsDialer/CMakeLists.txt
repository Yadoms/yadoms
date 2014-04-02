# Check dependencies
#TODO#include("CMakeListsUserConfig.txt")
include(findGammu.cmake)

#TODO
# find_package (Gammu)
# if (GAMMU_FOUND)
   # message("Gammu found")
# else (GAMMU_FOUND)
   # message("### Gammu NOT found ###")
# endif (GAMMU_FOUND)


IF(EXISTS ${GAMMU_ROOT}/libgammu/Debug/Gammu.lib) #TODO : le Debug me gêne...

	# include common plugin definitions
	include("../YadomsPluginsCommon.cmake")

	#########################################
	## Place here all the plugin source files
	#########################################
	set(SMS_DIALER_SOURCES
		stdafx.h
		stdafx.cpp
		GammuPhone.h
		GammuPhone.cpp
		GammuPhoneConnection.h
		GammuPhoneConnection.cpp
		IPhone.h
		ISmsDialerConfiguration.h
		PhoneException.hpp
		SmsDialer.h
		SmsDialer.cpp 
		SmsDialerConfiguration.h
		SmsDialerConfiguration.cpp 
		SmsDialerFactory.h
		SmsDialerFactory.cpp 
	)

	#########################################
	## Place here all additional include directories
	## (by default yadoms-shared and boost are included)
	#########################################
	set(SMS_DIALER_ADDITIONAL_INC_DIR
		${GAMMU_ROOT}/include
	)

	#########################################
	## Place here all additional libraries to link with
	## (by default yadoms-shared and boost are included)
	#########################################
	#toto gérer les différentes config et PF
	set(SMS_DIALER_ADDITIONAL_LIB
		${GAMMU_ROOT}/libgammu/Debug/Gammu.lib
		${GAMMU_ROOT}/helper/Debug/string.lib
		${GAMMU_ROOT}/helper/Debug/win32dirent.lib)

	#########################################
	## Always leave this three lines to configure plugin correctly
	## even if SMS_DIALER_ADDITIONAL_INC_DIR or SMS_DIALER_ADDITIONAL_LIB are empty
	#########################################
	PLUGIN_SOURCES(smsDialer ${SMS_DIALER_SOURCES})
	PLUGIN_INCLDIR(smsDialer ${SMS_DIALER_ADDITIONAL_INC_DIR})
	PLUGIN_LINK(smsDialer ${SMS_DIALER_ADDITIONAL_LIB})

	#########################################
	## Post-build commands
	## Copy files to build directory
	## "package.json" and "icon.png" are needed, but you can add extra-files
	#########################################
	PLUGIN_POST_BUILD_COPY_FILE(smsDialer package.json)
	PLUGIN_POST_BUILD_COPY_FILE(smsDialer icon.png)
	PLUGIN_POST_BUILD_COPY_FILE(smsDialer fr.json)
ELSE()
	message(WARNING "SMS dialer plugin is not available. Gammu is Mssing")
ENDIF()