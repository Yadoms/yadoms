#include "stdafx.h"
#include "CountPerSecondRadioActivity.h"

namespace specificHistorizers
{
   static const std::string CountPerSecondRadioActivityUnit("data.units.countPerSecond");

   DECLARE_CAPACITY(CountPerSecondRadioActivityCapacity,
                    "CountPerSecondRadioActivityCapacity",
                    CountPerSecondRadioActivityUnit,
                    shared::plugin::yPluginApi::EKeywordDataType::kNumeric);

   CCountPerSecondRadioActivity::CCountPerSecondRadioActivity(const std::string& keywordName)
      : CSingleHistorizableData<double>(keywordName,
                                        CountPerSecondRadioActivityCapacity(),
                                        shared::plugin::yPluginApi::EKeywordAccessMode::kGet)
   {
   }
} // namespace specificHistorizers
