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
               ((Sunny))
               ((Cloudy))
               ((CloudyGusts))
               ((CloudyWindy))
               ((CloudyHigh))
               ((PartlyCloudy))
               ((Fog))
               ((Hail))
               ((Haze))
               ((Drizzle))
               ((Lightning))
               ((Rain))
               ((RainMix))
               ((RainWind))
               ((Showers))
               ((Sleet))
               ((SleetStorm))
               ((Snow))
               ((SnowThunderstorm))
               ((SnowWind))
               ((Sprinkle))
               ((StormShowers))
               ((Thunderstorm))
               ((Windy))
               ((LightWind))

               ((NightClear))
               ((NightCloudy))
               ((NightCloudyGusts))
               ((NightCloudyWindy))
               ((NightCloudyHigh))
               ((NightPartlyCloudy))
               ((NightFog))
               ((NightHail))
               ((NightHaze))
               ((NightLightning))
               ((NightRain))
               ((NightRainMix))
               ((NightRainWind))
               ((NightShowers))
               ((NightSleet))
               ((NightSleetStorm))
               ((NightSnow))
               ((NightSnowThunderstorm))
               ((NightSnowWind))
               ((NightSprinkle))
               ((NightStormShowers))
               ((NightThunderstorm))
               ((NightWindy))
               ((NightLightWind))
            );


            CWeatherCondition::CWeatherCondition(const std::string& keywordName,
                                                 const EKeywordAccessMode& accessMode,
                                                 const EMeasureType& measureType,
                                                 const EHistoryDepth& historyDepth)
               : CSingleHistorizableData<EWeatherCondition>(keywordName,
                                                            CStandardCapacities::WeatherCondition(),
                                                            accessMode,
                                                            EWeatherCondition::kCloudy,
                                                            measureType,
                                                            typeInfo::CEmptyTypeInfo::Empty,
                                                            historyDepth)
            {
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization
