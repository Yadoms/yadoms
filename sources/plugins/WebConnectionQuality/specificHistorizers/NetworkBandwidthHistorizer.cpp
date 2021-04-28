#include "stdafx.h"
#include "NetworkBandwidthHistorizer.h"

namespace specificHistorizers
{
   DECLARE_CAPACITY(NetworkBandwidthCapacity,
      "NetworkBandwith",
      "bit/s",
      shared::plugin::yPluginApi::EKeywordDataType::kNumeric);

   CNetworkBandwidthHistorizer::CNetworkBandwidthHistorizer(const std::string& keywordName)
      : CSingleHistorizableData<unsigned int>(keywordName,
                                              NetworkBandwidthCapacity(),
                                              shared::plugin::yPluginApi::EKeywordAccessMode::kGet)
   {
   }
} // namespace specificHistorizers
