
IF(NOT DISABLE_TEST_PLUGIN_ENOCEAN)
   SET(YADOMS_SOURCES
      shared/shared/event/EventHandler.hpp
      shared/shared/communication/Buffer.hpp
      shared/shared/communication/IReceiveBufferHandler.h
   )
   
   SET(YADOMS_SOURCES
      plugins/EnOcean/message/types.h
      plugins/EnOcean/message/types.cpp
      plugins/EnOcean/message/DongleVersionResponseReceivedMessage.h
      plugins/EnOcean/message/DongleVersionResponseReceivedMessage.cpp
      plugins/EnOcean/message/ResponseReceivedMessage.h
      plugins/EnOcean/message/ResponseReceivedMessage.cpp
      plugins/EnOcean/message/Esp3ReceivedPacket.h
      plugins/EnOcean/message/Esp3ReceivedPacket.cpp
      plugins/EnOcean/message/Esp3SendPacket.h
      plugins/EnOcean/message/Esp3SendPacket.cpp
      plugins/EnOcean/message/Header.h
      plugins/EnOcean/message/Header.cpp
      plugins/EnOcean/message/Crc8.h
      plugins/EnOcean/message/Crc8.cpp
      plugins/EnOcean/MessageHandler.h
      plugins/EnOcean/MessageHandler.cpp
      plugins/EnOcean/ProfileHelper.h
      plugins/EnOcean/ProfileHelper.cpp
      plugins/EnOcean/ReceiveBufferHandler.h
      plugins/EnOcean/ReceiveBufferHandler.cpp
   )
   
   ADD_YADOMS_SOURCES(${YADOMS_SOURCES})
   
   ADD_SOURCES(
      AsyncPortMock.hpp
      BufferLoggerMock.hpp
      MessageHandlerMock.hpp
      ReceiveBufferHandlerMock.hpp
      
      TestBitsetHelpers.cpp
      TestCrc8.cpp
      TestReceiveBufferHandler.cpp
      TestDongleVersionResponseReceivedMessage.cpp
      TestMessageHandler.cpp
   )
   
ENDIF()
