#include "stdafx.h"
#include "Forecast.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace xplrules
{
   namespace rfxLanXpl
   {
      namespace data
      {
         DECLARE_ENUM_IMPLEMENTATION(EForecast,
            ((Sunny)("sunny"))
            ((PartlyCloudy)("partly cloudy"))
            ((Cloudy)("cloudy"))
            ((Rain)("rain"))
         );


         DECLARE_CAPACITY(ForecastCapacity, "forecast", shared::plugin::yPluginApi::CStandardUnits::NoUnit(), shared::plugin::yPluginApi::EKeywordDataType::kNoData);


         CForecast::CForecast(const std::string& keywordName)
            : CSingleHistorizableData<EForecast>(keywordName,
                                                 ForecastCapacity(),
                                                 shared::plugin::yPluginApi::EKeywordAccessMode::kGet)
         {
         }

         CForecast::~CForecast()
         {
         }
      }
   }
} // namespace xplrules::rfxLanXpl::data


