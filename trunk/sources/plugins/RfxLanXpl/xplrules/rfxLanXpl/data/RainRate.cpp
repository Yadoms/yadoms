#include "stdafx.h"
#include "RainRate.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>
#include <shared/plugin/yadomsApi/StandardUnits.h>

namespace xplrules { namespace rfxLanXpl { namespace data {


const shared::plugin::yadomsApi::CStandardCapacity& RainRateCapacity = shared::plugin::yadomsApi::CStandardCapacity("RainRate", shared::plugin::yadomsApi::CStandardUnits::NoUnits, shared::plugin::yadomsApi::kGet, shared::plugin::yadomsApi::kNumeric);

   
CRainRate::CRainRate(const std::string& keywordName)
   :CHistorizableEnum<ERainRateCommand>(keywordName, RainRateCapacity, "command")
{
}

CRainRate::~CRainRate()
{
}




















} } } // namespace xplrules::rfxLanXpl::data

