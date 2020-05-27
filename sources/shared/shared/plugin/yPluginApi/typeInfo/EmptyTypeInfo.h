#pragma once
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
            class CEmptyTypeInfo : public ITypeInfo
            {
            public:
               //-----------------------------------------------------
               ///\brief   Empty type info
               //-----------------------------------------------------
               static const CEmptyTypeInfo Empty;

               CEmptyTypeInfo() = default;
               virtual ~CEmptyTypeInfo() = default;

               boost::shared_ptr<CDataContainer> serialize() const override;
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization::typeInfo
