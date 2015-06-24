#include "stdafx.h"
#include "Switch.h"
#include "../StandardCapacities.h"
#include <shared/Log.h>

namespace shared { namespace plugin { namespace yPluginApi { namespace historization
{
   CSwitch::CSwitch(const std::string& keywordName, const EKeywordAccessMode& accessMode)
      :CSingleHistorizableData<bool>(keywordName, CStandardCapacities::Switch, accessMode, false, EMeasureType::kAbsolute)
   {
   }

   CSwitch::~CSwitch()
   {
   }

} } } } // namespace shared::plugin::yPluginApi::historization

