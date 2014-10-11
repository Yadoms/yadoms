#include "stdafx.h"
#include "Forecast.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>
#include <shared/plugin/yadomsApi/StandardUnits.h>

namespace xplrules { namespace rfxLanXpl { namespace data {

DECLARE_ENUM_IMPLEMENTATION_CUSTOM(EForecast,
   ((Sunny)("sunny"))
   ((PartlyCloudy)("partly cloudy"))
   ((Cloudy)("cloudy"))
   ((Rain)("rain"))
);


   const shared::plugin::yadomsApi::CStandardCapacity& ForecastCapacity = shared::plugin::yadomsApi::CStandardCapacity("forecast", shared::plugin::yadomsApi::CStandardUnits::NoUnits, shared::plugin::yadomsApi::EKeywordAccessMode::kGet, shared::plugin::yadomsApi::EKeywordDataType::kNoData);

   
CForecast::CForecast(const std::string& keywordName)
   :CHistorizableEnum<EForecast>(keywordName, ForecastCapacity, "command")
{
}

CForecast::~CForecast()
{
}




















} } } // namespace xplrules::rfxLanXpl::data

