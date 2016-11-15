
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
   IEnOceanConfiguration.h
   Device.h
   Device.cpp
   EnOcean.h
   EnOcean.cpp
   EnOceanReceiveBufferHandler.h
   EnOceanReceiveBufferHandler.cpp
   EnOceanConfiguration.h
   EnOceanConfiguration.cpp
   EnOceanFactory.h
   EnOceanFactory.cpp
   ProfileHelper.h
   ProfileHelper.cpp
   
   message/Crc8.h
   message/Crc8.cpp
   message/ReceivedMessage.h
   message/ReceivedMessage.cpp
   message/SendMessage.h
   message/SendMessage.cpp
   message/types.h
   )
   
   
source_group(message message/*.*)


#########################################
## Code generator configuration
#########################################
set(EEP_VERSION "2.6.5")
file(GLOB PYTHON_GENERATOR_FILES enOceanDescriptors/*.py)
file(GLOB IN_TRANSLATION_FILES locales.in/*.json)
source_group(locales\\in locales.in/*.*)

set(ENOCEAN_SOURCES ${ENOCEAN_SOURCES}
   enOceanDescriptors/manufacturers.xml
   enOceanDescriptors/eep${EEP_VERSION}.xml
   
   ${PYTHON_GENERATOR_FILES}

   enOceanDescriptors/bitsetHelpers.hpp
   enOceanDescriptors/commonHelpers.hpp

   enOceanDescriptors/generated-manufacturers.cpp
   enOceanDescriptors/generated-manufacturers.h
   enOceanDescriptors/generated-eep.cpp
   enOceanDescriptors/generated-eep.h
   
   enOceanDescriptors/IRorg.h
   enOceanDescriptors/IFunc.h
   enOceanDescriptors/IType.h
   
   enOceanDescriptors/hardCoded/VLD_Electronic_switches_and_dimmers_with_Energy_Measurement_and_Local_Control_0x0D.cpp
   enOceanDescriptors/hardCoded/VLD_Electronic_switches_and_dimmers_with_Energy_Measurement_and_Local_Control_0x0D.h
   enOceanDescriptors/hardCoded/VLD_Electronic_switches_and_dimmers_with_Energy_Measurement_and_Local_Control_0x0E.cpp
   enOceanDescriptors/hardCoded/VLD_Electronic_switches_and_dimmers_with_Energy_Measurement_and_Local_Control_0x0E.h
   enOceanDescriptors/hardCoded/VLD_Electronic_switches_and_dimmers_with_Energy_Measurement_and_Local_Control_0x0F.cpp
   enOceanDescriptors/hardCoded/VLD_Electronic_switches_and_dimmers_with_Energy_Measurement_and_Local_Control_0x0F.h
   enOceanDescriptors/hardCoded/VLD_Electronic_switches_and_dimmers_with_Energy_Measurement_and_Local_Control_0x12.cpp
   enOceanDescriptors/hardCoded/VLD_Electronic_switches_and_dimmers_with_Energy_Measurement_and_Local_Control_0x12.h
   
   package.in.json
   package.json

   ${IN_TRANSLATION_FILES}
   )
source_group(enOceanDescriptors enOceanDescriptors/*.*)
source_group(enOceanDescriptors\\hardCoded enOceanDescriptors/hardCoded/*.*)

add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/enOceanDescriptors/generated-manufacturers.h
                          ${CMAKE_CURRENT_SOURCE_DIR}/enOceanDescriptors/generated-manufacturers.cpp
                   COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/enOceanDescriptors/generateManufacturers.py ${CMAKE_CURRENT_SOURCE_DIR}/enOceanDescriptors/manufacturers.xml ${CMAKE_CURRENT_SOURCE_DIR}/enOceanDescriptors/generated-manufacturers.h ${CMAKE_CURRENT_SOURCE_DIR}/enOceanDescriptors/generated-manufacturers.cpp
                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/enOceanDescriptors/manufacturers.xml
                           ${PYTHON_GENERATOR_FILES} 
                   COMMENT "Generating Manufacturers source code...")

add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/enOceanDescriptors/generated-eep.h
                          ${CMAKE_CURRENT_SOURCE_DIR}/enOceanDescriptors/generated-eep.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/package.json
                   COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/enOceanDescriptors/generateEep.py ${CMAKE_CURRENT_SOURCE_DIR}/enOceanDescriptors/eep${EEP_VERSION}.xml ${CMAKE_CURRENT_SOURCE_DIR}/enOceanDescriptors/generated-eep.h ${CMAKE_CURRENT_SOURCE_DIR}/enOceanDescriptors/generated-eep.cpp ${CMAKE_CURRENT_SOURCE_DIR}/package.in.json ${CMAKE_CURRENT_SOURCE_DIR}/package.json  ${CMAKE_CURRENT_SOURCE_DIR}/locales
                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/enOceanDescriptors/eep${EEP_VERSION}.xml
                           ${CMAKE_CURRENT_SOURCE_DIR}/package.in.json
                           ${PYTHON_GENERATOR_FILES}
						   ${IN_TRANSLATION_FILES}
                   COMMENT "Generating EEP profiles source code...")


#########################################
## Place here all additional include directories
## (by default yadoms-shared and boost are included)
#########################################
set(ENOCEAN_ADDITIONAL_INC_DIR
   ${CMAKE_CURRENT_BINARY_DIR}/enOceanDescriptors
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
PLUGIN_SOURCES(dev-enOcean ${ENOCEAN_SOURCES})
PLUGIN_INCLDIR(dev-enOcean ${ENOCEAN_ADDITIONAL_INC_DIR})
PLUGIN_LINK(dev-enOcean ${ENOCEAN_ADDITIONAL_LIB})

#########################################
## Post-build commands
## Copy files to build directory
## "package.json" and "icon.png" are needed, but you can add extra-files
#########################################
PLUGIN_POST_BUILD_COPY_FILE(dev-enOcean package.json)
PLUGIN_POST_BUILD_COPY_FILE(dev-enOcean icon.png)
PLUGIN_POST_BUILD_COPY_DIRECTORY(dev-enOcean locales)
