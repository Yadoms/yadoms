#include "stdafx.h"
#include "deviceStatus.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>
#include <shared/plugin/yPluginApi/StandardCapacities.h>

namespace specificHistorizers
{
   DECLARE_ENUM_IMPLEMENTATION(EWESdeviceStatus,
      ((Undefined))
      ((Ok))
      ((Error))
      ((TimeOut))
   );

   DECLARE_CAPACITY(WESdeviceCapacity, "Status", shared::plugin::yPluginApi::CStandardUnits::NoUnit(), shared::plugin::yPluginApi::EKeywordDataType::kEnum);

   CdeviceStatus::CdeviceStatus(const std::string& keywordName)
      : CSingleHistorizableData<EWESdeviceStatus>(keywordName,
                                                  WESdeviceCapacity(),
                                                  shared::plugin::yPluginApi::EKeywordAccessMode::kGet)
   {
   }

   CdeviceStatus::~CdeviceStatus()
   {}
} // namespace specificHistorizers