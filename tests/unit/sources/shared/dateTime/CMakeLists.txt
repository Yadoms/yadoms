
IF(NOT DISABLE_TEST_SHARED_EVENT)
   ADD_YADOMS_SOURCES(
      shared/shared/dateTime/Helper.h
      shared/shared/dateTime/Helper.cpp)
   
   ADD_SOURCES(
      TestHelper.cpp)
   
ENDIF()
