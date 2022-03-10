#pragma once
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace specificHistorizers
{
   class CAnalog final : public yApi::historization::CSingleHistorizableData<unsigned int>
   {
   public:
      explicit CAnalog(const std::string& keywordName,
                       unsigned int min,
                       unsigned int max,
                       yApi::EKeywordAccessMode accessMode = yApi::EKeywordAccessMode::kGetSet);
      ~CAnalog() override = default;
   };
} // namespace specificHistorizers
