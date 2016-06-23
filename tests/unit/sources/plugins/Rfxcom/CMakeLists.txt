
IF(NOT DISABLE_TEST_RFXCOM)
   ADD_YADOMS_SOURCES(
      plugins/Rfxcom/rfxcomMessages/CurrentEnergyFilter.cpp
   )
   
   ADD_SOURCES(
      TestCurrentEnergyFilter.cpp
   )
   
ENDIF()
