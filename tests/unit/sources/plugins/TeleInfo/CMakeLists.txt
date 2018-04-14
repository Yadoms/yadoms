IF(NOT DISABLE_TEST_PLUGIN_TELEINFO)
   SET(YADOMS_SOURCES
      shared/shared/event/EventHandler.hpp
      shared/shared/communication/Buffer.hpp
      shared/shared/communication/IReceiveBufferHandler.h
      shared/shared/communication/AsyncSerialPort.h
      shared/shared/communication/AsyncSerialPort.cpp
      shared/shared/communication/AsyncPortConnectionNotification.h
      shared/shared/communication/AsyncPortConnectionNotification.cpp
      shared/shared/communication/AsciiBufferLogger.h
      shared/shared/communication/AsciiBufferLogger.cpp
      shared/shared/communication/BufferLoggerHelper.h
      shared/shared/communication/BufferLoggerHelper.cpp
   )

   SET(YADOMS_SOURCES ${YADOMS_SOURCES}
      plugins/TeleInfo/TeleInfoReceiveBufferHandler.h
      plugins/TeleInfo/TeleInfoReceiveBufferHandler.cpp
      plugins/TeleInfo/TeleInfoFactory.h
      plugins/TeleInfo/TeleInfoFactory.cpp
      plugins/TeleInfo/Decoder.h
   )

   IF (WIN32)
      set(YADOMS_SOURCES ${YADOMS_SOURCES}
	       # Add Windows sources
	       plugins/TeleInfo/Windows/FT2xxSerialPort.h
	   )
      
      set(YADOMS_SOURCES ${YADOMS_SOURCES}
          shared/Windows/shared/Peripherals.h
          shared/Windows/shared/Peripherals.cpp
      )
      
	   source_group(Windows Windows/*.*)
   ELSEIF (UNIX AND NOT APPLE)
      set(YADOMS_SOURCES   ${YADOMS_SOURCES}
	       # Add Common Linux and RaspberryPI sources
	       plugins/TeleInfo/Linux/FT2xxSerialPort.h
	   )
      source_group(Linux Linux/*.*)
   ENDIF()

   ADD_YADOMS_SOURCES(${YADOMS_SOURCES})
   
   ADD_SOURCES(
      TestTeleInfoReceiveBufferHandler.cpp
      TestTeleInfoFactory.cpp
      FT2xxSerialPortMock.cpp
      DecoderMock.cpp
   )
ENDIF()
