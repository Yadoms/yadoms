
IF(NOT DISABLE_TEST_RFXCOM)
   ADD_YADOMS_SOURCES(
      plugins/WES/equipments/Helpers.cpp
   )
   
   ADD_SOURCES(
      TestHelpers.cpp
   )
   
ENDIF()
