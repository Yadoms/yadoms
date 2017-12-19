
IF(NOT DISABLE_TEST_AUTOMATION_SCRIPT)
   SET(YADOMS_SOURCES
      shared/shared/currentTime/Provider.h
      shared/shared/exception/InvalidParameter.hpp
      server/automation/script/DayLightProvider.h
      server/automation/script/DayLightProvider.cpp
   )
   
   ADD_YADOMS_SOURCES(${YADOMS_SOURCES})
   
   ADD_SOURCES(TestDayLightProvider.cpp)
   
ENDIF()
