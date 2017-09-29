#include "stdafx.h"
#include "PilotWireHistorizer.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace specificHistorizers
{
   DECLARE_ENUM_IMPLEMENTATION(EPilotWire,
      ((Off))
      ((Comfort))
      ((Comfort2))
      ((Comfort3))
      ((Eco))
      ((AntiFreeze))
   );


   CPilotWireHistorizer::CPilotWireHistorizer(const std::string& keywordName)
      : CSingleHistorizableData<EPilotWire>(keywordName,
                                            shared::plugin::yPluginApi::CStandardCapacity("PilotWire",
                                                                                          shared::plugin::yPluginApi::CStandardUnits::NoUnit(),
                                                                                          shared::plugin::yPluginApi::EKeywordDataType::kEnum),
                                            shared::plugin::yPluginApi::EKeywordAccessMode::kGetSet)
   {
   }

   CPilotWireHistorizer::~CPilotWireHistorizer()
   {
   }
} // namespace specificHistorizers


