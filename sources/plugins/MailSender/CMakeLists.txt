# include common plugin definitions
include("../YadomsPluginsCommon.cmake")

#########################################
## Place here all the plugin source files
#########################################

set(MAILSENDER_SOURCES
   IMailBodyBuilder.h
   IMSConfiguration.h
   MSConfiguration.h
   MSConfiguration.cpp
   MailSender.h
   MailSender.cpp
   MailBodyBuilder.h
   MailBodyBuilder.cpp
   SecurityMode.h
   SecurityMode.cpp
)


#########################################
## Place here all additional groups (solution folder)
#########################################
source_group(sources ./*.*)

#########################################
## Place here all additional include directories
## (by default yadoms-shared and boost are included)
#########################################
set(MAILSENDER_ADDITIONAL_INC_DIR
   ${CURL_INCLUDE_DIR}
   ${CURLPP_INCLUDE_DIR}
)

#########################################
## Place here all additional libraries to link with
## (by default yadoms-shared and boost are included)
#########################################
set(MAILSENDER_ADDITIONAL_LIB
   ${CURL_LIBRARY}
   ${CURLPP_LIBRARY}
)

#########################################
## Always leave this three lines to configure plugin correctly
## even if MAILSENDER_ADDITIONAL_INC_DIR or MAILSENDER_ADDITIONAL_LIB are empty
#########################################
PLUGIN_SOURCES(MailSender ${MAILSENDER_SOURCES})
PLUGIN_INCLDIR(MailSender ${MAILSENDER_ADDITIONAL_INC_DIR})
PLUGIN_LINK(MailSender ${MAILSENDER_ADDITIONAL_LIB})

