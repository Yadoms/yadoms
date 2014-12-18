
IF(NOT DISABLE_TEST_CONDITION)
   ADD_YADOMS_SOURCES(
      shared/shared/DataContainer.h
      shared/shared/DataContainer.cpp

      server/job/condition/And.h
      server/job/condition/And.cpp)
   
   ADD_SOURCES(TestAnd.cpp)
   
ENDIF()
