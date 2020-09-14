#include "stdafx.h"
#include "BoolTypeInfo.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace typeInfo
         {
            const CBoolTypeInfo CBoolTypeInfo::Empty;

            boost::shared_ptr<CDataContainer> CBoolTypeInfo::serialize() const
            {
               return shared::CDataContainer::make();
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization::typeInfo
