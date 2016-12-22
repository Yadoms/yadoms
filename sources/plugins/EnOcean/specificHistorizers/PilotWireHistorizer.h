#pragma once
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>
#include <shared/enumeration/EnumHelpers.hpp>


namespace specificHistorizers
{
   DECLARE_ENUM_HEADER(EPilotWire,
      ((Off)(0))
      ((Comfort)(1))
      ((Eco)(2))
      ((AntiFreeze)(3))
   );

   class CPilotWireHistorizer : public shared::plugin::yPluginApi::historization::CSingleHistorizableData<EPilotWire>
   {
   public:
      explicit CPilotWireHistorizer(const std::string& keywordName);
      virtual ~CPilotWireHistorizer();
   };
} // namespace specificHistorizers
