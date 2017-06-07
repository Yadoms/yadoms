#include "stdafx.h"
#include "Blyss.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace specificHistorizers {
namespace outgoing {
      
   DECLARE_ENUM_IMPLEMENTATION(EBlyss,
      ((Off)("OFF"))
      ((On)("ON"))
      ((Dim)("DIM"))
      ((AllOff)("ALL_OFF"))
      ((AllOn)("ALL_ON"))
      ((Assoc)("ASSOC"))
   )


   DECLARE_CAPACITY(BlyssCapacity, "blyss_capacity", shared::plugin::yPluginApi::CStandardUnits::NoUnits(), shared::plugin::yPluginApi::EKeywordDataType::kEnum);

   CBlyssKeyword::CBlyssKeyword(const std::string& keywordName)
      : shared::plugin::yPluginApi::historization::CSingleHistorizableData<EBlyss>(keywordName,
         BlyssCapacity(),
         shared::plugin::yPluginApi::EKeywordAccessMode::kGetSet)
   {
   }

   CBlyssKeyword::~CBlyssKeyword()
   {
   }

} //namespace outgoing
} //namespace specificHistorizers