#include "stdafx.h"
#include "Switch.h"
#include "../StandardCapacities.h"
#include <shared/Log.h>

namespace shared { namespace plugin { namespace yadomsApi { namespace historization
{
   CSwitch::CSwitch(const std::string& keywordName, const EKeywordAccessMode& accessMode)
      :CSingleHistorizableData<bool>(keywordName, CStandardCapacities::Switch, "state", accessMode, false, EMeasureType::kAbsolute)
   {
   }

   CSwitch::~CSwitch()
   {
   }

} } } } // namespace shared::plugin::yadomsApi::historization

