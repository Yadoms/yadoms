
IF(NOT DISABLE_TEST_FAKEPLUGIN)
   ADD_YADOMS_SOURCES(
      shared/shared/plugin/yadomsApi/IYadomsApi.h
      shared/shared/plugin/yadomsApi/historization/BatteryLevel.cpp
      shared/shared/plugin/yadomsApi/historization/BatteryLevel.h
      shared/shared/plugin/yadomsApi/historization/Historizers.h
      shared/shared/plugin/yadomsApi/historization/Counter.cpp
      shared/shared/plugin/yadomsApi/historization/Counter.h
      shared/shared/plugin/yadomsApi/historization/Curtain.cpp
      shared/shared/plugin/yadomsApi/historization/Curtain.h
      shared/shared/plugin/yadomsApi/historization/IHistorizable.h
      shared/shared/plugin/yadomsApi/historization/Message.cpp
      shared/shared/plugin/yadomsApi/historization/Message.h
      shared/shared/plugin/yadomsApi/historization/Rssi.cpp
      shared/shared/plugin/yadomsApi/historization/Rssi.h
      shared/shared/plugin/yadomsApi/historization/Switch.cpp
      shared/shared/plugin/yadomsApi/historization/Switch.h
      shared/shared/plugin/yadomsApi/historization/Temperature.cpp
      shared/shared/plugin/yadomsApi/historization/Temperature.h
   
      shared/shared/DataContainer.h
      shared/shared/DataContainer.cpp
      shared/shared/StringExtension.h
      shared/shared/StringExtension.cpp
      shared/shared/plugin/yadomsApi/KeywordAccessMode.h
      shared/shared/plugin/yadomsApi/KeywordAccessMode.cpp
      shared/shared/plugin/yadomsApi/KeywordDataType.h
      shared/shared/plugin/yadomsApi/KeywordDataType.cpp
		shared/shared/plugin/yadomsApi/StandardUnits.h
		shared/shared/plugin/yadomsApi/StandardCapacities.h
		shared/shared/plugin/yadomsApi/StandardValues.h
		shared/shared/plugin/yadomsApi/StandardDeclarations.cpp
		shared/shared/plugin/yadomsApi/StandardCapacity.h
		shared/shared/plugin/yadomsApi/StandardCapacity.cpp
      shared/shared/plugin/yadomsApi/historization/MeasureType.h
      shared/shared/plugin/yadomsApi/historization/MeasureType.cpp
      plugins/FakePlugin/FakeSensor.h
      plugins/FakePlugin/FakeSensor.cpp)
   ADD_SOURCES(TestFakeSensor.cpp)
   
   ADD_YADOMS_SOURCES(
      plugins/FakePlugin/FakePluginConfiguration.h
      plugins/FakePlugin/FakePluginConfiguration.cpp)
   ADD_SOURCES(TestFakePluginConfiguration.cpp)
   
ENDIF()
