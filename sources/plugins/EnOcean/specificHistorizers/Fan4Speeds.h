#pragma once
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>
#include <shared/enumeration/EnumHelpers.hpp>


namespace specificHistorizers
{
   DECLARE_ENUM_HEADER(EFan4Speeds,
                       ((Auto)(0))
                       ((Speed0)(1))
                       ((Speed1)(2))
                       ((Speed2)(3))
                       ((Speed3)(4))
   ) ;

   class CFan4Speeds : public shared::plugin::yPluginApi::historization::CSingleHistorizableData<EFan4Speeds>
   {
   public:
      explicit CFan4Speeds(const std::string& keywordName);
      virtual ~CFan4Speeds() = default;
   };
} // namespace specificHistorizers
