#include "stdafx.h"
#include "BoolTypeInfo.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            namespace typeInfo
            {
               CBoolTypeInfo CBoolTypeInfo::Empty;

               CBoolTypeInfo::CBoolTypeInfo()
               {
               }

               CBoolTypeInfo::~CBoolTypeInfo()
               {
               }

               CDataContainer CBoolTypeInfo::serialize() const
               {
                  CDataContainer serializedData;
                  return serializedData;
               }
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization::typeInfo


