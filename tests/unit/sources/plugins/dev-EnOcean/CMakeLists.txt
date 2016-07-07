
IF(NOT DISABLE_TEST_PLUGIN_ENOCEAN)
   SET(YADOMS_SOURCES
      shared/shared/event/EventHandler.hpp
      shared/shared/communication/Buffer.hpp
      shared/shared/communication/IReceiveBufferHandler.h
   )
   
   SET(YADOMS_SOURCES
      plugins/dev-EnOcean/message/types.h
      plugins/dev-EnOcean/message/ReceivedMessage.h
      plugins/dev-EnOcean/message/ReceivedMessage.cpp
      plugins/dev-EnOcean/message/Crc8.h
      plugins/dev-EnOcean/message/Crc8.cpp
      plugins/dev-EnOcean/EnOceanReceiveBufferHandler.h
      plugins/dev-EnOcean/EnOceanReceiveBufferHandler.cpp
   )
   
   ADD_YADOMS_SOURCES(${YADOMS_SOURCES})
   
   ADD_SOURCES(TestEnOceanReceiveBufferHandler.cpp)
   
ENDIF()
