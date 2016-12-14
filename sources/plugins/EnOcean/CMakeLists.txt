
# include common plugin definitions
include("../YadomsPluginsCommon.cmake")

#########################################
## Place here all the plugin source files
#########################################
set(ENOCEAN_SOURCES
   stdafx.h
   stdafx.cpp

   4BSTeachinVariant2.h
   4BSTeachinVariant2.cpp
   IConfiguration.h
   DeviceConfigurationHelper.h
   DeviceConfigurationHelper.cpp
   EnOcean.h
   EnOcean.cpp
   ReceiveBufferHandler.h
   ReceiveBufferHandler.cpp
   Configuration.h
   Configuration.cpp
   Factory.h
   Factory.cpp
   IMessageHandler.h
   MessageHandler.h
   MessageHandler.cpp
   ProfileHelper.h
   ProfileHelper.cpp
   
   message/CommonCommandSendMessage.h
   message/CommonCommandSendMessage.cpp
   message/DongleVersionResponseReceivedMessage.h
   message/DongleVersionResponseReceivedMessage.cpp
   message/Crc8.h
   message/Crc8.cpp
   message/Esp3ReceivedPacket.h
   message/Esp3ReceivedPacket.cpp
   message/Esp3SendPacket.h
   message/Esp3SendPacket.cpp
   message/Header.h
   message/Header.cpp
   message/RadioErp1ReceivedMessage.h
   message/RadioErp1ReceivedMessage.cpp
   message/RadioErp1SendMessage.h
   message/RadioErp1SendMessage.cpp
   message/ResponseReceivedMessage.h
   message/ResponseReceivedMessage.cpp
   message/types.h
   message/UTE_AnswerSendMessage.h
   message/UTE_AnswerSendMessage.cpp
   message/UTE_GigaConceptReversedReceivedMessage.h
   message/UTE_GigaConceptReversedReceivedMessage.cpp
   message/UTE_ReceivedMessage.h
   message/UTE_ReceivedMessage.cpp
   )
   
   
source_group(message message/*.*)


#########################################
## Code generator configuration
#########################################
set(EEP_VERSION "2.6.5")
file(GLOB PYTHON_GENERATOR_FILES profiles/*.py)
file(GLOB IN_TRANSLATION_FILES locales.in/*.json)
source_group(locales\\in locales.in/*.*)

set(ENOCEAN_SOURCES ${ENOCEAN_SOURCES}
   profiles/manufacturers.xml
   profiles/eep${EEP_VERSION}.xml
   
   ${PYTHON_GENERATOR_FILES}

   profiles/bitsetHelpers.hpp
   profiles/commonHelpers.hpp

   profiles/generated-manufacturers.cpp
   profiles/generated-manufacturers.h
   profiles/generated-eep.cpp
   profiles/generated-eep.h
   
   profiles/IRorg.h
   profiles/IFunc.h
   profiles/IType.h
   
   profiles/hardCoded/Profile_D2_01_Common.cpp
   profiles/hardCoded/Profile_D2_01_Common.h
   profiles/hardCoded/Profile_D2_01_0D.cpp
   profiles/hardCoded/Profile_D2_01_0D.h
   profiles/hardCoded/Profile_D2_01_0E.cpp
   profiles/hardCoded/Profile_D2_01_0E.h
   profiles/hardCoded/Profile_D2_01_0F.cpp
   profiles/hardCoded/Profile_D2_01_0F.h
   profiles/hardCoded/Profile_D2_01_00.cpp
   profiles/hardCoded/Profile_D2_01_00.h
   profiles/hardCoded/Profile_D2_01_01.cpp
   profiles/hardCoded/Profile_D2_01_01.h
   profiles/hardCoded/Profile_D2_01_12.cpp
   profiles/hardCoded/Profile_D2_01_12.h

   profiles/hardCoded/Profile_F6_02_01.cpp
   profiles/hardCoded/Profile_F6_02_01.h
   profiles/hardCoded/Profile_F6_02_02.cpp
   profiles/hardCoded/Profile_F6_02_02.h
   
   package.in.json
   package.json

   ${IN_TRANSLATION_FILES}
   )
source_group(profiles profiles/*.*)
source_group(profiles\\hardCoded profiles/hardCoded/*.*)

add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/profiles/generated-manufacturers.h
                          ${CMAKE_CURRENT_SOURCE_DIR}/profiles/generated-manufacturers.cpp
                   COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/profiles/generateManufacturers.py ${CMAKE_CURRENT_SOURCE_DIR}/profiles/manufacturers.xml ${CMAKE_CURRENT_SOURCE_DIR}/profiles/generated-manufacturers.h ${CMAKE_CURRENT_SOURCE_DIR}/profiles/generated-manufacturers.cpp
                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/profiles/manufacturers.xml
                           ${PYTHON_GENERATOR_FILES} 
                   COMMENT "Generating Manufacturers source code...")

add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/profiles/generated-eep.h
                          ${CMAKE_CURRENT_SOURCE_DIR}/profiles/generated-eep.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/package.json
                   COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/profiles/generateEep.py ${CMAKE_CURRENT_SOURCE_DIR}/profiles/eep${EEP_VERSION}.xml ${CMAKE_CURRENT_SOURCE_DIR}/profiles/generated-eep.h ${CMAKE_CURRENT_SOURCE_DIR}/profiles/generated-eep.cpp ${CMAKE_CURRENT_SOURCE_DIR}/package.in.json ${CMAKE_CURRENT_SOURCE_DIR}/package.json  ${CMAKE_CURRENT_SOURCE_DIR}/locales
                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/profiles/eep${EEP_VERSION}.xml
                           ${CMAKE_CURRENT_SOURCE_DIR}/package.in.json
                           ${PYTHON_GENERATOR_FILES}
						   ${IN_TRANSLATION_FILES}
                   COMMENT "Generating EEP profiles source code...")


#########################################
## Place here all additional include directories
## (by default yadoms-shared and boost are included)
#########################################
set(ENOCEAN_ADDITIONAL_INC_DIR
)

#########################################
## Place here all additional libraries to link with
## (by default yadoms-shared and boost are included)
#########################################
set(ENOCEAN_ADDITIONAL_LIB
)

#########################################
## Always leave this three lines to configure plugin correctly
## even if ENOCEAN_ADDITIONAL_INC_DIR or ENOCEAN_ADDITIONAL_LIB are empty
#########################################
PLUGIN_SOURCES(enOcean ${ENOCEAN_SOURCES})
PLUGIN_INCLDIR(enOcean ${ENOCEAN_ADDITIONAL_INC_DIR})
PLUGIN_LINK(enOcean ${ENOCEAN_ADDITIONAL_LIB})

#########################################
## Post-build commands
## Copy files to build directory
## "package.json" and "icon.png" are needed, but you can add extra-files
#########################################
PLUGIN_POST_BUILD_COPY_FILE(enOcean package.json)
PLUGIN_POST_BUILD_COPY_FILE(enOcean icon.png)
PLUGIN_POST_BUILD_COPY_DIRECTORY(enOcean locales)
