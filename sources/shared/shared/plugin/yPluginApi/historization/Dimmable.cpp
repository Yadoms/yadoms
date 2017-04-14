#include "stdafx.h"
#include "Dimmable.h"
#include "../StandardCapacities.h"
#include <shared/plugin/yPluginApi/IDeviceCommand.h>

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            CDimmable::CDimmable(const std::string& keywordName,
                                 const EKeywordAccessMode& accessMode,
                                 const EMeasureType& measureType,
                                 typeInfo::CIntTypeInfo& additionalInfo)
               : m_keywordName(keywordName),
                 m_switchLevel(0),
                 m_measureType(measureType),
                 m_accessMode(accessMode),
                 m_additionalInfo(additionalInfo)
            {
            }

            CDimmable::~CDimmable()
            {
            }

            const std::string& CDimmable::getKeyword() const
            {
               return m_keywordName;
            }

            const CStandardCapacity& CDimmable::getCapacity() const
            {
               return CStandardCapacities::Dimmable();
            }

            const EKeywordAccessMode& CDimmable::getAccessMode() const
            {
               return m_accessMode;
            }

            void CDimmable::set(const std::string& yadomsCommand)
            {
               set(IDeviceCommand::simpleNumericCommandHelperToInt(yadomsCommand));
            }

            void CDimmable::set(bool isOn)
            {
               m_switchLevel = isOn ? 100 : 0;
            }

            void CDimmable::set(int switchLevel)
            {
               m_switchLevel = NormalizeLevel(switchLevel);
            }

            std::string CDimmable::formatValue() const
            {
               return boost::lexical_cast<std::string>(switchLevel());
            }

            int CDimmable::NormalizeLevel(int level)
            {
               if (level > 100)
                  return 100;
               if (level < 0)
                  return 0;
               return level;
            }

            int CDimmable::switchLevel() const
            {
               return m_switchLevel;
            }

            bool CDimmable::isOn() const
            {
               return (m_switchLevel >= 50) ? true : false;
            }

            const EMeasureType& CDimmable::getMeasureType() const
            {
               return m_measureType;
            }

            CDataContainer CDimmable::getTypeInfo() const
            {
               return m_additionalInfo.serialize();
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization


