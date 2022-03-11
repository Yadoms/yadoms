#pragma once
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace specificHistorizers
{
   class CVolumicRadioActivity final : public yApi::historization::CSingleHistorizableData<double>
   {
   public:
      explicit CVolumicRadioActivity(const std::string& keywordName);
      ~CVolumicRadioActivity() override = default;
   };
} // namespace specificHistorizers
