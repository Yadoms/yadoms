#include "stdafx.h"
#include "HumidityDescription.h"
#include <shared/plugin/yadomsApi/StandardUnits.h>

namespace xplrules { namespace rfxLanXpl { namespace data {

DECLARE_ENUM_IMPLEMENTATION(EHumidityDescription,
   ((Normal))
   ((Confort))
   ((Dry))
   ((Wet))
);


   const shared::plugin::yadomsApi::CStandardCapacity& HumidityDescriptionCapacity = shared::plugin::yadomsApi::CStandardCapacity("humidityDescription", shared::plugin::yadomsApi::CStandardUnits::NoUnits, shared::plugin::yadomsApi::EKeywordDataType::kNoData);

   
CHumidityDescription::CHumidityDescription(const std::string& keywordName)
   :shared::plugin::yadomsApi::historization::CSingleHistorizableData<EHumidityDescription>(keywordName, HumidityDescriptionCapacity, "command", shared::plugin::yadomsApi::EKeywordAccessMode::kGet)
{
}

CHumidityDescription::~CHumidityDescription()
{
}




















} } } // namespace xplrules::rfxLanXpl::data

