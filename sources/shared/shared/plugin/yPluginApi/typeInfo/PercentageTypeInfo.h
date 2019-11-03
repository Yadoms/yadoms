#pragma once
#include "IntTypeInfo.h"
#include <shared/Export.h>

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace typeInfo
         {
            class YADOMS_SHARED_EXPORT CPercentageTypeInfo : public ITypeInfo
            {
            public:
               CPercentageTypeInfo();
               virtual ~CPercentageTypeInfo() = default;

               // ITypeInfo implementation 
               CDataContainer serialize() const override;
               // END ITypeInfo implementation 
            private:
               const CIntTypeInfo m_intTypeInfo;
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization::typeInfo
