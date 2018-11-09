#pragma once
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace specificHistorizers
{
   class CNetworkBandwithHistorizer : public yApi::historization::CSingleHistorizableData<unsigned int>
   {
   public:
      explicit CNetworkBandwithHistorizer(const std::string& keywordName);
      virtual ~CNetworkBandwithHistorizer();
   };
} // namespace specificHistorizers


