#pragma once
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace specificHistorizers
{
   class CMassRadioActivity final : public yApi::historization::CSingleHistorizableData<double>
   {
   public:
      explicit CMassRadioActivity(const std::string& keywordName);
      ~CMassRadioActivity() override = default;
   };
} // namespace specificHistorizers
