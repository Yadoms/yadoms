#include "stdafx.h"
#include "Type11State.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace specificHistorizers {
namespace incoming {
      
   DECLARE_ENUM_IMPLEMENTATION(EType11StateValues,
      ((On))
      ((Off))
      ((Stop))

   )

   DECLARE_CAPACITY(Type11StateCapacity, "deltadoreX2dAlarm", yApi::CStandardUnits::NoUnit(), yApi::EKeywordDataType::kEnum);

   CType11State::CType11State(const std::string& keywordName)
      : CSingleHistorizableData<EType11StateValues>(keywordName, Type11StateCapacity(), yApi::EKeywordAccessMode::kGet)
   {
   }

   CType11State::~CType11State()
   {
   }

} //namespace incoming
} //namespace specificHistorizers