#include "stdafx.h"
#include "TeleInfoStatus.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>
#include <shared/plugin/yPluginApi/StandardCapacities.h>

//TODO_V2 : To be copy in a library teleInfo
namespace teleInfo
{
   namespace specificHistorizers
   {
      DECLARE_ENUM_IMPLEMENTATION(EStatus,
         ((Ok))
         ((Error))
         ((Desactivated))
         );

      const shared::plugin::yPluginApi::CStandardCapacity& statusCapacity = shared::plugin::yPluginApi::CStandardCapacity("Status", shared::plugin::yPluginApi::CStandardUnits::NoUnits, shared::plugin::yPluginApi::EKeywordDataType::kNoData);

      CTeleInfoStatus::CTeleInfoStatus(const std::string& keywordName)
         : CSingleHistorizableData<EStatus>(keywordName, statusCapacity, shared::plugin::yPluginApi::EKeywordAccessMode::kGet)
      {}

      CTeleInfoStatus::~CTeleInfoStatus()
      {}
   }
} // namespace teleInfo::specificHistorizer


