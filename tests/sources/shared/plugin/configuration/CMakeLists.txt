
IF(NOT DISABLE_TEST_PLUGIN_CONFIGURATION)
   ADD_YADOMS_SOURCES(
      plugins/FakePlugin/FakePluginConfiguration.h
      plugins/FakePlugin/FakePluginConfiguration.cpp)
   ADD_SOURCES(TestPluginConfiguration.cpp)
   
ENDIF()
