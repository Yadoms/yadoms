#include "stdafx.h"
#include "DoubleTypeInfo.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace typeInfo
         {
            const CDoubleTypeInfo CDoubleTypeInfo::Empty;

            CDoubleTypeInfo& CDoubleTypeInfo::setMin(const double minValue)
            {
               m_min = minValue;
               return *this;
            }

            CDoubleTypeInfo& CDoubleTypeInfo::setMax(const double maxValue)
            {
               m_max = maxValue;
               return *this;
            }

            CDoubleTypeInfo& CDoubleTypeInfo::setStep(const double stepValue)
            {
               m_step = stepValue;
               return *this;
            }

            CDoubleTypeInfo& CDoubleTypeInfo::setPrecision(const double precisionValue)
            {
               m_precision = precisionValue;
               return *this;
            }

            boost::shared_ptr<CDataContainer> CDoubleTypeInfo::serialize() const
            {
               boost::shared_ptr<CDataContainer> serializedData = shared::CDataContainer::make();
               if (m_min.isDefined())
                  serializedData->set("min", m_min());
               if (m_max.isDefined())
                  serializedData->set("max", m_max());
               if (m_step.isDefined())
                  serializedData->set("step", m_step());
               if (m_precision.isDefined())
                  serializedData->set("precision", m_precision());
               return serializedData;
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization::typeInfo
