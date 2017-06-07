#include "stdafx.h"
#include "Type2KeyCode.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace specificHistorizers {
namespace incoming {
      
   DECLARE_ENUM_IMPLEMENTATION(EType2KeyCodeValues,
      ((Button1))
      ((Button2))
      ((Button3))
      ((Button4))
   )

   DECLARE_CAPACITY(Type2KeyCodeCapacity, "visonicKeyCode", yApi::CStandardUnits::NoUnits(), yApi::EKeywordDataType::kEnum);

   CType2KeyCode::CType2KeyCode(const std::string& keywordName)
      : CSingleHistorizableData<EType2KeyCodeValues>(keywordName, Type2KeyCodeCapacity(), yApi::EKeywordAccessMode::kGet)
   {
   }

   CType2KeyCode::~CType2KeyCode()
   {
   }

} //namespace incoming
} //namespace specificHistorizers