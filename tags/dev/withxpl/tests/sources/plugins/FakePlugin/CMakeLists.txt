
IF(NOT DISABLE_TEST_FAKEPLUGIN)
   ADD_YADOMS_SOURCES(
      plugins/FakePlugin/FakeTemperatureSensor.h
      plugins/FakePlugin/FakeTemperatureSensor.cpp)
   ADD_SOURCES(TestFakeTemperatureSensor.cpp)
   
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
   ADD_SOURCES(TestFakePluginConfiguration.cpp)
   
ENDIF()
