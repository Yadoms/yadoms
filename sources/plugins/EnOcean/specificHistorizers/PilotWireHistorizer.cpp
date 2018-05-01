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
   
   DECLARE_CAPACITY(PilotWireCapacity,
      "PilotWire",
      shared::plugin::yPluginApi::CStandardUnits::NoUnit(),
      shared::plugin::yPluginApi::EKeywordDataType::kEnum);

   CPilotWireHistorizer::CPilotWireHistorizer(const std::string& keywordName)
      : CSingleHistorizableData<EPilotWire>(keywordName,
                                            PilotWireCapacity(),
                                            shared::plugin::yPluginApi::EKeywordAccessMode::kGetSet)
   {
   }

   CPilotWireHistorizer::~CPilotWireHistorizer()
   {
   }
} // namespace specificHistorizers


