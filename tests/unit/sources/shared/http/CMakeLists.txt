
IF(NOT DISABLE_TEST_SHARED_HTTP)
   ADD_YADOMS_SOURCES(
      shared/shared/http/HttpMethods.cpp
      shared/shared/http/HttpMethods.h)
   
   ADD_SOURCES(
      TestHttpMethods.cpp)
   
ENDIF()
