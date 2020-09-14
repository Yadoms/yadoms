#pragma once
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>
#include <shared/enumeration/EnumHelpers.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace specificHistorizers
{
   DECLARE_ENUM_HEADER(EDayOfWeek,
                       ((Monday)(1))
                       ((Thuesday)(2))
                       ((Wednesday)(3))
                       ((Thursday)(4))
                       ((Friday)(5))
                       ((Saturday)(6))
                       ((Sunday)(7))
   ) ;

   class CDayOfWeek : public yApi::historization::CSingleHistorizableData<EDayOfWeek>
   {
   public:
      explicit CDayOfWeek(const std::string& keywordName);
      virtual ~CDayOfWeek() = default;
   };
} // namespace specificHistorizers
