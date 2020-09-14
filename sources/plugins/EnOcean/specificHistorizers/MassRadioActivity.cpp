#include "stdafx.h"
#include "MassRadioActivity.h"

namespace specificHistorizers
{
   static const std::string MassRadioActivityUnit("data.units.becquerelPerGram");

   DECLARE_CAPACITY(MassRadioActivityCapacity,
                    "MassRadioActivityCapacity",
                    MassRadioActivityUnit,
                    yApi::EKeywordDataType::kNumeric);

   CMassRadioActivity::CMassRadioActivity(const std::string& keywordName)
      : CSingleHistorizableData<double>(keywordName,
                                        MassRadioActivityCapacity(),
                                        yApi::EKeywordAccessMode::kGet)
   {
   }
} // namespace specificHistorizers
