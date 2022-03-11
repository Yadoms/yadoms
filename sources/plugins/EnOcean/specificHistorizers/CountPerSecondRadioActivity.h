#pragma once
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace specificHistorizers
{
   class CCountPerSecondRadioActivity final : public yApi::historization::CSingleHistorizableData<double>
   {
   public:
      explicit CCountPerSecondRadioActivity(const std::string& keywordName);
      ~CCountPerSecondRadioActivity() override = default;
   };
} // namespace specificHistorizers
