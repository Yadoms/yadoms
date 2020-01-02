#include "stdafx.h"
#include "deviceStatus.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>
#include <shared/plugin/yPluginApi/StandardCapacities.h>

namespace specificHistorizers
{
   DECLARE_ENUM_IMPLEMENTATION(EdeviceStatus,
      ((Undefined))
      ((Ok))
      ((Error))
      ((TimeOut))
   );

   DECLARE_CAPACITY(deviceCapacity, "Status", shared::plugin::yPluginApi::CStandardUnits::NoUnit(), shared::plugin::yPluginApi::EKeywordDataType::kEnum);

   CdeviceStatus::CdeviceStatus(const std::string& keywordName)
      : CSingleHistorizableData<EdeviceStatus>(keywordName,
                                               deviceCapacity(),
                                               shared::plugin::yPluginApi::EKeywordAccessMode::kGet)
   {
   }

   CdeviceStatus::~CdeviceStatus()
   {}
} // namespace specificHistorizers