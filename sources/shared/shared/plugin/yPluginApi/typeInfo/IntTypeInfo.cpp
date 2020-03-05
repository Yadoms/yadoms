#include "stdafx.h"
#include "IntTypeInfo.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace typeInfo
         {
            const CIntTypeInfo CIntTypeInfo::Empty;

            CIntTypeInfo::CIntTypeInfo(int minValue,
                                       int maxValue)
               : m_min(minValue, true),
                 m_max(maxValue, true)
            {
            }

            CIntTypeInfo::CIntTypeInfo(int minValue,
                                       int maxValue,
                                       int stepValue)
               : m_min(minValue, true),
                 m_max(maxValue, true),
                 m_step(stepValue, true)
            {
            }

            CIntTypeInfo& CIntTypeInfo::setMin(const int minValue)
            {
               m_min = minValue;
               return *this;
            }

            CIntTypeInfo& CIntTypeInfo::setMax(const int maxValue)
            {
               m_max = maxValue;
               return *this;
            }

            CIntTypeInfo& CIntTypeInfo::setStep(const int stepValue)
            {
               m_step = stepValue;
               return *this;
            }

            CDataContainerSharedPtr CIntTypeInfo::serialize() const
            {
               CDataContainerSharedPtr serializedData = new_CDataContainerSharedPtr();
               if (m_min.isDefined())
                  serializedData->set("min", m_min());
               if (m_max.isDefined())
                  serializedData->set("max", m_max());
               if (m_step.isDefined())
                  serializedData->set("step", m_step());
               return serializedData;
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization::typeInfo
