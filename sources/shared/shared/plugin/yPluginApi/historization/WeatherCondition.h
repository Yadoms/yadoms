#pragma once
#include <shared/Export.h>
#include <shared/enumeration/EnumHelpers.hpp>
#include "SingleHistorizableData.hpp"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            //-----------------------------------------------------
            ///\brief  The weather states
            //-----------------------------------------------------   
            DECLARE_ENUM_HEADER_SHARED(EWeatherCondition, YADOMS_SHARED_EXPORT,
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
            ) ;

            //-----------------------------------------------------
            ///\brief A Weather object
            //-----------------------------------------------------
            class YADOMS_SHARED_EXPORT CWeatherCondition : public CSingleHistorizableData<EWeatherCondition>
            {
            public:
               //-----------------------------------------------------
               ///\brief                     Constructor
               ///\param[in] keywordName     Yadoms keyword name
               ///\param[in] accessMode      The access mode
               ///\param[in] measureType     The measure type (normally kAbsolute)
               ///\param[in] historyDepth    The history depth policy
               //-----------------------------------------------------
               explicit CWeatherCondition(const std::string& keywordName,
                                          const EKeywordAccessMode& accessMode = EKeywordAccessMode::kGet,
                                          const EMeasureType& measureType = EMeasureType::kAbsolute,
                                          const EHistoryDepth& historyDepth = EHistoryDepth::kDefault);

               virtual ~CWeatherCondition() = default;
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization
