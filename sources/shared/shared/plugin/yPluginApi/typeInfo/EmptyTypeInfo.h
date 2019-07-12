#pragma once
#include <shared/Export.h>
#include <shared/DataContainer.h>
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
            class YADOMS_SHARED_EXPORT CEmptyTypeInfo : public ITypeInfo
            {
            public:
               //-----------------------------------------------------
               ///\brief   Empty type info
               //-----------------------------------------------------
               static const CEmptyTypeInfo Empty;

               CEmptyTypeInfo() = default;
               virtual ~CEmptyTypeInfo() = default;

               CDataContainer serialize() const override;
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization::typeInfo
