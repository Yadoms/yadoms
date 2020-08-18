#include "stdafx.h"
#include "ConcentrationPpm.h"

namespace specificHistorizers
{
   static const std::string PartPerMillionUnit("data.units.partsPerMillion");

   DECLARE_CAPACITY(ConcentrationPpmCapacity,
                    "ConcentrationPpm",
                    PartPerMillionUnit,
                    shared::plugin::yPluginApi::EKeywordDataType::kNumeric);

   CConcentrationPpm::CConcentrationPpm(const std::string& keywordName)
      : CSingleHistorizableData<double>(keywordName,
                                        ConcentrationPpmCapacity(),
                                        shared::plugin::yPluginApi::EKeywordAccessMode::kGet)
   {
   }
} // namespace specificHistorizers
