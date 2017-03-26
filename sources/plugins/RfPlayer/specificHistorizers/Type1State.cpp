#include "stdafx.h"
#include "Type1State.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace specificHistorizers {
      
   DECLARE_ENUM_IMPLEMENTATION(EType1StateValues,
      ((Off)("OFF"))
      ((On)("ON/Alert"))
      ((AllOff)("ALL_OFF"))
      ((AllOn)("ALL_ON"))
   )


   const yApi::CStandardCapacity& Type1StateCapacity = yApi::CStandardCapacity("x10state", yApi::CStandardUnits::NoUnits, yApi::EKeywordDataType::kEnum);


   CType1State::CType1State(const std::string& keywordName)
      : CSingleHistorizableData<EType1StateValues>(keywordName, Type1StateCapacity, yApi::EKeywordAccessMode::kGetSet)
   {
   }

   CType1State::~CType1State()
   {
   }

} //namespace specificHistorizers