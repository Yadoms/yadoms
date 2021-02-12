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

            void CColorRGBW::setRGBW(int red, int green, int blue, int white)
            {
               set((red & 0xFF) << 24 | (green & 0xFF) << 16 | (blue & 0xFF) << 8 | white & 0xFF);
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization
