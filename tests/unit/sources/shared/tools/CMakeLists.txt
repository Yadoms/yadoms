
IF(NOT DISABLE_TEST_SHARED_EVENT)
   ADD_YADOMS_SOURCES(
      shared/shared/tools/Random.h
      shared/shared/tools/Random.cpp)
   
   ADD_SOURCES(
      TestRandom.cpp)
   
ENDIF()
