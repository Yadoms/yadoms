#include "stdafx.h"
#include "NetworkBandwithHistorizer.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace specificHistorizers
{
   DECLARE_CAPACITY(NetworkBandwithCapacity,
      "NetworkBandwith",
      "bitsPerSecond",
      shared::plugin::yPluginApi::EKeywordDataType::kNumeric);

   CNetworkBandwithHistorizer::CNetworkBandwithHistorizer(const std::string& keywordName)
      : CSingleHistorizableData<unsigned int>(keywordName,
                                              NetworkBandwithCapacity(),
                                              shared::plugin::yPluginApi::EKeywordAccessMode::kGetSet)
   {
   }

   CNetworkBandwithHistorizer::~CNetworkBandwithHistorizer()
   {
   }
} // namespace specificHistorizers
