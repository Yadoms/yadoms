#include "stdafx.h"
#include "Forecast.h"
#include <shared/plugin/yadomsApi/StandardUnits.h>

namespace xplrules { namespace rfxLanXpl { namespace data {

DECLARE_ENUM_IMPLEMENTATION(EForecast,
   ((Sunny)("sunny"))
   ((PartlyCloudy)("partly cloudy"))
   ((Cloudy)("cloudy"))
   ((Rain)("rain"))
);


const shared::plugin::yadomsApi::CStandardCapacity& ForecastCapacity = shared::plugin::yadomsApi::CStandardCapacity("forecast", shared::plugin::yadomsApi::CStandardUnits::NoUnits, shared::plugin::yadomsApi::EKeywordDataType::kNoData);

   
CForecast::CForecast(const std::string& keywordName)
   :shared::plugin::yadomsApi::historization::CSingleHistorizableData<EForecast>(keywordName, ForecastCapacity, "command", shared::plugin::yadomsApi::EKeywordAccessMode::kGet)
{
}

CForecast::~CForecast()
{
}




















} } } // namespace xplrules::rfxLanXpl::data

