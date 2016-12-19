#include "stdafx.h"
#include "Load.h"
#include "../StandardCapacities.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            CLoad::CLoad(const std::string& keywordName,
                         const EKeywordAccessMode& accessMode,
                         typeInfo::CDoubleTypeInfo& additionalInfo)
               : CSingleHistorizableData<double>(keywordName,
                                                 CStandardCapacities::Load(),
                                                 accessMode,
                                                 0.0,
                                                 EMeasureType::kAbsolute, additionalInfo)
            {
            }

            CLoad::~CLoad()
            {
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization


