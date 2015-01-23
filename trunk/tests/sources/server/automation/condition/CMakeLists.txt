
IF(NOT DISABLE_TEST_AUTOMATION)
   ADD_YADOMS_SOURCES(
      server/automation/condition/Comparators.h
      server/automation/condition/Comparators.cpp
      server/automation/condition/Become.h
      server/automation/condition/Become.cpp
      server/automation/condition/Is.h
      server/automation/condition/Is.cpp)
   
   ADD_SOURCES(
      TestBecome.cpp
      TestIs.cpp)
   
ENDIF()

