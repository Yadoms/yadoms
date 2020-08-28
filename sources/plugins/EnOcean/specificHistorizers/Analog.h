#pragma once
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace specificHistorizers
{
   class CAnalog : public yApi::historization::CSingleHistorizableData<unsigned int>
   {
   public:
      explicit CAnalog(const std::string& keywordName,
                       unsigned int min,
                       unsigned int max,
                       yApi::EKeywordAccessMode accessMode = yApi::EKeywordAccessMode::kGetSet);
      virtual ~CAnalog() = default;
   };
} // namespace specificHistorizers
