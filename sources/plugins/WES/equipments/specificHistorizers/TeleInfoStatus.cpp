#include "stdafx.h"
#include "TeleInfoStatus.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>
#include <shared/plugin/yPluginApi/StandardCapacities.h>

namespace specificHistorizers
{
   DECLARE_ENUM_IMPLEMENTATION(EWESTeleInfoStatus,
      ((Ok))
      ((Error))
      ((Desactivated))
      ((FrameError))
      ((SignalLoss))
   );

   DECLARE_CAPACITY(StatusCapacity, "Status", shared::plugin::yPluginApi::CStandardUnits::NoUnit(), shared::plugin::yPluginApi::EKeywordDataType::kEnum);

   CTeleInfoStatus::CTeleInfoStatus(const std::string& keywordName)
      : CSingleHistorizableData<EWESTeleInfoStatus>(keywordName,
                                                    StatusCapacity(),
                                                    shared::plugin::yPluginApi::EKeywordAccessMode::kGet)
   {
   }

   CTeleInfoStatus::~CTeleInfoStatus()
   {
   }
} // namespace specificHistorizers