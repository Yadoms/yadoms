#dd current directory tests
IF(NOT DISABLE_TEST_DATACONTAINER)
   ADD_YADOMS_SOURCES(
      shared/shared/DataContainer.h
      shared/shared/DataContainer.cpp
      shared/shared/Log.h
      shared/shared/Log.cpp
      shared/shared/logInternal/LogStreamEnhanced.h
      shared/shared/logInternal/LogStreamEnhanced.cpp
   )
   ADD_SOURCES(TestDataContainer.cpp)
   
ENDIF()

# List subdirectories here
add_subdirectory(communication)
add_subdirectory(event)
add_subdirectory(notification)



set (YADOMS_TESTS_SRC ${YADOMS_TESTS_SRC} PARENT_SCOPE)
