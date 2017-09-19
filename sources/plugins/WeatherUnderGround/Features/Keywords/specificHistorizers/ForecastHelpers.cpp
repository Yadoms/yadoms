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
      ("cloudy", "Cloudy")
      ("mostlycloudy", "Cloudy")
      ("partlycloudy", "PartlySunny")
      ("mostlysunny", "PartlySunny")
      ("snow", "Snow")
      ("flurries", "ChanceSnow")
      ("chanceflurries", "ChanceSnow")
      ("chancesnow", "ChanceSnow")
      ("sunny", "Sunny")
      ("clear", "Sunny")
      ("rain", "Rain")
      ("chancerain", "ChanceRain")
      ("sleet", "Sleet")
      ("chancesleet", "ChanceRain")
      ("tstorms", "Storm")
      ("chancetstorms", "ChanceStorm")
      ("fog", "Fog")
      ("hazy", "Fog")
      ("hazy", "Fog")
      ("partlysunny", "PartlySunny")
      ("nt_chancerain", "Night_Rain")
      ("nt_clear", "Night_Clear")
      ("nt_cloudy", "Night_Cloudy")
      ("nt_mostlycloudy", "Night_Cloudy")
      ("nt_partlycloudy", "Night_Cloudy")
      ("nt_snow", "Night_Snow");
   }
}

