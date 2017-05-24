#include "stdafx.h"
#include "PilotWireHistorizer.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace specificHistorizers
{
   DECLARE_ENUM_IMPLEMENTATION(EPilotWire,
      ((Off))
      ((Comfort))
      ((Eco))
      ((AntiFreeze))
   );


   CPilotWireHistorizer::CPilotWireHistorizer(const std::string& keywordName)
      : CSingleHistorizableData<EPilotWire>(keywordName,
                                            shared::plugin::yPluginApi::CStandardCapacity("PilotWire", shared::plugin::yPluginApi::CStandardUnits::NoUnits(), shared::plugin::yPluginApi::EKeywordDataType::kNoData),
                                            shared::plugin::yPluginApi::EKeywordAccessMode::kGetSet)
   {
   }

   CPilotWireHistorizer::~CPilotWireHistorizer()
   {
   }
} // namespace specificHistorizers
