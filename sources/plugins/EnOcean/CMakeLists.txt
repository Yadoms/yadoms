
# include common plugin definitions
include("../YadomsPluginsCommon.cmake")

#########################################
## Plugin needs Python to generate code
#########################################
set (PYTHON_REQUIRED_VERSION 2.7)
find_package(PythonInterp ${PYTHON_REQUIRED_VERSION} REQUIRED)


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
   message/types.cpp
   message/UTE_AnswerSendMessage.h
   message/UTE_AnswerSendMessage.cpp
   message/UTE_GigaConceptReversedAnswerSendMessage.h
   message/UTE_GigaConceptReversedAnswerSendMessage.cpp
   message/UTE_GigaConceptReversedReceivedMessage.h
   message/UTE_GigaConceptReversedReceivedMessage.cpp
   message/UTE_ReceivedMessage.h
   message/UTE_ReceivedMessage.cpp
   
   specificHistorizers/DimmerModeHistorizer.h
   specificHistorizers/DimmerModeHistorizer.cpp
   specificHistorizers/PilotWireHistorizer.h
   specificHistorizers/PilotWireHistorizer.cpp
   )
   
   
#########################################
## Place here all additional groups (solution folder)
#########################################
source_group(sources ./*.*)
source_group(sources\\message message/*.*)
source_group(sources\\specificHistorizers specificHistorizers/*.*)


#########################################
## Code generator configuration
#########################################
set(EEP_VERSION "2.6.5")

file(GLOB PYTHON_GENERATOR_FILES profiles/*.py)
file(GLOB IN_TRANSLATION_FILES locales.in/*.json)

set(ENOCEAN_GENERATED_OUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)

set(ENOCEAN_SOURCES ${ENOCEAN_SOURCES}
   profiles/manufacturers.xml
   profiles/eep${EEP_VERSION}.xml
   
   ${PYTHON_GENERATOR_FILES}

   profiles/bitsetHelpers.hpp
   profiles/commonHelpers.hpp
   
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
   profiles/hardCoded/Profile_D2_01_02.cpp
   profiles/hardCoded/Profile_D2_01_02.h
   profiles/hardCoded/Profile_D2_01_03.cpp
   profiles/hardCoded/Profile_D2_01_03.h
   profiles/hardCoded/Profile_D2_01_04.cpp
   profiles/hardCoded/Profile_D2_01_04.h
   profiles/hardCoded/Profile_D2_01_05.cpp
   profiles/hardCoded/Profile_D2_01_05.h
   profiles/hardCoded/Profile_D2_01_06.cpp
   profiles/hardCoded/Profile_D2_01_06.h
   profiles/hardCoded/Profile_D2_01_07.cpp
   profiles/hardCoded/Profile_D2_01_07.h
   profiles/hardCoded/Profile_D2_01_08.cpp
   profiles/hardCoded/Profile_D2_01_08.h
   profiles/hardCoded/Profile_D2_01_09.cpp
   profiles/hardCoded/Profile_D2_01_09.h
   profiles/hardCoded/Profile_D2_01_0A.cpp
   profiles/hardCoded/Profile_D2_01_0A.h
   profiles/hardCoded/Profile_D2_01_0B.cpp
   profiles/hardCoded/Profile_D2_01_0B.h
   profiles/hardCoded/Profile_D2_01_0C.cpp
   profiles/hardCoded/Profile_D2_01_0C.h
   profiles/hardCoded/Profile_D2_01_10.cpp
   profiles/hardCoded/Profile_D2_01_10.h
   profiles/hardCoded/Profile_D2_01_11.cpp
   profiles/hardCoded/Profile_D2_01_11.h
   profiles/hardCoded/Profile_D2_01_12.cpp
   profiles/hardCoded/Profile_D2_01_12.h

   profiles/hardCoded/Profile_F6_02_01.cpp
   profiles/hardCoded/Profile_F6_02_01.h
   profiles/hardCoded/Profile_F6_02_02.cpp
   profiles/hardCoded/Profile_F6_02_02.h
   
   package.in.json

   ${IN_TRANSLATION_FILES}
   )

add_custom_command(OUTPUT ${ENOCEAN_GENERATED_OUT_DIR}/manufacturers.h
                          ${ENOCEAN_GENERATED_OUT_DIR}/manufacturers.cpp
                   COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/profiles/generateManufacturers.py 
                           ${CMAKE_CURRENT_SOURCE_DIR}/profiles/manufacturers.xml
                           ${ENOCEAN_GENERATED_OUT_DIR}
                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/profiles/manufacturers.xml
                           ${PYTHON_GENERATOR_FILES} 
                   COMMENT "Generating Manufacturers source code...")

add_custom_command(OUTPUT ${ENOCEAN_GENERATED_OUT_DIR}/package.json
                          ${ENOCEAN_GENERATED_OUT_DIR}/profiles/eep.cpp
                          ${ENOCEAN_GENERATED_OUT_DIR}/profiles/eep.h
                          ${ENOCEAN_GENERATED_OUT_DIR}/profiles/C1BSTelegram.cpp
                          ${ENOCEAN_GENERATED_OUT_DIR}/profiles/C1BSTelegram.h
                          ${ENOCEAN_GENERATED_OUT_DIR}/profiles/C4BSTelegram.cpp
                          ${ENOCEAN_GENERATED_OUT_DIR}/profiles/C4BSTelegram.h
                          ${ENOCEAN_GENERATED_OUT_DIR}/profiles/CRPSTelegram.cpp
                          ${ENOCEAN_GENERATED_OUT_DIR}/profiles/CRPSTelegram.h
                          ${ENOCEAN_GENERATED_OUT_DIR}/profiles/CVLDTelegram.cpp
                          ${ENOCEAN_GENERATED_OUT_DIR}/profiles/CVLDTelegram.h
                   COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/profiles/generateEep.py
                          ${CMAKE_CURRENT_SOURCE_DIR}/profiles/eep${EEP_VERSION}.xml
                          ${ENOCEAN_GENERATED_OUT_DIR}/profiles
                          ${CMAKE_CURRENT_SOURCE_DIR}/package.in.json
                          ${ENOCEAN_GENERATED_OUT_DIR}/package.json
                          ${CMAKE_CURRENT_SOURCE_DIR}/locales.in
                          ${ENOCEAN_GENERATED_OUT_DIR}/locales
                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/profiles/eep${EEP_VERSION}.xml
                           ${CMAKE_CURRENT_SOURCE_DIR}/package.in.json
                           ${PYTHON_GENERATOR_FILES}
                           ${IN_TRANSLATION_FILES}
                   COMMENT "Generating EEP profiles source code...")
                   
                   
set(ENOCEAN_GENERATED_SOURCES
   ${ENOCEAN_GENERATED_OUT_DIR}/package.json
   ${ENOCEAN_GENERATED_OUT_DIR}/manufacturers.cpp
   ${ENOCEAN_GENERATED_OUT_DIR}/manufacturers.h
   ${ENOCEAN_GENERATED_OUT_DIR}/profiles/eep.cpp
   ${ENOCEAN_GENERATED_OUT_DIR}/profiles/eep.h
   ${ENOCEAN_GENERATED_OUT_DIR}/profiles/C1BSTelegram.cpp
   ${ENOCEAN_GENERATED_OUT_DIR}/profiles/C1BSTelegram.h
   ${ENOCEAN_GENERATED_OUT_DIR}/profiles/C4BSTelegram.cpp
   ${ENOCEAN_GENERATED_OUT_DIR}/profiles/C4BSTelegram.h
   ${ENOCEAN_GENERATED_OUT_DIR}/profiles/CRPSTelegram.cpp
   ${ENOCEAN_GENERATED_OUT_DIR}/profiles/CRPSTelegram.h
   ${ENOCEAN_GENERATED_OUT_DIR}/profiles/CVLDTelegram.cpp
   ${ENOCEAN_GENERATED_OUT_DIR}/profiles/CVLDTelegram.h
   )
set_source_files_properties(${ENOCEAN_GENERATED_SOURCES} PROPERTIES GENERATED TRUE)

set(ENOCEAN_SOURCES ${ENOCEAN_SOURCES} ${ENOCEAN_GENERATED_SOURCES})


source_group(profiles profiles/*.*)
source_group(profiles\\hardCoded profiles/hardCoded/*.*)
source_group(locales\\in locales.in/*.*)
source_group(generated ${ENOCEAN_GENERATED_OUT_DIR}/*.*)
source_group(generated\\profiles ${ENOCEAN_GENERATED_OUT_DIR}/profiles/*.*)

#########################################
## Place here all additional include directories
## (by default yadoms-shared and boost are included)
#########################################
set(ENOCEAN_ADDITIONAL_INC_DIR
   ${ENOCEAN_GENERATED_OUT_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}
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
# Workaround to tell CMake to not looking for package.json as it will be generated at compile time
set_source_files_properties(package.json PROPERTIES GENERATED TRUE)
set(PACKAGE_JSON_FILE package.in.json)
PLUGIN_SOURCES(enOcean ${ENOCEAN_SOURCES})
PLUGIN_INCLDIR(enOcean ${ENOCEAN_ADDITIONAL_INC_DIR})
PLUGIN_LINK(enOcean ${ENOCEAN_ADDITIONAL_LIB})

#########################################
## Post-build commands
## Copy files to build directory
## "package.json" and "icon.png" are needed, but you can add extra-files
#########################################
PLUGIN_POST_BUILD_COPY_FILE(enOcean ${ENOCEAN_GENERATED_OUT_DIR}/package.json)
PLUGIN_POST_BUILD_COPY_FILE(enOcean icon.png)
PLUGIN_POST_BUILD_COPY_DIRECTORY(enOcean ${ENOCEAN_GENERATED_OUT_DIR}/locales)
