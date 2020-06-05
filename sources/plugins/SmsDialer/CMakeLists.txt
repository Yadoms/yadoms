# Check dependencies
include(findGammu.cmake)

if(GAMMU_FOUND)

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
      ISms.h
      PhoneException.hpp
      Sms.h
      Sms.cpp 
      SmsDialer.h
      SmsDialer.cpp 
      SmsDialerConfiguration.h
      SmsDialerConfiguration.cpp 
      SmsDialerFactory.h
      SmsDialerFactory.cpp 
   )

   #########################################
   ## Place here all additional groups (solution folder)
   #########################################
   source_group(sources ./*.*)
   
   #########################################
   ## Place here all additional include directories
   ## (by default yadoms-shared and boost are included)
   #########################################
   set(SMS_DIALER_ADDITIONAL_INC_DIR
      ${GAMMU_INCLUDE_DIRS}
   )

   #########################################
   ## Place here all additional libraries to link with
   ## (by default yadoms-shared and boost are included)
   #########################################
   if(WIN32 OR APPLE)
	   set(SMS_DIALER_ADDITIONAL_LIB
		   ${GAMMU_LIBRARIES}
	   )
   else()
	   set(SMS_DIALER_ADDITIONAL_LIB
		   ${GAMMU_LIBRARIES}
		   usb-1.0
		   bluetooth
		   glib-2.0
	   )
   endif()

   #########################################
   ## Always leave this three lines to configure plugin correctly
   ## even if SMS_DIALER_ADDITIONAL_INC_DIR or SMS_DIALER_ADDITIONAL_LIB are empty
   #########################################
   PLUGIN_SOURCES(smsDialer ${SMS_DIALER_SOURCES})
   PLUGIN_INCLDIR(smsDialer ${SMS_DIALER_ADDITIONAL_INC_DIR})
   PLUGIN_LINK(smsDialer ${SMS_DIALER_ADDITIONAL_LIB})
   
else()
   message(WARNING "smsDialer plugin is not available. Gammu is Missing")
endif()

