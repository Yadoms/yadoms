#pragma once
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>
#include <shared/enumeration/EnumHelpers.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace specificHistorizers
{
   DECLARE_ENUM_HEADER(EDimmerMode,
                       ((SwitchToValue)(0))
                       ((DimToValueWithTimer1)(1))
                       ((DimToValueWithTimer2)(2))
                       ((DimToValueWithTimer3)(3))
                       ((StopDimming)(4))
   ) ;

   class CDimmerModeHistorizer final : public yApi::historization::CSingleHistorizableData<EDimmerMode>
   {
   public:
      explicit CDimmerModeHistorizer(const std::string& keywordName);
      ~CDimmerModeHistorizer() override = default;
   };
} // namespace specificHistorizers
