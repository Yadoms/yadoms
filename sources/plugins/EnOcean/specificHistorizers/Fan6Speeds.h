#pragma once
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>
#include <shared/enumeration/EnumHelpers.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace specificHistorizers
{
   DECLARE_ENUM_HEADER(EFan6Speeds,
                       ((Off)(0))
                       ((Auto)(1))
                       ((Speed0)(2))
                       ((Speed1)(3))
                       ((Speed2)(4))
                       ((Speed3)(5))
                       ((Speed4)(6))
                       ((Speed5)(7))
   ) ;

   class CFan6Speeds : public yApi::historization::CSingleHistorizableData<EFan6Speeds>
   {
   public:
      explicit CFan6Speeds(const std::string& keywordName);
      virtual ~CFan6Speeds() = default;
   };
} // namespace specificHistorizers
