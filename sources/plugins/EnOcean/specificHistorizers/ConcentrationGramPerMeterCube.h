#pragma once
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace specificHistorizers
{
   class CConcentrationGramPerMeterCube final : public yApi::historization::CSingleHistorizableData<double>
   {
   public:
      explicit CConcentrationGramPerMeterCube(const std::string& keywordName);
      ~CConcentrationGramPerMeterCube() override = default;
   };
} // namespace specificHistorizers
