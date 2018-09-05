#include "stdafx.h"
#include "Kd101.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace specificHistorizers {
namespace outgoing {
      
   DECLARE_ENUM_IMPLEMENTATION(EKd101,
      ((Off)("OFF"))
      ((On)("ON"))
      ((Dim)("DIM"))
      ((Assoc)("ASSOC"))
   )


   DECLARE_CAPACITY(Kd101Capacity, "kd101_capacity", shared::plugin::yPluginApi::CStandardUnits::NoUnit(), shared::plugin::yPluginApi::EKeywordDataType::kEnum);

   CKd101Keyword::CKd101Keyword(const std::string& keywordName)
      : shared::plugin::yPluginApi::historization::CSingleHistorizableData<EKd101>(keywordName,
         Kd101Capacity(),
         shared::plugin::yPluginApi::EKeywordAccessMode::kGetSet)
   {
   }

   CKd101Keyword::~CKd101Keyword()
   {
   }

} //namespace outgoing
} //namespace specificHistorizers