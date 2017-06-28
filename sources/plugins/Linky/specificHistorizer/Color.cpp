#include "stdafx.h"
#include "Color.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>
#include <shared/plugin/yPluginApi/StandardCapacities.h>

//TODO V2 : To be copy in a library teleInfo
namespace linky
{
   namespace specificHistorizers
   {
      DECLARE_ENUM_IMPLEMENTATION(EColor,
         ((NOTDEFINED))
         ((BLUE))
         ((WHITE))
         ((RED))
      );

      DECLARE_CAPACITY(ColorCapacity, "ForecastPrice", shared::plugin::yPluginApi::CStandardUnits::NoUnit(), shared::plugin::yPluginApi::EKeywordDataType::kEnum);

      CColor::CColor(const std::string& keywordName)
         : CSingleHistorizableData<EColor>(keywordName,
                                           ColorCapacity(),
                                           shared::plugin::yPluginApi::EKeywordAccessMode::kGet)
      {
      }

      CColor::~CColor()
      {
      }
   }
} // namespace teleInfo::specificHistorizer


