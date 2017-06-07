#include "stdafx.h"
#include "Type0State.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace specificHistorizers {
namespace incoming {
      
   DECLARE_ENUM_IMPLEMENTATION(EType0StateValues,
      ((Off)("OFF"))
      ((On)("ON"))
      ((Bright)("BRIGHT"))
      ((Dim)("DIM"))
      ((AllOff)("ALL_OFF"))
      ((AllOn)("ALL_ON"))
   )

   DECLARE_CAPACITY(Type0StateCapacity, "x10_domiaLite_parrot", yApi::CStandardUnits::NoUnit(), yApi::EKeywordDataType::kEnum);

   CType0State::CType0State(const std::string& keywordName)
      : CSingleHistorizableData<EType0StateValues>(keywordName, Type0StateCapacity(), yApi::EKeywordAccessMode::kGet)
   {
   }

   CType0State::~CType0State()
   {
   }

} //namespace incoming
} //namespace specificHistorizers