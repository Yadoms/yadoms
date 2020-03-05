#pragma once
#include "ITypeInfo.h"

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
            class CBoolTypeInfo : public ITypeInfo
            {
            public:
               //-----------------------------------------------------
               ///\brief   Empty container (can be with references)
               //-----------------------------------------------------
               static const CBoolTypeInfo Empty;

               CBoolTypeInfo() = default;
               virtual ~CBoolTypeInfo() = default;

               // ITypeInfo implementation 
               CDataContainerSharedPtr serialize() const override;
               // END ITypeInfo implementation 
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization::typeInfo
