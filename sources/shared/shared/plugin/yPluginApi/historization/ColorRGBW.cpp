#include "stdafx.h"
#include "ColorRGBW.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            CColorRGBW::CColorRGBW(const std::string& keywordName,
                                   const EKeywordAccessMode& accessMode,
                                   const EHistoryDepth& historyDepth)
               : CSingleHistorizableData<Poco::UInt32>(keywordName,
                                                       CStandardCapacities::ColorRGBW(),
                                                       accessMode,
                                                       0,
                                                       EMeasureType::kAbsolute,
                                                       typeInfo::CEmptyTypeInfo::Empty,
                                                       historyDepth)
            {
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization
