#include "stdafx.h"
#include "ConcentrationPpm.h"

namespace specificHistorizers
{
   static const std::string PartPerMillionUnit("data.units.partsPerMillion");

   DECLARE_CAPACITY(ConcentrationPpmCapacity,
                    "ConcentrationPpm",
                    PartPerMillionUnit,
                    yApi::EKeywordDataType::kNumeric);

   CConcentrationPpm::CConcentrationPpm(const std::string& keywordName)
      : CSingleHistorizableData<double>(keywordName,
                                        ConcentrationPpmCapacity(),
                                        yApi::EKeywordAccessMode::kGet)
   {
   }
} // namespace specificHistorizers
