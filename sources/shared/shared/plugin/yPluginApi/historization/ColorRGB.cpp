#include "stdafx.h"
#include "ColorRGB.h"
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
            CColorRGB::CColorRGB(const std::string& keywordName,  const EKeywordAccessMode& accessMode)
               : CSingleHistorizableData<unsigned int>(keywordName, CStandardCapacities::ColorRGB(), accessMode, 0)
            {
            }

            CColorRGB::~CColorRGB()
            {
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization


