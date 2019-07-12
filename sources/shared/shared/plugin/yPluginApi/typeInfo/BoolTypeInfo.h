#pragma once
#include "ITypeInfo.h"
#include <shared/Export.h>

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace typeInfo
         {
            //-----------------------------------------------------
            ///\brief Interface for keyword type information
            //-----------------------------------------------------
            class YADOMS_SHARED_EXPORT CBoolTypeInfo : public ITypeInfo
            {
            public:
               //-----------------------------------------------------
               ///\brief   Empty container (can be with references)
               //-----------------------------------------------------
               static const CBoolTypeInfo Empty;

               CBoolTypeInfo() = default;
               virtual ~CBoolTypeInfo() = default;

               // ITypeInfo implementation 
               CDataContainer serialize() const override;
               // END ITypeInfo implementation 
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization::typeInfo
