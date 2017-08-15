IF(NOT DISABLE_TEST_PLUGIN_LINKY)
   SET(YADOMS_SOURCES
      shared/shared/event/EventHandler.hpp
      shared/shared/communication/Buffer.hpp
      shared/shared/communication/IReceiveBufferHandler.h
   )
   
   SET(YADOMS_SOURCES
      plugins/Linky/LinkyReceiveBufferHandler.h
      plugins/Linky/LinkyReceiveBufferHandler.cpp
   )
   
   ADD_YADOMS_SOURCES(${YADOMS_SOURCES})
   
   ADD_SOURCES(
      TestLinkyReceiveBufferHandler.cpp
   )
   
ENDIF()
