#include "stdafx.h"
#include "Domia.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace specificHistorizers {
namespace outgoing {
      
   DECLARE_ENUM_IMPLEMENTATION(EDomia,
      ((Off)("OFF"))
      ((On)("ON"))
      ((Dim)("DIM"))
      ((Assoc)("ASSOC"))
   )


   DECLARE_CAPACITY(DomiaCapacity, "blyss_capacity", shared::plugin::yPluginApi::CStandardUnits::NoUnit(), shared::plugin::yPluginApi::EKeywordDataType::kEnum);

   CDomiaKeyword::CDomiaKeyword(const std::string& keywordName)
      : shared::plugin::yPluginApi::historization::CSingleHistorizableData<EDomia>(keywordName,
         DomiaCapacity(),
         shared::plugin::yPluginApi::EKeywordAccessMode::kGetSet)
   {
   }

   CDomiaKeyword::~CDomiaKeyword()
   {
   }

} //namespace outgoing
} //namespace specificHistorizers