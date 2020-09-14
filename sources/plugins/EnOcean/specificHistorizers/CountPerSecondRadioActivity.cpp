#include "stdafx.h"
#include "CountPerSecondRadioActivity.h"

namespace specificHistorizers
{
   static const std::string CountPerSecondRadioActivityUnit("data.units.countPerSecond");

   DECLARE_CAPACITY(CountPerSecondRadioActivityCapacity,
                    "CountPerSecondRadioActivityCapacity",
                    CountPerSecondRadioActivityUnit,
                    yApi::EKeywordDataType::kNumeric);

   CCountPerSecondRadioActivity::CCountPerSecondRadioActivity(const std::string& keywordName)
      : CSingleHistorizableData<double>(keywordName,
                                        CountPerSecondRadioActivityCapacity(),
                                        yApi::EKeywordAccessMode::kGet)
   {
   }
} // namespace specificHistorizers
