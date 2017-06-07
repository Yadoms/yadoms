#include "stdafx.h"
#include "DigimaxDemand.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace xplrules
{
   namespace rfxLanXpl
   {
      namespace data
      {
         DECLARE_ENUM_IMPLEMENTATION(EDigimaxDemand,
            ((HeaterOn)("heater_on"))
            ((HeaterOff)("heater_off"))
            ((CoolingOn)("cooling_on"))
            ((CoolingOff)("cooling_off"))
         );


         DECLARE_CAPACITY(DigimaxDemandCapacity, "digimax_demand", shared::plugin::yPluginApi::CStandardUnits::NoUnits(), shared::plugin::yPluginApi::EKeywordDataType::kNoData);


         CDigimaxDemand::CDigimaxDemand(const std::string& keywordName)
            : CSingleHistorizableData<EDigimaxDemand>(keywordName,
                                                      DigimaxDemandCapacity(),
                                                      shared::plugin::yPluginApi::EKeywordAccessMode::kGetSet)
         {
         }

         CDigimaxDemand::~CDigimaxDemand()
         {
         }
      }
   }
} // namespace xplrules::rfxLanXpl::data


