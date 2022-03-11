#pragma once
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>
#include <shared/enumeration/EnumHelpers.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace specificHistorizers
{
   DECLARE_ENUM_HEADER(EFan4Speeds,
                       ((Auto)(0))
                       ((Speed0)(1))
                       ((Speed1)(2))
                       ((Speed2)(3))
                       ((Speed3)(4))
   ) ;

   class CFan4Speeds final : public yApi::historization::CSingleHistorizableData<EFan4Speeds>
   {
   public:
      explicit CFan4Speeds(const std::string& keywordName,
                           yApi::EKeywordAccessMode accessMode = yApi::EKeywordAccessMode::kGetSet);
      ~CFan4Speeds() override = default;
   };
} // namespace specificHistorizers
