
IF(NOT DISABLE_TEST_RFXCOM)
   ADD_YADOMS_SOURCES(
      plugins/Rfxcom/Buffer.hpp
      plugins/Rfxcom/rfxcomMessages/CurrentEnergyFilter.cpp
   )
   
   ADD_SOURCES(
      TestBuffer.cpp
      TestCurrentEnergyFilter.cpp
   )
   
ENDIF()
