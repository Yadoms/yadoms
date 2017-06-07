#include "stdafx.h"
#include "X10.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace specificHistorizers {
namespace outgoing {
      
   DECLARE_ENUM_IMPLEMENTATION(EX10,
      ((Off)("OFF"))
      ((On)("ON"))
      ((Dim)("DIM"))
      ((AllOff)("ALL_OFF"))
      ((AllOn)("ALL_ON"))
      ((Assoc)("ASSOC"))
   )


   DECLARE_CAPACITY(X10Capacity, "x10_capacity", shared::plugin::yPluginApi::CStandardUnits::NoUnit(), shared::plugin::yPluginApi::EKeywordDataType::kEnum);

   CX10Keyword::CX10Keyword(const std::string& keywordName)
      : shared::plugin::yPluginApi::historization::CSingleHistorizableData<EX10>(keywordName,
         X10Capacity(),
         shared::plugin::yPluginApi::EKeywordAccessMode::kGetSet)
   {
   }

   CX10Keyword::~CX10Keyword()
   {
   }

} //namespace outgoing
} //namespace specificHistorizers