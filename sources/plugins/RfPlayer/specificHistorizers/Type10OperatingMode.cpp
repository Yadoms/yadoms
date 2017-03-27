#include "stdafx.h"
#include "Type10OperatingMode.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace specificHistorizers {
      
   DECLARE_ENUM_IMPLEMENTATION(EType10OperatingModeValues,
      ((Eco))
      ((Moderate))
      ((Medio))
      ((Comfort))
      ((Stop))
      ((OutOfFrost))
      ((Special))
      ((Auto))
      ((Centralised))
   )

   DECLARE_CAPACITY(Type10OperatingModeCapacity, "shutterCode", yApi::CStandardUnits::NoUnits, yApi::EKeywordDataType::kEnum);

   CType10OperatingMode::CType10OperatingMode(const std::string& keywordName)
      : CSingleHistorizableData<EType10OperatingModeValues>(keywordName, Type10OperatingModeCapacity(), yApi::EKeywordAccessMode::kGetSet)
   {
   }

   CType10OperatingMode::~CType10OperatingMode()
   {
   }

} //namespace specificHistorizers