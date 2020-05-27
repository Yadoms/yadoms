#include "stdafx.h"
#include "ColorRGB.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            CColorRGB::CColorRGB(const std::string& keywordName,
                                 const EKeywordAccessMode& accessMode,
                                 const EHistoryDepth& historyDepth)
               : CSingleHistorizableData<Poco::UInt32>(keywordName,
                                                       CStandardCapacities::ColorRGB(),
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
