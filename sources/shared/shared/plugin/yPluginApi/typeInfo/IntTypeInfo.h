#pragma once
#include "ITypeInfo.h"
#include <shared/Field.hpp>

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace typeInfo
         {
            class CIntTypeInfo : public ITypeInfo
            {
            public:
               static const CIntTypeInfo Empty;

               CIntTypeInfo() = default;
               CIntTypeInfo(int minValue,
                            int maxValue);
               CIntTypeInfo(int minValue,
                            int maxValue,
                            int stepValue);
               virtual ~CIntTypeInfo() = default;

               CIntTypeInfo& setMin(int minValue);
               CIntTypeInfo& setMax(int maxValue);
               CIntTypeInfo& setStep(int stepValue);

               // ITypeInfo implementation 
               CDataContainer serialize() const override;
               // END ITypeInfo implementation 

            private:
               CField<int> m_min;
               CField<int> m_max;
               CField<int> m_step;
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization::typeInfo
