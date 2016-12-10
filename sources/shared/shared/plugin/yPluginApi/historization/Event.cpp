#include "stdafx.h"
#include "Event.h"
#include "../StandardCapacities.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            CEvent::CEvent(const std::string& keywordName,
                           const EKeywordAccessMode& accessMode)
               : m_keywordName(keywordName),
                 m_accessMode(accessMode)
            {
            }

            CEvent::~CEvent()
            {
            }

            const std::string& CEvent::getKeyword() const
            {
               return m_keywordName;
            }

            const CStandardCapacity& CEvent::getCapacity() const
            {
               return CStandardCapacities::Event;
            }

            const EKeywordAccessMode& CEvent::getAccessMode() const
            {
               return m_accessMode;
            }

            std::string CEvent::formatValue() const
            {
               // No value
               return std::string();
            }

            const EMeasureType& CEvent::getMeasureType() const
            {
               static const EMeasureType MeasureType(EMeasureType::kAbsolute);
               return MeasureType;
            }

            CDataContainer CEvent::getTypeInfo() const
            {
               return CDataContainer();
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization


