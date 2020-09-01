#pragma once
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace specificHistorizers
{
   class CConcentrationGramPerMeterCube : public yApi::historization::CSingleHistorizableData<double>
   {
   public:
      explicit CConcentrationGramPerMeterCube(const std::string& keywordName);
      virtual ~CConcentrationGramPerMeterCube() = default;
   };
} // namespace specificHistorizers
