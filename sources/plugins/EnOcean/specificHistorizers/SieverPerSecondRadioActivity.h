#pragma once
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace specificHistorizers
{
   class CSieverPerSecondRadioActivity final : public yApi::historization::CSingleHistorizableData<double>
   {
   public:
      explicit CSieverPerSecondRadioActivity(const std::string& keywordName);
      ~CSieverPerSecondRadioActivity() override = default;
   };
} // namespace specificHistorizers
