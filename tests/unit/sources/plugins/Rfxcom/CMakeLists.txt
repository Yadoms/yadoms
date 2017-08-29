
IF(NOT DISABLE_TEST_RFXCOM)
   ADD_YADOMS_SOURCES(
      plugins/Rfxcom/rfxcomMessages/RareDeviceIdFilter.cpp
      plugins/Rfxcom/RfxComReceiveBufferHandler.h
      plugins/Rfxcom/RfxComReceiveBufferHandler.cpp
   )
   
   ADD_SOURCES(
      TestRareDeviceIdFilter.cpp
      TestRfxComReceiveBufferHandler.cpp
   )
   
ENDIF()
