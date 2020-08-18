#include "stdafx.h"
#include "ConcentrationGramPerMeterCube.h"

namespace specificHistorizers
{
   static const std::string GramPerMeterCubeUnit("data.units.gramPerMeterCube");

   DECLARE_CAPACITY(ConcentrationGramPerMeterCubeCapacity,
                    "ConcentrationGramPerMeterCubeCapacity",
                    GramPerMeterCubeUnit,
                    shared::plugin::yPluginApi::EKeywordDataType::kNumeric);

   CConcentrationGramPerMeterCube::CConcentrationGramPerMeterCube(const std::string& keywordName)
      : CSingleHistorizableData<double>(keywordName,
                                        ConcentrationGramPerMeterCubeCapacity(),
                                        shared::plugin::yPluginApi::EKeywordAccessMode::kGet)
   {
   }
} // namespace specificHistorizers
