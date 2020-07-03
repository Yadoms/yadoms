#dd current directory tests
IF(NOT DISABLE_TEST_HTTP_SSDP)
   ADD_YADOMS_SOURCES(
      shared/shared/http/ssdp/DiscoveredDevice.h
      shared/shared/http/ssdp/DiscoveredDevice.cpp
   )
   ADD_SOURCES(TestDiscoveredDevice.cpp)
   
ENDIF()


set (YADOMS_TESTS_SRC ${YADOMS_TESTS_SRC} PARENT_SCOPE)
