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

            CDataContainer CEmptyTypeInfo::serialize() const
            {
               return CDataContainer();
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization::typeInfo
