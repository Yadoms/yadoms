
IF(NOT DISABLE_TEST_START_OPTIONS_LOADER)
   ADD_YADOMS_SOURCES(
      server/startupOptions/Loader.h
      server/startupOptions/Loader.cpp)
   
   ADD_SOURCES(TestLoader.cpp)
   
ENDIF()
