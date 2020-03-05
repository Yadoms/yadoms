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

            CDataContainerSharedPtr CBoolTypeInfo::serialize() const
            {
               return new_CDataContainerSharedPtr();
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization::typeInfo
