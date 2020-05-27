#include "stdafx.h"
#include "EmptyTypeInfo.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace typeInfo
         {
            const CEmptyTypeInfo CEmptyTypeInfo::Empty;

            boost::shared_ptr<CDataContainer> CEmptyTypeInfo::serialize() const
            {
               return shared::CDataContainer::make();
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization::typeInfo
