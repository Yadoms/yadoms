#include "stdafx.h"
#include "UvDescription.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace xplrules
{
   namespace rfxLanXpl
   {
      namespace data
      {
         DECLARE_ENUM_IMPLEMENTATION(EUvDescription,
            ((Low)("low"))
            ((Medium)("medium"))
            ((High)("high"))
            ((VeryHigh)("very high"))
            ((Dangerous)("dangerous"))
         );

         DECLARE_CAPACITY(UvDescriptionCapacity, "UvDescription", shared::plugin::yPluginApi::CStandardUnits::NoUnits, shared::plugin::yPluginApi::EKeywordDataType::kNoData);


         CUvDescription::CUvDescription(const std::string& keywordName)
            : CSingleHistorizableData<EUvDescription>(keywordName,
                                                      UvDescriptionCapacity(),
                                                      shared::plugin::yPluginApi::EKeywordAccessMode::kGet)
         {
         }

         CUvDescription::~CUvDescription()
         {
         }
      }
   }
} // namespace xplrules::rfxLanXpl::data


