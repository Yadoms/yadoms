#include "stdafx.h"
#include "Type3ShutterCode.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace specificHistorizers {
namespace incoming {
      
   DECLARE_ENUM_IMPLEMENTATION(EType3ShutterCodeValues,
      ((DownOff))
      ((My))
      ((UpOn))
      ((Assoc))
   )

   DECLARE_CAPACITY(Type3ShutterCodeCapacity, "somfyRtsPortalsShutterCode", yApi::CStandardUnits::NoUnits, yApi::EKeywordDataType::kEnum);

   CType3ShutterCode::CType3ShutterCode(const std::string& keywordName)
      : CSingleHistorizableData<EType3ShutterCodeValues>(keywordName, Type3ShutterCodeCapacity(), yApi::EKeywordAccessMode::kGet)
   {
   }

   CType3ShutterCode::~CType3ShutterCode()
   {
   }

} //namespace incoming
} //namespace specificHistorizers