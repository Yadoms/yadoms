#pragma once
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace specificHistorizers
{
   class CConcentrationPpm final : public yApi::historization::CSingleHistorizableData<double>
   {
   public:
      explicit CConcentrationPpm(const std::string& keywordName,
                                 yApi::EKeywordAccessMode accessMode = yApi::EKeywordAccessMode::kGet);
      ~CConcentrationPpm() override = default;
   };
} // namespace specificHistorizers
