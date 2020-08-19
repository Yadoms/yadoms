#include "stdafx.h"
#include "SieverPerSecondRadioActivity.h"

namespace specificHistorizers
{
   static const std::string SieverPerSecondRadioActivityUnit("data.units.sieverPerSecond");

   DECLARE_CAPACITY(SieverPerSecondRadioActivityCapacity,
                    "SieverPerSecondRadioActivityCapacity",
                    SieverPerSecondRadioActivityUnit,
                    shared::plugin::yPluginApi::EKeywordDataType::kNumeric);

   CSieverPerSecondRadioActivity::CSieverPerSecondRadioActivity(const std::string& keywordName)
      : CSingleHistorizableData<double>(keywordName,
                                        SieverPerSecondRadioActivityCapacity(),
                                        shared::plugin::yPluginApi::EKeywordAccessMode::kGet)
   {
   }
} // namespace specificHistorizers
