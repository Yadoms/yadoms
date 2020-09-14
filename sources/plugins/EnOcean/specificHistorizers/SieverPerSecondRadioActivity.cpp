#include "stdafx.h"
#include "SieverPerSecondRadioActivity.h"

namespace specificHistorizers
{
   static const std::string SieverPerSecondRadioActivityUnit("data.units.sieverPerSecond");

   DECLARE_CAPACITY(SieverPerSecondRadioActivityCapacity,
                    "SieverPerSecondRadioActivityCapacity",
                    SieverPerSecondRadioActivityUnit,
                    yApi::EKeywordDataType::kNumeric);

   CSieverPerSecondRadioActivity::CSieverPerSecondRadioActivity(const std::string& keywordName)
      : CSingleHistorizableData<double>(keywordName,
                                        SieverPerSecondRadioActivityCapacity(),
                                        yApi::EKeywordAccessMode::kGet)
   {
   }
} // namespace specificHistorizers
