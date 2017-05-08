#include "stdafx.h"
#include "deviceStatus.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>
#include <shared/plugin/yPluginApi/StandardCapacities.h>

//TODO_V2 : To be copy in a library teleInfo
namespace specificHistorizers
{
   DECLARE_ENUM_IMPLEMENTATION(EdeviceStatus,
      ((Undefined))
      ((Ok))
      ((Error))
      ((TimeOut))
   );

   DECLARE_CAPACITY(WESdeviceCapacity, "Status", shared::plugin::yPluginApi::CStandardUnits::NoUnits, shared::plugin::yPluginApi::EKeywordDataType::kNoData);

   CdeviceStatus::CdeviceStatus(const std::string& keywordName)
      : CSingleHistorizableData<EdeviceStatus>(keywordName,
                                          WESdeviceCapacity(),
                                          shared::plugin::yPluginApi::EKeywordAccessMode::kGet)
   {
   }

   CdeviceStatus::~CdeviceStatus()
   {}
} // namespace specificHistorizers