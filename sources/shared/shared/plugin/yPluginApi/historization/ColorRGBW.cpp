#include "stdafx.h"
#include "ColorRGBW.h"
#include <shared/DataContainer.h>
#include <Poco/RegularExpression.h>

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            CColorRGBW::CColorRGBW(const std::string& keywordName,  const EKeywordAccessMode& accessMode)
               : CSingleHistorizableData<Poco::UInt32>(keywordName, CStandardCapacities::ColorRGBW(), accessMode, 0)
            {
            }

            CColorRGBW::~CColorRGBW()
            {
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization


