#include "stdafx.h"
#include "SetPoint.h"
#include "shared/plugin/yPluginApi/StandardUnits.h"

namespace specificHistorizers
{
   DECLARE_CAPACITY(SetPointCapacity,
                    "SetPoint",
                    shared::plugin::yPluginApi::CStandardUnits::NoUnit(),
                    shared::plugin::yPluginApi::EKeywordDataType::kNumeric);

   CSetPoint::CSetPoint(const std::string& keywordName)
      : CSingleHistorizableData<unsigned int>(keywordName,
                                              SetPointCapacity(),
                                              shared::plugin::yPluginApi::EKeywordAccessMode::kGetSet)
   {
   }
} // namespace specificHistorizers
