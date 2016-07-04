
IF(NOT DISABLE_TEST_PLUGIN_ENOCEAN)
   SET(YADOMS_SOURCES
      shared/shared/event/EventHandler.hpp
      shared/shared/communication/Buffer.hpp
      shared/shared/communication/IReceiveBufferHandler.h
   )
   
   SET(YADOMS_SOURCES
      plugins/dev-EnOcean/EnOceanMessage.h
      plugins/dev-EnOcean/EnOceanMessage.cpp
      plugins/dev-EnOcean/EnOceanCrc8.h
      plugins/dev-EnOcean/EnOceanCrc8.cpp
      plugins/dev-EnOcean/EnOceanReceiveBufferHandler.h
      plugins/dev-EnOcean/EnOceanReceiveBufferHandler.cpp
   )
   
   ADD_YADOMS_SOURCES(${YADOMS_SOURCES})
   
   ADD_SOURCES(TestEnOceanReceiveBufferHandler.cpp)
   
ENDIF()
