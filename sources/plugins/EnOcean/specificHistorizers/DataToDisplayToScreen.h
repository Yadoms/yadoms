#pragma once
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>
#include <shared/enumeration/EnumHelpers.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace specificHistorizers
{
   DECLARE_ENUM_HEADER(EDataToDisplayToScreen,
                       ((NoDisplay)(0))
                       ((RoomTemperatureDegrees)(1))
                       ((RoomTemperatureFarenheit)(2))
                       ((NominalTemperatureDegrees)(3))
                       ((NominalTemperatureFarenheit)(4))
                       ((DeltaTemperatureSetPointDegrees)(5))
                       ((DeltaTemperatureSetPointFarenheit)(6))
                       ((DeltaTemperatureSetPointGraphic)(7))
                       ((Illumination)(8))
                       ((Percentage)(9))
                       ((PartsPerMillion)(10))
                       ((RelativeHumidity)(11))
   ) ;

   class CDataToDisplayToScreen : public yApi::historization::CSingleHistorizableData<EDataToDisplayToScreen>
   {
   public:
      explicit CDataToDisplayToScreen(const std::string& keywordName);
      virtual ~CDataToDisplayToScreen() = default;
   };
} // namespace specificHistorizers
