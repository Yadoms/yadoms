
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
   )
source_group(enOceanDescriptors enOceanDescriptors/*.*)

add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/enOceanDescriptors/generated-manufacturers.h
                          ${CMAKE_CURRENT_SOURCE_DIR}/enOceanDescriptors/generated-manufacturers.cpp
                   COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/enOceanDescriptors/generateManufacturers.py ${CMAKE_CURRENT_SOURCE_DIR}/enOceanDescriptors/manufacturers.xml ${CMAKE_CURRENT_SOURCE_DIR}/enOceanDescriptors/generated-manufacturers.h ${CMAKE_CURRENT_SOURCE_DIR}/enOceanDescriptors/generated-manufacturers.cpp
                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/enOceanDescriptors/manufacturers.xml
                           ${PYTHON_GENERATOR_FILES} 
                   COMMENT "Generating Manufacturers source code...")

add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/enOceanDescriptors/generated-eep.h
                          ${CMAKE_CURRENT_SOURCE_DIR}/enOceanDescriptors/generated-eep.cpp
                   COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/enOceanDescriptors/generateEep.py ${CMAKE_CURRENT_SOURCE_DIR}/enOceanDescriptors/eep${EEP_VERSION}.xml ${CMAKE_CURRENT_SOURCE_DIR}/enOceanDescriptors/generated-eep.h ${CMAKE_CURRENT_SOURCE_DIR}/enOceanDescriptors/generated-eep.cpp
                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/enOceanDescriptors/eep${EEP_VERSION}.xml
                           ${PYTHON_GENERATOR_FILES} 
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
