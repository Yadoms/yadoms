#pragma once
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>


namespace specificHistorizers
{
   class CConcentrationGramPerMeterCube : public shared::plugin::yPluginApi::historization::CSingleHistorizableData<double>
   {
   public:
      explicit CConcentrationGramPerMeterCube(const std::string& keywordName);
      virtual ~CConcentrationGramPerMeterCube() = default;
   };
} // namespace specificHistorizers
