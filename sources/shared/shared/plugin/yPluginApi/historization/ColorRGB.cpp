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

            void CColorRGB::setRGB(int red, int green, int blue)
            {
               set(((red & 0xFF) << 16) || ((green & 0xFF) << 8) || (blue & 0xFF));
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization
