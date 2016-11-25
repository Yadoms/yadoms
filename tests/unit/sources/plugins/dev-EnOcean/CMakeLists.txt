
IF(NOT DISABLE_TEST_PLUGIN_ENOCEAN)
   SET(YADOMS_SOURCES
      shared/shared/event/EventHandler.hpp
      shared/shared/communication/Buffer.hpp
      shared/shared/communication/IReceiveBufferHandler.h
   )
   
   SET(YADOMS_SOURCES
      plugins/dev-EnOcean/message/types.h
      plugins/dev-EnOcean/message/Esp3ReceivedPacket.h
      plugins/dev-EnOcean/message/Esp3ReceivedPacket.cpp
      plugins/dev-EnOcean/message/Header.h
      plugins/dev-EnOcean/message/Header.cpp
      plugins/dev-EnOcean/message/Crc8.h
      plugins/dev-EnOcean/message/Crc8.cpp
      plugins/dev-EnOcean/profiles/generated-eep.h
      plugins/dev-EnOcean/ProfileHelper.h
      plugins/dev-EnOcean/ProfileHelper.cpp
      plugins/dev-EnOcean/ReceiveBufferHandler.h
      plugins/dev-EnOcean/ReceiveBufferHandler.cpp
   )
   
   ADD_YADOMS_SOURCES(${YADOMS_SOURCES})
   
   ADD_SOURCES(
      TestEnOceanReceiveBufferHandler.cpp
      TestEnOceanBitsetHelpers
   )
   
ENDIF()
