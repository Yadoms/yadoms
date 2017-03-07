
IF(NOT DISABLE_TEST_RFXCOM)
   ADD_YADOMS_SOURCES(
      plugins/Rfxcom/rfxcomMessages/CurrentEnergyFilter.cpp
      plugins/Rfxcom/RfxComReceiveBufferHandler.h
      plugins/Rfxcom/RfxComReceiveBufferHandler.cpp
   )
   
   ADD_SOURCES(
      TestCurrentEnergyFilter.cpp
      TestRfxComReceiveBufferHandler.cpp
   )
   
ENDIF()
