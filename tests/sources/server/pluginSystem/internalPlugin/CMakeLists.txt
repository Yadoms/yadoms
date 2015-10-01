
IF(NOT DISABLE_TEST_INTERNAL_PLUGIN)
   ADD_YADOMS_SOURCES(
      shared/shared/DataContainer.h
      shared/shared/DataContainer.cpp
      shared/shared/plugin/yPluginApi/KeywordDataType.h
      shared/shared/plugin/yPluginApi/KeywordDataType.cpp
      shared/shared/plugin/yPluginApi/historization/Event.cpp
      shared/shared/plugin/yPluginApi/historization/Event.h
      shared/shared/plugin/yPluginApi/historization/MeasureType.h
      shared/shared/plugin/yPluginApi/historization/MeasureType.cpp
      shared/shared/ServiceLocator.h
      shared/shared/ServiceLocator.cpp
      server/automation/script/DayLight.cpp
      server/pluginSystem/internalPlugin/System.cpp)
   ADD_SOURCES(TestSystem.cpp)
   
ENDIF()
