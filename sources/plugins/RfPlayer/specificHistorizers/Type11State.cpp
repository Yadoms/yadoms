#include "stdafx.h"
#include "Type11State.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace specificHistorizers {
      
   DECLARE_ENUM_IMPLEMENTATION(EType11StateValues,
      ((On))
      ((Off))
      ((Stop))

   )


   const yApi::CStandardCapacity& Type11StateCapacity = yApi::CStandardCapacity("alarmRemoteControl", yApi::CStandardUnits::NoUnits, yApi::EKeywordDataType::kEnum);


   CType11State::CType11State(const std::string& keywordName)
      : CSingleHistorizableData<EType11StateValues>(keywordName, Type11StateCapacity, yApi::EKeywordAccessMode::kGetSet)
   {
   }

   CType11State::~CType11State()
   {
   }

} //namespace specificHistorizers