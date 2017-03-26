#include "stdafx.h"
#include "Type3ShutterCode.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace specificHistorizers {
      
   DECLARE_ENUM_IMPLEMENTATION(EType3ShutterCodeValues,
      ((DownOff))
      ((My))
      ((UpOn))
      ((Assoc))
   )


   const yApi::CStandardCapacity& Type3ShutterCodeCapacity = yApi::CStandardCapacity("shutterCode", yApi::CStandardUnits::NoUnits, yApi::EKeywordDataType::kEnum);


   CType3ShutterCode::CType3ShutterCode(const std::string& keywordName)
      : CSingleHistorizableData<EType3ShutterCodeValues>(keywordName, Type3ShutterCodeCapacity, yApi::EKeywordAccessMode::kGetSet)
   {
   }

   CType3ShutterCode::~CType3ShutterCode()
   {
   }

} //namespace specificHistorizers