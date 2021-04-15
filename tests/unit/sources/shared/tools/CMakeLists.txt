
IF(NOT DISABLE_TEST_SHARED_EVENT)
   ADD_YADOMS_SOURCES(
      shared/shared/tools/Random.h
      shared/shared/tools/Random.cpp)
   
   ADD_SOURCES(
      TestRandom.cpp)
   
ENDIF()



set (YADOMS_TESTS_SRC ${YADOMS_TESTS_SRC} PARENT_SCOPE)