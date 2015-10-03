if(CMAKE_COMPILER_IS_GNUCXX OR RASPBERRYPI)
   # Check dependencies
   include(findLibQuickMail.cmake)

   if(LIBQUICKMAIL_FOUND)

	#########################################
	## Place here all the plugin source files
	#########################################

	set(MAILSENDER_SOURCES
	   IMSConfiguration.h
	   MSConfiguration.h
	   MSConfiguration.cpp
	   MailSender.h
	   MailSender.cpp
	)

	#########################################
	## Place here all additional include directories
	## (by default yadoms-shared and boost are included)
	#########################################
        set(MAILSENDER_ADDITIONAL_INC_DIR
         ${LIBQUICKMAIL_INCLUDE_DIRS}
        )

	#########################################
	## Place here all additional libraries to link with
	## (by default yadoms-shared and boost are included)
	#########################################
        set(MAILSENDER_ADDITIONAL_LIB
         ${LIBQUICKMAIL_LIBRARIES}
        )

	#########################################
	## Always leave this three lines to configure plugin correctly
	## even if MAILSENDER_ADDITIONAL_INC_DIR or MAILSENDER_ADDITIONAL_LIB are empty
	#########################################
	PLUGIN_SOURCES(MailSender ${MAILSENDER_SOURCES})
	PLUGIN_INCLDIR(MailSender ${MAILSENDER_ADDITIONAL_INC_DIR})
	PLUGIN_LINK(MailSender ${MAILSENDER_ADDITIONAL_LIB})

	#########################################
	## Post-build commands
	## Copy files to build directory
	## "package.json" and "icon.png" are needed, but you can add extra-files
	#########################################
	PLUGIN_POST_BUILD_COPY_FILE(MailSender package.json)
	PLUGIN_POST_BUILD_COPY_FILE(MailSender icon.png)
	PLUGIN_POST_BUILD_COPY_DIRECTORY(MailSender locales)
   else()
      message(WARNING "MailSender plugin is not available. LibQuickMail is Missing")
   endif()
endif()
