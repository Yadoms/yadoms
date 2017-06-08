#include "stdafx.h"
#include "HumidityDescription.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace xplrules
{
   namespace rfxLanXpl
   {
      namespace data
      {
         DECLARE_ENUM_IMPLEMENTATION(EHumidityDescription,
            ((Normal))
            ((Confort))
            ((Dry))
            ((Wet))
         );


         DECLARE_CAPACITY(HumidityDescriptionCapacity, "humidityDescription", shared::plugin::yPluginApi::CStandardUnits::NoUnit(), shared::plugin::yPluginApi::EKeywordDataType::kNoData);


         CHumidityDescription::CHumidityDescription(const std::string& keywordName)
            : CSingleHistorizableData<EHumidityDescription>(keywordName,
                                                            HumidityDescriptionCapacity(),
                                                            shared::plugin::yPluginApi::EKeywordAccessMode::kGet)
         {
         }

         CHumidityDescription::~CHumidityDescription()
         {
         }
      }
   }
} // namespace xplrules::rfxLanXpl::data


