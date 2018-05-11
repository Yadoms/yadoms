#include "stdafx.h"
#include "UPSStatus.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>
#include <shared/plugin/yPluginApi/StandardCapacities.h>

//TODO V2 : To be copy in a library teleInfo
namespace nitram
{
   namespace specificHistorizers
   {
      DECLARE_ENUM_IMPLEMENTATION(EUPSState,
         ((Charging))
         ((Discharging))
         ((FullyCharged))
         );

      DECLARE_CAPACITY(UPSStatusCapacity, "Status", shared::plugin::yPluginApi::CStandardUnits::NoUnit(), shared::plugin::yPluginApi::EKeywordDataType::kEnum);

      CStatus::CStatus(const std::string& keywordName)
         : CSingleHistorizableData<EUPSState>(keywordName,
                                              UPSStatusCapacity(),
                                              shared::plugin::yPluginApi::EKeywordAccessMode::kGet)
      {
      }

      CStatus::~CStatus()
      {
      }
   }
} // namespace nitram::specificHistorizers


