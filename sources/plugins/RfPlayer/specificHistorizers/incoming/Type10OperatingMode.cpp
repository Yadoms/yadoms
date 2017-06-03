#include "stdafx.h"
#include "Type10OperatingMode.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace specificHistorizers {
namespace incoming {
      
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

   DECLARE_CAPACITY(Type10OperatingModeCapacity, "deltadoreX2dThermostat", yApi::CStandardUnits::NoUnits, yApi::EKeywordDataType::kEnum);

   CType10OperatingMode::CType10OperatingMode(const std::string& keywordName)
      : CSingleHistorizableData<EType10OperatingModeValues>(keywordName, Type10OperatingModeCapacity(), yApi::EKeywordAccessMode::kGet)
   {
   }

   CType10OperatingMode::~CType10OperatingMode()
   {
   }

} //namespace incoming
} //namespace specificHistorizers