IF(NOT DISABLE_TEST_PLUGIN_ORANGE_BUSINESS)
   SET(YADOMS_SOURCES
      shared/shared/plugin/yPluginApi/IYPluginApi.h
      shared/shared/plugin/yPluginApi/historization/IHistorizable.h
      shared/shared/plugin/yPluginApi/historization/Historizers.h
      shared/shared/plugin/yPluginApi/historization/Text.cpp
      shared/shared/plugin/yPluginApi/historization/Text.h
      shared/shared/plugin/yPluginApi/historization/BatteryLevel.cpp
      shared/shared/plugin/yPluginApi/historization/BatteryLevel.h

      shared/shared/plugin/yPluginApi/historization/typeInfo/ITypeInfo.h
      shared/shared/plugin/yPluginApi/historization/typeInfo/EmptyTypeInfo.h
      shared/shared/plugin/yPluginApi/historization/typeInfo/EmptyTypeInfo.cpp
      shared/shared/plugin/yPluginApi/historization/typeInfo/IntTypeInfo.h
      shared/shared/plugin/yPluginApi/historization/typeInfo/IntTypeInfo.cpp
      shared/shared/plugin/yPluginApi/historization/typeInfo/DoubleTypeInfo.h
      shared/shared/plugin/yPluginApi/historization/typeInfo/DoubleTypeInfo.cpp
      shared/shared/plugin/yPluginApi/historization/typeInfo/StringTypeInfo.h
      shared/shared/plugin/yPluginApi/historization/typeInfo/StringTypeInfo.cpp
      shared/shared/plugin/yPluginApi/historization/typeInfo/EnumTypeInfo.hpp   
   
      shared/shared/DataContainer.h
      shared/shared/DataContainer.cpp
      shared/shared/StringExtension.h
      shared/shared/plugin/yPluginApi/KeywordAccessMode.h
      shared/shared/plugin/yPluginApi/KeywordAccessMode.cpp
      shared/shared/plugin/yPluginApi/KeywordDataType.h
      shared/shared/plugin/yPluginApi/KeywordDataType.cpp
      shared/shared/plugin/yPluginApi/StandardUnits.h
      shared/shared/plugin/yPluginApi/StandardUnits.cpp
      shared/shared/plugin/yPluginApi/StandardCapacities.h
      shared/shared/plugin/yPluginApi/StandardCapacities.cpp
      shared/shared/plugin/yPluginApi/StandardValues.h
      shared/shared/plugin/yPluginApi/StandardValues.cpp
      shared/shared/plugin/yPluginApi/StandardCapacity.h
      shared/shared/plugin/yPluginApi/StandardCapacity.cpp
      shared/shared/plugin/yPluginApi/historization/MeasureType.h
      shared/shared/plugin/yPluginApi/historization/MeasureType.cpp
   )
   
   SET(YADOMS_SOURCES
      plugins/OrangeBusiness/DefaultEquipment.h
      plugins/OrangeBusiness/DefaultEquipment.cpp
      plugins/OrangeBusiness/Decoder.h
      plugins/OrangeBusiness/Decoder.cpp
      plugins/OrangeBusiness/deviceManager.h
      plugins/OrangeBusiness/deviceManager.cpp
      plugins/OrangeBusiness/IurlManager.h
      plugins/OrangeBusiness/IDecoder.h
      plugins/OrangeBusiness/IEquipment.h
   )
   
   ADD_YADOMS_SOURCES(${YADOMS_SOURCES})
   
   ADD_SOURCES(
      TestOrangeBusinessDecoder.cpp
      TestOrangeBusinessDefaultEquipment.cpp
      TestOrangeBusinessEquipmentManager.cpp
      urlManagerMock.cpp
      urlManagerMock.h
      MessagesHelper.cpp
      MessagesHelper.h
   )
   
ENDIF()
