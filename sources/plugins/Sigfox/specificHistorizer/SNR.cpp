#include "stdafx.h"
#include "SNR.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>
#include <shared/plugin/yPluginApi/StandardCapacities.h>

namespace specificHistorizers
{
   DECLARE_CAPACITY(SNRCapacity, "SNR", shared::plugin::yPluginApi::CStandardUnits::NoUnits, shared::plugin::yPluginApi::EKeywordDataType::kNoData);

   CSNR::CSNR(const std::string& keywordName)
      : CSingleHistorizableData<int>(keywordName,
                                     SNRCapacity(),
                                     shared::plugin::yPluginApi::EKeywordAccessMode::kGet)
   {
   }

   CSNR::~CSNR()
   {
   }
} // namespace specificHistorizers