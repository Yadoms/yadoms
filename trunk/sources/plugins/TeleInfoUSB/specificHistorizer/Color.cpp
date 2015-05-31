#include "stdafx.h"
#include "Color.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>
#include <shared/plugin/yPluginApi/StandardCapacities.h>

namespace teleInfoUSB { namespace specificHistorizers
{

DECLARE_ENUM_IMPLEMENTATION(EColor,
      ((BLUE))
      ((WHITE))
	  ((RED))
);

const shared::plugin::yPluginApi::CStandardCapacity& ColorCapacity = shared::plugin::yPluginApi::CStandardCapacity("ForecastPrice", shared::plugin::yPluginApi::CStandardUnits::NoUnits, shared::plugin::yPluginApi::EKeywordDataType::kNoData);

CColor::CColor(const std::string& keywordName)
   :CSingleHistorizableData<EColor>(keywordName, ColorCapacity, shared::plugin::yPluginApi::EKeywordAccessMode::kGetSet)
{}

CColor::~CColor()
{}

} } // namespace teleInfoUSB::specificHistorizer

