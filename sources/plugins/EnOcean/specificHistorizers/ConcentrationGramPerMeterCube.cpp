#include "stdafx.h"
#include "ConcentrationGramPerMeterCube.h"

namespace specificHistorizers
{
   static const std::string GramPerMeterCubeUnit("data.units.gramPerMeterCube");

   DECLARE_CAPACITY(ConcentrationGramPerMeterCubeCapacity,
                    "ConcentrationGramPerMeterCubeCapacity",
                    GramPerMeterCubeUnit,
                    yApi::EKeywordDataType::kNumeric);

   CConcentrationGramPerMeterCube::CConcentrationGramPerMeterCube(const std::string& keywordName)
      : CSingleHistorizableData<double>(keywordName,
                                        ConcentrationGramPerMeterCubeCapacity(),
                                        yApi::EKeywordAccessMode::kGet)
   {
   }
} // namespace specificHistorizers
