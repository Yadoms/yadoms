
IF(NOT DISABLE_TEST_PLUGIN_CONFIGURATION)
   ADD_YADOMS_SOURCES(
      shared/shared/serialization/IPtreeToStringSerializer.h
      shared/shared/serialization/PTreeToJsonSerializer.h
      shared/shared/serialization/PTreeToJsonSerializer.cpp
      shared/shared/plugin/configuration/Factory.h
      shared/shared/plugin/configuration/Factory.cpp
      shared/shared/plugin/configuration/Configuration.h
      shared/shared/plugin/configuration/Configuration.cpp
      plugins/FakePlugin/FakePluginConfiguration.h
      plugins/FakePlugin/FakePluginConfiguration.cpp)
   ADD_SOURCES(TestPluginConfiguration.cpp)
   
ENDIF()
