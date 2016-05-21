#include "stdafx.h"
#include "CartelectronicTICColor.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>
#include <shared/plugin/yPluginApi/StandardCapacities.h>

namespace rfxcomMessages { namespace specificHistorizers
{

DECLARE_ENUM_IMPLEMENTATION(EColor,
      ((NOTDEFINED))
      ((BLUE))
      ((WHITE))
      ((RED))
);

const shared::plugin::yPluginApi::CStandardCapacity& ColorCapacity = shared::plugin::yPluginApi::CStandardCapacity("ForecastPrice", shared::plugin::yPluginApi::CStandardUnits::NoUnits, shared::plugin::yPluginApi::EKeywordDataType::kNoData);

CCartelectronicTICColor::CCartelectronicTICColor(const std::string& keywordName)
   :CSingleHistorizableData<EColor>(keywordName, ColorCapacity, shared::plugin::yPluginApi::EKeywordAccessMode::kGet)
{}

CCartelectronicTICColor::~CCartelectronicTICColor()
{}

} } // namespace teleInfoUSB::specificHistorizer

