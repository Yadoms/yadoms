#include "stdafx.h"
#include "Forecast.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace xplrules { namespace rfxLanXpl { namespace data {

DECLARE_ENUM_IMPLEMENTATION(EForecast,
   ((Sunny)("sunny"))
   ((PartlyCloudy)("partly cloudy"))
   ((Cloudy)("cloudy"))
   ((Rain)("rain"))
);


const shared::plugin::yPluginApi::CStandardCapacity& ForecastCapacity = shared::plugin::yPluginApi::CStandardCapacity("forecast", shared::plugin::yPluginApi::CStandardUnits::NoUnits, shared::plugin::yPluginApi::EKeywordDataType::kNoData);

   
CForecast::CForecast(const std::string& keywordName)
   :shared::plugin::yPluginApi::historization::CSingleHistorizableData<EForecast>(keywordName, ForecastCapacity, shared::plugin::yPluginApi::EKeywordAccessMode::kGet)
{
}

CForecast::~CForecast()
{
}




















} } } // namespace xplrules::rfxLanXpl::data

