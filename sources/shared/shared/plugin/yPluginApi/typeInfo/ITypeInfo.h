#pragma once
#include <shared/DataContainer.h>

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
            class ITypeInfo
            {
            public:
               virtual ~ITypeInfo() = default;

               //-----------------------------------------------------
               ///\brief   Serialize
               ///\return  A data container of the typeInfo
               //-----------------------------------------------------
               virtual CDataContainerSharedPtr serialize() const = 0;
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization::typeInfo
