#pragma once
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace specificHistorizers
{
   class CNetworkBandwidthHistorizer final : public yApi::historization::CSingleHistorizableData<unsigned int>
   {
   public:
      explicit CNetworkBandwidthHistorizer(const std::string& keywordName);
      ~CNetworkBandwidthHistorizer() = default;
   };
} // namespace specificHistorizers
