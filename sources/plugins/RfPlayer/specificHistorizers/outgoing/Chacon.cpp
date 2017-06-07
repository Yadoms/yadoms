#include "stdafx.h"
#include "Chacon.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace specificHistorizers {
namespace outgoing {
      
   DECLARE_ENUM_IMPLEMENTATION(EChacon,
      ((Off)("OFF"))
      ((On)("ON"))
      ((Dim)("DIM"))
      ((AllOff)("ALL_OFF"))
      ((AllOn)("ALL_ON"))
      ((Assoc)("ASSOC"))
   )


   DECLARE_CAPACITY(ChaconCapacity, "chacon_capacity", shared::plugin::yPluginApi::CStandardUnits::NoUnits(), shared::plugin::yPluginApi::EKeywordDataType::kEnum);

   CChaconKeyword::CChaconKeyword(const std::string& keywordName)
      : shared::plugin::yPluginApi::historization::CSingleHistorizableData<EChacon>(keywordName,
         ChaconCapacity(),
         shared::plugin::yPluginApi::EKeywordAccessMode::kGetSet)
   {
   }

   CChaconKeyword::~CChaconKeyword()
   {
   }

} //namespace outgoing
} //namespace specificHistorizers