#dd current directory tests
IF(NOT DISABLE_TEST_DATACONTAINER)
   ADD_YADOMS_SOURCES(
      shared/shared/DataContainer.h
      shared/shared/DataContainer.cpp)
   ADD_SOURCES(TestDataContainer.cpp)
   
ENDIF()

# List subdirectories here
add_subdirectory(event)
add_subdirectory(notification)



set (YADOMS_TESTS_SRC ${YADOMS_TESTS_SRC} PARENT_SCOPE)
