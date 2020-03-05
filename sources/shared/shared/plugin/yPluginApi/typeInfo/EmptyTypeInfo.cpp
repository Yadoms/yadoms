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

            CDataContainerSharedPtr CEmptyTypeInfo::serialize() const
            {
               return new_CDataContainerSharedPtr();
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization::typeInfo
