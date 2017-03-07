#include "stdafx.h"
#include "WeatherCondition.h"
#include "../StandardCapacities.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            DECLARE_ENUM_IMPLEMENTATION(EWeatherCondition,
               ((Cloudy))
               ((Snow))
               ((ChanceSnow))
               ((PartlySunny))
               ((Sunny))
               ((ChanceRain))
               ((Rain))
               ((Sleet))
               ((ChanceStorm))
               ((Storm))
               ((Fog))
               ((Night_Clear))
               ((Night_Cloudy))
               ((Night_Rain))
               ((Night_Snow))
            );


            CWeatherCondition::CWeatherCondition(const std::string& keywordName,
                                                 const EKeywordAccessMode& accessMode,
                                                 const EMeasureType& measureType)
               : CSingleHistorizableData<EWeatherCondition>(keywordName,
                                                            CStandardCapacities::WeatherCondition(),
                                                            accessMode,
                                                            EWeatherCondition::kCloudy,
                                                            measureType)
            {
            }

            CWeatherCondition::~CWeatherCondition()
            {
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization


