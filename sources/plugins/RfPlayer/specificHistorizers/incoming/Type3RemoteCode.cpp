#include "stdafx.h"
#include "Type3RemoteCode.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace specificHistorizers {
namespace incoming {
      
   DECLARE_ENUM_IMPLEMENTATION(EType3RemoteCodeValues,
      ((Left))
      ((Right))
   )

   DECLARE_CAPACITY(Type3RemoteCodeCapacity, "somfyRtsPortalsRemoteCode", yApi::CStandardUnits::NoUnits, yApi::EKeywordDataType::kEnum);

   CType3RemoteCode::CType3RemoteCode(const std::string& keywordName)
      : CSingleHistorizableData<EType3RemoteCodeValues>(keywordName, Type3RemoteCodeCapacity(), yApi::EKeywordAccessMode::kGet)
   {
   }

   CType3RemoteCode::~CType3RemoteCode()
   {
   }

} //namespace incoming
} //namespace specificHistorizers