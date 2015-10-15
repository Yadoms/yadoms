
IF(NOT DISABLE_TEST_RFXCOM)
   ADD_YADOMS_SOURCES(
      plugins/Rfxcom/Buffer.hpp)
   ADD_SOURCES(TestBuffer.cpp)
   
ENDIF()
