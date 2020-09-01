#pragma once
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>
#include <shared/enumeration/EnumHelpers.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace specificHistorizers
{
   DECLARE_ENUM_HEADER(EMechanicalHandleMovement,
                       ((ToUp)(0))
                       ((ToDown)(1))
                       ((ToHorizontal)(2))
   ) ;

   class CMechanicalHandleMovement : public yApi::historization::CSingleHistorizableData<
         EMechanicalHandleMovement>
   {
   public:
      explicit CMechanicalHandleMovement(const std::string& keywordName);
      virtual ~CMechanicalHandleMovement() = default;
   };
} // namespace specificHistorizers
