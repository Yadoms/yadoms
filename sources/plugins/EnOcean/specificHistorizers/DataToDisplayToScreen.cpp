#include "stdafx.h"
#include "DataToDisplayToScreen.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace specificHistorizers
{
   DECLARE_ENUM_IMPLEMENTATION(EDataToDisplayToScreen,
                               ((NoDisplay))
                               ((RoomTemperatureDegrees))
                               ((RoomTemperatureFarenheit))
                               ((NominalTemperatureDegrees))
                               ((NominalTemperatureFarenheit))
                               ((DeltaTemperatureSetPointDegrees))
                               ((DeltaTemperatureSetPointFarenheit))
                               ((DeltaTemperatureSetPointGraphic))
                               ((Illumination))
                               ((Percentage))
                               ((PartsPerMillion))
                               ((RelativeHumidity))
   );

   DECLARE_CAPACITY(DataToDisplayToScreenCapacity,
                    "DataToDisplayToScreen",
                    yApi::CStandardUnits::NoUnit(),
                    yApi::EKeywordDataType::kEnum);

   CDataToDisplayToScreen::CDataToDisplayToScreen(const std::string& keywordName)
      : CSingleHistorizableData<EDataToDisplayToScreen>(keywordName,
                                                        DataToDisplayToScreenCapacity(),
                                                        yApi::EKeywordAccessMode::kGetSet)
   {
   }
} // namespace specificHistorizers
