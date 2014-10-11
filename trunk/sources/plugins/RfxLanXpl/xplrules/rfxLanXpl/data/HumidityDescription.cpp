#include "stdafx.h"
#include "HumidityDescription.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>
#include <shared/plugin/yadomsApi/StandardUnits.h>

namespace xplrules { namespace rfxLanXpl { namespace data {

DECLARE_ENUM_IMPLEMENTATION_CUSTOM(EHumidityDescription,
   ((Normal)("normal"))
   ((Confort)("confort"))
   ((Dry)("dry"))
   ((Wet)("wet"))
);


   const shared::plugin::yadomsApi::CStandardCapacity& HumidityDescriptionCapacity = shared::plugin::yadomsApi::CStandardCapacity("humidityDescription", shared::plugin::yadomsApi::CStandardUnits::NoUnits, shared::plugin::yadomsApi::EKeywordAccessMode::kGet, shared::plugin::yadomsApi::EKeywordDataType::kNoData);

   
CHumidityDescription::CHumidityDescription(const std::string& keywordName)
   :CHistorizableEnum<EHumidityDescription>(keywordName, HumidityDescriptionCapacity, "command")
{
}

CHumidityDescription::~CHumidityDescription()
{
}




















} } } // namespace xplrules::rfxLanXpl::data

