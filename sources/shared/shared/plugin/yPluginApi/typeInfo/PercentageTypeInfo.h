#pragma once
#include "IntTypeInfo.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace typeInfo
         {
            class CPercentageTypeInfo : public ITypeInfo
            {
            public:
               CPercentageTypeInfo();
               virtual ~CPercentageTypeInfo() = default;

               // ITypeInfo implementation 
               boost::shared_ptr<CDataContainer> serialize() const override;
               // END ITypeInfo implementation 
            private:
               const CIntTypeInfo m_intTypeInfo;
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization::typeInfo
