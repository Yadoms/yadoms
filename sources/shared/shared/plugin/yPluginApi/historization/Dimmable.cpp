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
            const typeInfo::CIntTypeInfo CDimmable::DimmableDefaultTypeInfo(0, 100, 1);

            CDimmable::CDimmable(const std::string& keywordName,
                                 const EKeywordAccessMode& accessMode,
                                 const EMeasureType& measureType,
                                 const typeInfo::CIntTypeInfo& additionalInfo,
                                 const EHistoryDepth& historyDepth)
               : CSingleHistorizableData<int>(keywordName,
                                              CStandardCapacities::Dimmable(),
                                              accessMode,
                                              0,
                                              measureType,
                                              additionalInfo,
                                              historyDepth)
            {
            }

            void CDimmable::set(const std::string& yadomsCommand)
            {
               set(IDeviceCommand::simpleNumericCommandHelperToInt(yadomsCommand));
            }

            void CDimmable::set(bool isOn)
            {
               if (isOn)
                  set(100);
               else
                  set(0);
            }

            void CDimmable::set(int switchLevel)
            {
               CSingleHistorizableData<int>::set(NormalizeLevel(switchLevel));
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
               return CSingleHistorizableData<int>::get();
            }

            bool CDimmable::isOn() const
            {
               return (CSingleHistorizableData<int>::get() >= 50) ? true : false;
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization
