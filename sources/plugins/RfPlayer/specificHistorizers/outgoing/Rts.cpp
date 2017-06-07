#include "stdafx.h"
#include "Rts.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace specificHistorizers {
namespace outgoing {
      
   DECLARE_ENUM_IMPLEMENTATION(ERts,
      ((Off)("OFF"))
      ((On)("ON"))
      ((Dim)("DIM"))
      ((AllOff)("ALL_OFF"))
      ((AllOn)("ALL_ON"))
      ((Assoc)("ASSOC"))
   )


   DECLARE_CAPACITY(RtsCapacity, "blyss_capacity", shared::plugin::yPluginApi::CStandardUnits::NoUnits(), shared::plugin::yPluginApi::EKeywordDataType::kEnum);

   CRtsKeyword::CRtsKeyword(const std::string& keywordName)
      : shared::plugin::yPluginApi::historization::CSingleHistorizableData<ERts>(keywordName,
         RtsCapacity(),
         shared::plugin::yPluginApi::EKeywordAccessMode::kGetSet)
   {
   }

   CRtsKeyword::~CRtsKeyword()
   {
   }

} //namespace outgoing
} //namespace specificHistorizers