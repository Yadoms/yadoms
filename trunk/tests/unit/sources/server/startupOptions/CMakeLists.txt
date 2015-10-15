
IF(NOT DISABLE_TEST_START_OPTIONS_LOADER)
   ADD_YADOMS_SOURCES(
      server/startupOptions/StartupOptions.h
      server/startupOptions/StartupOptions.cpp      
      server/startupOptions/MustExistPathValidator.h
      server/startupOptions/MustExistPathValidator.cpp)
   
   ADD_SOURCES(TestLoader.cpp)
   
ENDIF()
