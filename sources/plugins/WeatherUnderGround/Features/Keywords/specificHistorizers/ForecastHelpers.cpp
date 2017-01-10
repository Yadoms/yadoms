#include "stdafx.h"
#include "ForecastHelpers.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>

namespace yApi = shared::plugin::yPluginApi;

namespace weatherunderground
{
   namespace helper
   {
      // Definition of the period type of frame to be sent to the HMI
      DECLARE_ENUM_IMPLEMENTATION(EPeriod,
         ((NoPeriod))
         ((Day))
         ((Hour))
      );

      //All tags sent by WeatherUnderground could be found here
      const EnumValuesNames EEnumTypeNames = boost::assign::map_list_of
      ("cloudy", yApi::historization::EWeatherCondition::kCloudy)
      ("mostlycloudy", yApi::historization::EWeatherCondition::kCloudy)
      ("partlycloudy", yApi::historization::EWeatherCondition::kPartlySunny)
      ("mostlysunny", yApi::historization::EWeatherCondition::kPartlySunny)
      ("snow", yApi::historization::EWeatherCondition::kSnow)
      ("flurries", yApi::historization::EWeatherCondition::kChanceSnow)
      ("chanceflurries", yApi::historization::EWeatherCondition::kChanceSnow)
      ("chancesnow", yApi::historization::EWeatherCondition::kChanceSnow)
      ("sunny", yApi::historization::EWeatherCondition::kSunny)
      ("clear", yApi::historization::EWeatherCondition::kSunny)
      ("rain", yApi::historization::EWeatherCondition::kRain)
      ("chancerain", yApi::historization::EWeatherCondition::kChanceRain)
      ("sleet", yApi::historization::EWeatherCondition::kSleet)
      ("chancesleet", yApi::historization::EWeatherCondition::kChanceRain)
      ("tstorms", yApi::historization::EWeatherCondition::kStorm)
      ("chancetstorms", yApi::historization::EWeatherCondition::kChanceStorm)
      ("fog", yApi::historization::EWeatherCondition::kFog)
      ("hazy", yApi::historization::EWeatherCondition::kFog)
      ("hazy", yApi::historization::EWeatherCondition::kFog)
      ("partlysunny", yApi::historization::EWeatherCondition::kPartlySunny)
      ("nt_chancerain", yApi::historization::EWeatherCondition::kNight_Rain)
      ("nt_clear", yApi::historization::EWeatherCondition::kNight_Clear)
      ("nt_cloudy", yApi::historization::EWeatherCondition::kNight_Cloudy)
      ("nt_mostlycloudy", yApi::historization::EWeatherCondition::kNight_Cloudy)
      ("nt_partlycloudy", yApi::historization::EWeatherCondition::kNight_Cloudy)
      ("nt_snow", yApi::historization::EWeatherCondition::kNight_Snow);
   }
}

