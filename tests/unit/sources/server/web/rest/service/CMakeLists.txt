
IF(NOT DISABLE_TEST_PLUGIN_CONFIGURATION_MERGER)
   ADD_YADOMS_SOURCES(
      server/web/rest/service/PluginConfigurationMerger.h
      server/web/rest/service/PluginConfigurationMerger.cpp
   )
   
   ADD_SOURCES(TestPluginConfigurationMerger.cpp)
   
ENDIF()
