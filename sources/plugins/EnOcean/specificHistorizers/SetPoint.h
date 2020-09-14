#pragma once
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace specificHistorizers
{
   class CSetPoint : public yApi::historization::CSingleHistorizableData<unsigned int>
   {
   public:
      explicit CSetPoint(const std::string& keywordName,
                         const yApi::EKeywordAccessMode& accessMode = yApi::EKeywordAccessMode::kGetSet);
      virtual ~CSetPoint() = default;
   };
} // namespace specificHistorizers
