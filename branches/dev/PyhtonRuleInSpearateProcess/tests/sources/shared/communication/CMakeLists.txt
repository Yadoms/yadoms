
IF(NOT DISABLE_TEST_SHARED_COMMUNICATION)
   ADD_YADOMS_SOURCES(
      shared/shared/communication/Buffer.hpp
      shared/shared/communication/FixedSizeReceiveBufferHandler.cpp
      shared/shared/communication/FixedSizeReceiveBufferHandler.h
      shared/shared/communication/EOFReceiveBufferHandler.cpp
      shared/shared/communication/EOFReceiveBufferHandler.h)
   
   ADD_SOURCES(
      TestBuffer.cpp
      TestFixedSizeReceiveBufferHandler.cpp
      TestEOFReceiveBufferHandler.cpp)
   
ENDIF()
