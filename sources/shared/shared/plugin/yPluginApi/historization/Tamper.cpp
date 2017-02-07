#include "stdafx.h"
#include "Tamper.h"
#include "../StandardCapacities.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            CTamper::CTamper(const std::string& keywordName, const EKeywordAccessMode& accessMode, typeInfo::CBoolTypeInfo &ti)
               : CSingleHistorizableData<bool>(keywordName, CStandardCapacities::Tamper, accessMode, false, EMeasureType::kAbsolute, ti)
            {
            }

            CTamper::~CTamper()
            {
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization


