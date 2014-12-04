#include "stdafx.h"
#include "HumidityDescription.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace xplrules { namespace rfxLanXpl { namespace data {

DECLARE_ENUM_IMPLEMENTATION(EHumidityDescription,
   ((Normal))
   ((Confort))
   ((Dry))
   ((Wet))
);


   const shared::plugin::yPluginApi::CStandardCapacity& HumidityDescriptionCapacity = shared::plugin::yPluginApi::CStandardCapacity("humidityDescription", shared::plugin::yPluginApi::CStandardUnits::NoUnits, shared::plugin::yPluginApi::EKeywordDataType::kNoData);

   
CHumidityDescription::CHumidityDescription(const std::string& keywordName)
   :shared::plugin::yPluginApi::historization::CSingleHistorizableData<EHumidityDescription>(keywordName, HumidityDescriptionCapacity, shared::plugin::yPluginApi::EKeywordAccessMode::kGet)
{
}

CHumidityDescription::~CHumidityDescription()
{
}




















} } } // namespace xplrules::rfxLanXpl::data

