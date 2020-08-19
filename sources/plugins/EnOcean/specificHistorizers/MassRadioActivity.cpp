#include "stdafx.h"
#include "MassRadioActivity.h"

namespace specificHistorizers
{
   static const std::string MassRadioActivityUnit("data.units.becquerelPerGram");

   DECLARE_CAPACITY(MassRadioActivityCapacity,
                    "MassRadioActivityCapacity",
                    MassRadioActivityUnit,
                    shared::plugin::yPluginApi::EKeywordDataType::kNumeric);

   CMassRadioActivity::CMassRadioActivity(const std::string& keywordName)
      : CSingleHistorizableData<double>(keywordName,
                                        MassRadioActivityCapacity(),
                                        shared::plugin::yPluginApi::EKeywordAccessMode::kGet)
   {
   }
} // namespace specificHistorizers
