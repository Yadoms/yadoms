#include "stdafx.h"
#include "VolumicRadioActivity.h"

namespace specificHistorizers
{
   static const std::string VolumicRadioActivityUnit("data.units.becquerelPerMeterCube");

   DECLARE_CAPACITY(VolumicRadioActivityCapacity,
                    "VolumicRadioActivityCapacity",
                    VolumicRadioActivityUnit,
                    shared::plugin::yPluginApi::EKeywordDataType::kNumeric);

   CVolumicRadioActivity::CVolumicRadioActivity(const std::string& keywordName)
      : CSingleHistorizableData<double>(keywordName,
                                        VolumicRadioActivityCapacity(),
                                        shared::plugin::yPluginApi::EKeywordAccessMode::kGet)
   {
   }
} // namespace specificHistorizers
