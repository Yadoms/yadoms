#pragma once
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>
#include <shared/enumeration/EnumHelpers.hpp>


namespace specificHistorizers
{
   DECLARE_ENUM_HEADER(ESpecialHeatingStates,
                       ((DefinedBySetPoint)(0))
                       ((FrostProtection)(1))
                       ((AutomaticControl)(2))
   ) ;

   class CSpecialHeatingStates : public shared::plugin::yPluginApi::historization::CSingleHistorizableData<
         ESpecialHeatingStates>
   {
   public:
      explicit CSpecialHeatingStates(const std::string& keywordName);
      virtual ~CSpecialHeatingStates() = default;
   };
} // namespace specificHistorizers
