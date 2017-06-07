#include "stdafx.h"
#include "X2D.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace specificHistorizers {
namespace outgoing {
      
   DECLARE_ENUM_IMPLEMENTATION(EX2D,
      ((Off)("OFF"))
      ((On)("ON"))
      ((Dim)("DIM"))
      ((AllOff)("ALL_OFF"))
      ((AllOn)("ALL_ON"))
      ((Assoc)("ASSOC"))
   )


   DECLARE_CAPACITY(X2DCapacity, "blyss_capacity", shared::plugin::yPluginApi::CStandardUnits::NoUnits(), shared::plugin::yPluginApi::EKeywordDataType::kEnum);

   CX2DKeyword::CX2DKeyword(const std::string& keywordName)
      : shared::plugin::yPluginApi::historization::CSingleHistorizableData<EX2D>(keywordName,
         X2DCapacity(),
         shared::plugin::yPluginApi::EKeywordAccessMode::kGetSet)
   {
   }

   CX2DKeyword::~CX2DKeyword()
   {
   }

} //namespace outgoing
} //namespace specificHistorizers