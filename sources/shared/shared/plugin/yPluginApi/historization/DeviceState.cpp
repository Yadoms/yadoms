#include "stdafx.h"
#include "DeviceState.h"
#include "../StandardCapacity.h"
#include "../StandardUnits.h"


namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            DECLARE_ENUM_IMPLEMENTATION(EDeviceState,
               ((Unknown))
               ((Active))
               ((Busy))
               ((Asleep))
               ((Dead))
               ((Custom))
            );

            DECLARE_CAPACITY(DeviceStateCapacity, "deviceState", shared::plugin::yPluginApi::CStandardUnits::NoUnit(), shared::plugin::
               yPluginApi::EKeywordDataType::kEnum);

            CDeviceState::CDeviceState(const std::string& keywordName,
                                       const EKeywordAccessMode& accessMode,
                                       const EHistoryDepth& historyDepth)
               : CSingleHistorizableData<EDeviceState>(keywordName,
                                                       DeviceStateCapacity(),
                                                       accessMode,
                                                       EMeasureType::kAbsolute,
                                                       typeInfo::CEmptyTypeInfo::Empty,
                                                       historyDepth)
            {
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization
