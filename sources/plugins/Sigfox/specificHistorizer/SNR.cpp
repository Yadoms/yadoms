#include "stdafx.h"
#include "SNR.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>
#include <shared/plugin/yPluginApi/StandardCapacities.h>

namespace specificHistorizers
{
   // TODO : To create Db unit
   DECLARE_CAPACITY(SNRCapacity, "SNR", shared::plugin::yPluginApi::CStandardUnits::NoUnits, shared::plugin::yPluginApi::EKeywordDataType::kNoData);

   CSNR::CSNR(const std::string& keywordName)
      : CSingleHistorizableData<double>(keywordName,
                                     SNRCapacity(),
                                     shared::plugin::yPluginApi::EKeywordAccessMode::kGet)
   {
   }

   CSNR::~CSNR()
   {
   }
} // namespace specificHistorizers