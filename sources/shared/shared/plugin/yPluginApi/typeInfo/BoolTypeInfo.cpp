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

            CDataContainer CBoolTypeInfo::serialize() const
            {
               CDataContainer serializedData;
               return serializedData;
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization::typeInfo
