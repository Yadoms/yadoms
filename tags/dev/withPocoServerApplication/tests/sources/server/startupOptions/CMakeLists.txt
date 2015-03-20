
IF(NOT DISABLE_TEST_START_OPTIONS_LOADER)
   ADD_YADOMS_SOURCES(
      server/startupOptions/Loader.h
      server/startupOptions/Loader.cpp      
      server/startupOptions/MustExistPathValidator.h
      server/startupOptions/MustExistPathValidator.cpp)
   
   ADD_SOURCES(TestLoader.cpp)
   
ENDIF()
