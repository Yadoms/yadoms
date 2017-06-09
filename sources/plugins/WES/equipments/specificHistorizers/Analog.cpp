#include "stdafx.h"
#include "Analog.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>
#include <shared/plugin/yPluginApi/StandardCapacity.h>

namespace specificHistorizers
{
   DECLARE_CAPACITY(analogCapacity, "analog", yApi::CStandardUnits::NoUnit(), yApi::EKeywordDataType::kNumeric);

   CAnalog::CAnalog(const std::string& keywordName,
                    const yApi::EKeywordAccessMode& accessMode,
                    const yApi::historization::EMeasureType& measureType,
                    yApi::historization::typeInfo::ITypeInfo& additionalInfo)
      : CSingleHistorizableData<unsigned int>(keywordName,
                                              analogCapacity(),
                                              accessMode,
                                              0,
                                              measureType,
                                              additionalInfo)
   {
   }

   CAnalog::~CAnalog()
   {
   }
}

