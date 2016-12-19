
IF(NOT DISABLE_TEST_PLUGIN_TELEINFO)
   SET(YADOMS_SOURCES
      shared/shared/event/EventHandler.hpp
      shared/shared/communication/Buffer.hpp
      shared/shared/communication/IReceiveBufferHandler.h
   )
   
   SET(YADOMS_SOURCES
      plugins/TeleInfo/TeleInfoReceiveBufferHandler.h
      plugins/TeleInfo/TeleInfoReceiveBufferHandler.cpp
   )
   
   ADD_YADOMS_SOURCES(${YADOMS_SOURCES})
   
   ADD_SOURCES(
      TestTeleInfoReceiveBufferHandler.cpp
   )
   
ENDIF()
