#include "stdafx.h"
#include "Switch.h"
#include "../StandardCapacities.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            CSwitch::CSwitch(const std::string& keywordName, const EKeywordAccessMode& accessMode, typeInfo::CBoolTypeInfo &ti)
               : CSingleHistorizableData<bool>(keywordName, CStandardCapacities::Switch, accessMode, false, EMeasureType::kAbsolute, ti)
            {
            }

            CSwitch::~CSwitch()
            {
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization


