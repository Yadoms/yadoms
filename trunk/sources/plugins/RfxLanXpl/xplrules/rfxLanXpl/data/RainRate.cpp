#include "stdafx.h"
#include "RainRate.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace xplrules { namespace rfxLanXpl { namespace data {


const shared::plugin::yPluginApi::CStandardCapacity& RainRateCapacity = shared::plugin::yPluginApi::CStandardCapacity("RainRate", shared::plugin::yPluginApi::CStandardUnits::NoUnits, shared::plugin::yPluginApi::kGet, shared::plugin::yPluginApi::kNumeric);

   
CRainRate::CRainRate(const std::string& keywordName)
   :CHistorizableEnum<ERainRateCommand>(keywordName, RainRateCapacity, "command")
{
}

CRainRate::~CRainRate()
{
}




















} } } // namespace xplrules::rfxLanXpl::data

