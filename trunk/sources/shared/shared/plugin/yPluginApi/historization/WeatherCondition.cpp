#include "stdafx.h"
#include "WeatherCondition.h"
#include "../StandardCapacities.h"

namespace shared { namespace plugin { namespace yPluginApi { namespace historization
{
   DECLARE_ENUM_IMPLEMENTATION(EWeatherCondition,
      ((Cloudy))
      ((Snow))
	  ((PartlySunny))
	  ((Sunny))
	  ((Rain))
	  ((Sleet))
	  ((Storm))
	  ((Fog))
   );


CWeatherCondition::CWeatherCondition(const std::string& keywordName, const EKeywordAccessMode& accessMode, const EMeasureType& measureType)
   :CSingleHistorizableData<EWeatherCondition>(keywordName, CStandardCapacities::WeatherCondition, accessMode, EWeatherCondition::kCloudyValue, measureType)
{
}

CWeatherCondition::~CWeatherCondition()
{
}

} } } } // namespace shared::plugin::yPluginApi::historization

