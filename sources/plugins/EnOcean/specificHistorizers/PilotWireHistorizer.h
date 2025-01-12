#pragma once
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>
#include <shared/enumeration/EnumHelpers.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace specificHistorizers
{
   DECLARE_ENUM_HEADER(EPilotWire,
      ((Off)(0))
      ((Comfort)(1))
      ((Comfort2)(2))
      ((Comfort3)(3))
      ((Eco)(4))
      ((AntiFreeze)(5))
   ) ;

   class CPilotWireHistorizer final : public yApi::historization::CSingleHistorizableData<EPilotWire>
   {
   public:
      explicit CPilotWireHistorizer(const std::string& keywordName);
      ~CPilotWireHistorizer() override = default;
   };
} // namespace specificHistorizers


