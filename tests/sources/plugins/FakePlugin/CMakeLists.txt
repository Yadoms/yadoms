
IF(NOT DISABLE_TEST_FAKEPLUGIN)
   ADD_YADOMS_SOURCES(
      shared/shared/plugin/yadomsApi/IYadomsApi.h
      shared/shared/DataContainer.h
      shared/shared/DataContainer.cpp
      shared/shared/StringExtension.h
      shared/shared/StringExtension.cpp
      shared/shared/plugin/yadomsApi/KeywordAccessMode.h
      shared/shared/plugin/yadomsApi/KeywordType.h
      shared/shared/plugin/yadomsApi/StandardCapacities.h
      shared/shared/plugin/yadomsApi/StandardCapacities.cpp
      shared/shared/plugin/yadomsApi/StandardUnits.h
      shared/shared/plugin/yadomsApi/StandardUnits.cpp
      shared/shared/plugin/yadomsApi/StandardValues.h
      shared/shared/plugin/yadomsApi/StandardValues.cpp
      shared/shared/plugin/yadomsApi/StandardCapacity.h
      shared/shared/plugin/yadomsApi/StandardCapacity.cpp
      plugins/FakePlugin/FakeSensor.h
      plugins/FakePlugin/FakeSensor.cpp)
   ADD_SOURCES(TestFakeSensor.cpp)
   
   ADD_YADOMS_SOURCES(
      plugins/FakePlugin/FakePluginConfiguration.h
      plugins/FakePlugin/FakePluginConfiguration.cpp)
   ADD_SOURCES(TestFakePluginConfiguration.cpp)
   
ENDIF()
