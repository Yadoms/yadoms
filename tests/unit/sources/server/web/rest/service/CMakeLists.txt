
IF(NOT DISABLE_TEST_PLUGIN_CONFIGURATION_MERGER)
   ADD_YADOMS_SOURCES(
      server/web/rest/service/ConfigurationMerger.h
      server/web/rest/service/ConfigurationMerger.cpp
   )
   
   ADD_SOURCES(TestConfigurationMerger.cpp)
   
ENDIF()
