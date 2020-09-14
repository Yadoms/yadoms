#pragma once
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace specificHistorizers
{
   class CMassRadioActivity : public yApi::historization::CSingleHistorizableData<double>
   {
   public:
      explicit CMassRadioActivity(const std::string& keywordName);
      virtual ~CMassRadioActivity() = default;
   };
} // namespace specificHistorizers
