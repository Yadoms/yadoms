#include "stdafx.h"
#include "Analog.h"
#include "shared/plugin/yPluginApi/StandardUnits.h"
#include "shared/plugin/yPluginApi/typeInfo/IntTypeInfo.h"

namespace specificHistorizers
{
   DECLARE_CAPACITY(AnalogCapacity,
                    "Analog I/O",
                    yApi::CStandardUnits::NoUnit(),
                    yApi::EKeywordDataType::kNumeric);

   CAnalog::CAnalog(const std::string& keywordName,
                    unsigned int min,
                    unsigned int max,
                    yApi::EKeywordAccessMode accessMode)
      : CSingleHistorizableData<unsigned int>(keywordName,
                                              AnalogCapacity(),
                                              accessMode,
                                              yApi::EMeasureType::kAbsolute,
                                              yApi::typeInfo::CIntTypeInfo(min, max))
   {
   }
} // namespace specificHistorizers
