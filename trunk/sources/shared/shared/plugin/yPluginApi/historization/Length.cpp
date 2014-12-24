#include "stdafx.h"
#include "Length.h"
#include "../StandardCapacities.h"

namespace shared { namespace plugin { namespace yPluginApi { namespace historization
{
   CLength::CLength(const std::string& keywordName, const EKeywordAccessMode& accessMode)
      :CSingleHistorizableData<double>(keywordName, CStandardCapacities::Length, accessMode, 0.0)
   {
   }

   CLength::~CLength()
   {
   }

} } } } // namespace shared::plugin::yPluginApi::historization
