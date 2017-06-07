#include "stdafx.h"
#include "Visonic.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace specificHistorizers {
namespace outgoing {
      
   DECLARE_ENUM_IMPLEMENTATION(EVisonic,
      ((Off)("OFF"))
      ((On)("ON"))
      ((Dim)("DIM"))
      ((Assoc)("ASSOC"))
   )


   DECLARE_CAPACITY(VisonicCapacity, "visonic_capacity", shared::plugin::yPluginApi::CStandardUnits::NoUnits(), shared::plugin::yPluginApi::EKeywordDataType::kEnum);

   CVisonicKeyword::CVisonicKeyword(const std::string& keywordName)
      : shared::plugin::yPluginApi::historization::CSingleHistorizableData<EVisonic>(keywordName,
         VisonicCapacity(),
         shared::plugin::yPluginApi::EKeywordAccessMode::kGetSet)
   {
   }

   CVisonicKeyword::~CVisonicKeyword()
   {
   }

} //namespace outgoing
} //namespace specificHistorizers