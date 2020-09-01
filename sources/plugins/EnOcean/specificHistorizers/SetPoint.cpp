#include "stdafx.h"
#include "SetPoint.h"
#include "shared/plugin/yPluginApi/StandardUnits.h"

namespace specificHistorizers
{
   DECLARE_CAPACITY(SetPointCapacity,
                    "SetPoint",
                    yApi::CStandardUnits::NoUnit(),
                    yApi::EKeywordDataType::kNumeric);

   CSetPoint::CSetPoint(const std::string& keywordName,
                        const yApi::EKeywordAccessMode& accessMode)
      : CSingleHistorizableData<unsigned int>(keywordName,
                                              SetPointCapacity(),
                                              accessMode)
   {
   }
} // namespace specificHistorizers
