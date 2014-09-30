
IF(NOT DISABLE_TEST_FAKEPLUGIN)
   ADD_YADOMS_SOURCES(
      shared/shared/plugin/yadomsApi/IYadomsApi.h
      shared/shared/plugin/yadomsApi/commands/BatteryLevel.cpp
      shared/shared/plugin/yadomsApi/commands/BatteryLevel.h
      shared/shared/plugin/yadomsApi/commands/commands.h
      shared/shared/plugin/yadomsApi/commands/Counter.cpp
      shared/shared/plugin/yadomsApi/commands/Counter.h
      shared/shared/plugin/yadomsApi/commands/Curtain.cpp
      shared/shared/plugin/yadomsApi/commands/Curtain.h
      shared/shared/plugin/yadomsApi/commands/IHistorizable.h
      shared/shared/plugin/yadomsApi/commands/Message.cpp
      shared/shared/plugin/yadomsApi/commands/Message.h
      shared/shared/plugin/yadomsApi/commands/Rssi.cpp
      shared/shared/plugin/yadomsApi/commands/Rssi.h
      shared/shared/plugin/yadomsApi/commands/Switch.cpp
      shared/shared/plugin/yadomsApi/commands/Switch.h
      shared/shared/plugin/yadomsApi/commands/Temperature.cpp
      shared/shared/plugin/yadomsApi/commands/Temperature.h
   
      shared/shared/DataContainer.h
      shared/shared/DataContainer.cpp
      shared/shared/StringExtension.h
      shared/shared/StringExtension.cpp
      shared/shared/plugin/yadomsApi/KeywordAccessMode.h
      shared/shared/plugin/yadomsApi/KeywordType.h
		shared/shared/plugin/yadomsApi/StandardUnits.h
		shared/shared/plugin/yadomsApi/StandardCapacities.h
		shared/shared/plugin/yadomsApi/StandardValues.h
		shared/shared/plugin/yadomsApi/StandardDeclarations.cpp
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
