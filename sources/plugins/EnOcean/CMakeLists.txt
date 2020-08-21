
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
   Configuration.h
   Configuration.cpp
   DeviceConfigurationHelper.h
   DeviceConfigurationHelper.cpp
   EnOcean.h
   EnOcean.cpp
   Factory.h
   Factory.cpp
   IConfiguration.h
   IPairingHelper.h
   IMessageHandler.h
   MessageHandler.h
   MessageHandler.cpp
   PairingHelper.h
   PairingHelper.cpp
   ProfileHelper.h
   ProfileHelper.cpp
   ReceiveBufferHandler.h
   ReceiveBufferHandler.cpp
   
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
   
   specificHistorizers/BlindLockingMode.h
   specificHistorizers/BlindLockingMode.cpp
   specificHistorizers/ConcentrationPpm.h
   specificHistorizers/ConcentrationPpm.cpp
   specificHistorizers/ConcentrationGramPerMeterCube.h
   specificHistorizers/ConcentrationGramPerMeterCube.cpp
   specificHistorizers/CountPerSecondRadioActivity.h
   specificHistorizers/CountPerSecondRadioActivity.cpp
   specificHistorizers/DimmerModeHistorizer.h
   specificHistorizers/DimmerModeHistorizer.cpp
   specificHistorizers/Fan4Speeds.h
   specificHistorizers/Fan4Speeds.cpp
   specificHistorizers/Fan6Speeds.h
   specificHistorizers/Fan6Speeds.cpp
   specificHistorizers/MassRadioActivity.h
   specificHistorizers/MassRadioActivity.cpp
   specificHistorizers/MechanicalHandleMovement.h
   specificHistorizers/MechanicalHandleMovement.cpp
   specificHistorizers/PilotWireHistorizer.h
   specificHistorizers/PilotWireHistorizer.cpp
   specificHistorizers/SetPoint.h
   specificHistorizers/SetPoint.cpp
   specificHistorizers/SieverPerSecondRadioActivity.h
   specificHistorizers/SieverPerSecondRadioActivity.cpp
   specificHistorizers/VolatileOrganicCompound.h
   specificHistorizers/VolatileOrganicCompound.cpp
   specificHistorizers/VolumicRadioActivity.h
   specificHistorizers/VolumicRadioActivity.cpp
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
set(EEP_VERSION "268")

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
   
   profiles/hardCoded/Profile_A5_04_01.cpp
   profiles/hardCoded/Profile_A5_04_01.h
   profiles/hardCoded/Profile_A5_04_02.cpp
   profiles/hardCoded/Profile_A5_04_02.h
   profiles/hardCoded/Profile_A5_04_03.cpp
   profiles/hardCoded/Profile_A5_04_03.h
   profiles/hardCoded/Profile_A5_04_04.cpp
   profiles/hardCoded/Profile_A5_04_04.h
   
   profiles/hardCoded/Profile_A5_09_02.cpp
   profiles/hardCoded/Profile_A5_09_02.h
   profiles/hardCoded/Profile_A5_09_04.cpp
   profiles/hardCoded/Profile_A5_09_04.h
   profiles/hardCoded/Profile_A5_09_05.cpp
   profiles/hardCoded/Profile_A5_09_05.h
   profiles/hardCoded/Profile_A5_09_06.cpp
   profiles/hardCoded/Profile_A5_09_06.h
   profiles/hardCoded/Profile_A5_09_07.cpp
   profiles/hardCoded/Profile_A5_09_07.h
   profiles/hardCoded/Profile_A5_09_08.cpp
   profiles/hardCoded/Profile_A5_09_08.h
   profiles/hardCoded/Profile_A5_09_09.cpp
   profiles/hardCoded/Profile_A5_09_09.h
   profiles/hardCoded/Profile_A5_09_0A.cpp
   profiles/hardCoded/Profile_A5_09_0A.h
   profiles/hardCoded/Profile_A5_09_0B.cpp
   profiles/hardCoded/Profile_A5_09_0B.h
   profiles/hardCoded/Profile_A5_09_0C.cpp
   profiles/hardCoded/Profile_A5_09_0C.h

   profiles/hardCoded/Profile_A5_10_01.cpp
   profiles/hardCoded/Profile_A5_10_01.h
   profiles/hardCoded/Profile_A5_10_02.cpp
   profiles/hardCoded/Profile_A5_10_02.h
   profiles/hardCoded/Profile_A5_10_03.cpp
   profiles/hardCoded/Profile_A5_10_03.h
   profiles/hardCoded/Profile_A5_10_04.cpp
   profiles/hardCoded/Profile_A5_10_04.h
   profiles/hardCoded/Profile_A5_10_05.cpp
   profiles/hardCoded/Profile_A5_10_05.h
   profiles/hardCoded/Profile_A5_10_06.cpp
   profiles/hardCoded/Profile_A5_10_06.h
   profiles/hardCoded/Profile_A5_10_07.cpp
   profiles/hardCoded/Profile_A5_10_07.h
   profiles/hardCoded/Profile_A5_10_08.cpp
   profiles/hardCoded/Profile_A5_10_08.h
   profiles/hardCoded/Profile_A5_10_09.cpp
   profiles/hardCoded/Profile_A5_10_09.h
   profiles/hardCoded/Profile_A5_10_0A.cpp
   profiles/hardCoded/Profile_A5_10_0A.h
   profiles/hardCoded/Profile_A5_10_0B.cpp
   profiles/hardCoded/Profile_A5_10_0B.h
   profiles/hardCoded/Profile_A5_10_0C.cpp
   profiles/hardCoded/Profile_A5_10_0C.h
   profiles/hardCoded/Profile_A5_10_0D.cpp
   profiles/hardCoded/Profile_A5_10_0D.h
   profiles/hardCoded/Profile_A5_10_10.cpp
   profiles/hardCoded/Profile_A5_10_10.h
   profiles/hardCoded/Profile_A5_10_11.cpp
   profiles/hardCoded/Profile_A5_10_11.h
   profiles/hardCoded/Profile_A5_10_12.cpp
   profiles/hardCoded/Profile_A5_10_12.h
   profiles/hardCoded/Profile_A5_10_13.cpp
   profiles/hardCoded/Profile_A5_10_13.h
   profiles/hardCoded/Profile_A5_10_14.cpp
   profiles/hardCoded/Profile_A5_10_14.h
   profiles/hardCoded/Profile_A5_10_15.cpp
   profiles/hardCoded/Profile_A5_10_15.h
   profiles/hardCoded/Profile_A5_10_16.cpp
   profiles/hardCoded/Profile_A5_10_16.h
   profiles/hardCoded/Profile_A5_10_17.cpp
   profiles/hardCoded/Profile_A5_10_17.h
   profiles/hardCoded/Profile_A5_10_18.cpp
   profiles/hardCoded/Profile_A5_10_18.h

   profiles/hardCoded/Profile_A5_12_Common.cpp
   profiles/hardCoded/Profile_A5_12_Common.h
   profiles/hardCoded/Profile_A5_12_00.cpp
   profiles/hardCoded/Profile_A5_12_00.h
   profiles/hardCoded/Profile_A5_12_01.cpp
   profiles/hardCoded/Profile_A5_12_01.h
   profiles/hardCoded/Profile_A5_12_02.cpp
   profiles/hardCoded/Profile_A5_12_02.h
   profiles/hardCoded/Profile_A5_12_03.cpp
   profiles/hardCoded/Profile_A5_12_03.h
   profiles/hardCoded/Profile_A5_12_04.cpp
   profiles/hardCoded/Profile_A5_12_04.h
   profiles/hardCoded/Profile_A5_12_05.cpp
   profiles/hardCoded/Profile_A5_12_05.h
   profiles/hardCoded/Profile_A5_12_10.cpp
   profiles/hardCoded/Profile_A5_12_10.h
   
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

   profiles/hardCoded/Profile_D2_05_Common.cpp
   profiles/hardCoded/Profile_D2_05_Common.h
   profiles/hardCoded/Profile_D2_05_00.cpp
   profiles/hardCoded/Profile_D2_05_00.h

   profiles/hardCoded/Profile_F6_02_01.cpp
   profiles/hardCoded/Profile_F6_02_01.h
   profiles/hardCoded/Profile_F6_02_02.cpp
   profiles/hardCoded/Profile_F6_02_02.h
   profiles/hardCoded/Profile_F6_02_03.cpp
   profiles/hardCoded/Profile_F6_02_03.h

   profiles/hardCoded/Profile_F6_03_01.cpp
   profiles/hardCoded/Profile_F6_03_01.h
   profiles/hardCoded/Profile_F6_03_02.cpp
   profiles/hardCoded/Profile_F6_03_02.h

   profiles/hardCoded/Profile_F6_04_01.cpp
   profiles/hardCoded/Profile_F6_04_01.h
   profiles/hardCoded/Profile_F6_04_02.cpp
   profiles/hardCoded/Profile_F6_04_02.h

   profiles/hardCoded/Profile_F6_10_00.cpp
   profiles/hardCoded/Profile_F6_10_00.h
   profiles/hardCoded/Profile_F6_10_01.cpp
   profiles/hardCoded/Profile_F6_10_01.h

   profiles/hardCoded/Profile_F6_05_00.cpp
   profiles/hardCoded/Profile_F6_05_00.h
   profiles/hardCoded/Profile_F6_05_01.cpp
   profiles/hardCoded/Profile_F6_05_01.h
   profiles/hardCoded/Profile_F6_05_02.cpp
   profiles/hardCoded/Profile_F6_05_02.h
   
   package.in.json

   ${IN_TRANSLATION_FILES}
   )                   
                   
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

PLUGIN_SOURCES(enOcean ${ENOCEAN_SOURCES})

   
# package.in.json is firstly patched at makefile generation time to set plugin version number.
# Set result file in PACKAGE_JSON_IN_FILE.
set(PACKAGE_JSON_IN_FILE ${CMAKE_CURRENT_BINARY_DIR}/package.json)
USE_SPECIFIC_PACKAGE_JSON(${ENOCEAN_GENERATED_OUT_DIR}/package.json)

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
                          ${PACKAGE_JSON_IN_FILE}
                          ${ENOCEAN_GENERATED_OUT_DIR}/package.json
                          ${CMAKE_CURRENT_SOURCE_DIR}/locales.in
                          ${ENOCEAN_GENERATED_OUT_DIR}/locales
                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/profiles/eep${EEP_VERSION}.xml
                           ${PACKAGE_JSON_IN_FILE}
                           ${PYTHON_GENERATOR_FILES}
                           ${IN_TRANSLATION_FILES}
                   COMMENT "Generating EEP profiles source code...")

USE_SPECIFIC_LOCALES_DIRECTORY(${ENOCEAN_GENERATED_OUT_DIR}/locales)                  

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
PLUGIN_INCLDIR(enOcean ${ENOCEAN_ADDITIONAL_INC_DIR})
PLUGIN_LINK(enOcean ${ENOCEAN_ADDITIONAL_LIB})

