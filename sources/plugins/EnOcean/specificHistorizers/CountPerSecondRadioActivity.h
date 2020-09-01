#pragma once
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace specificHistorizers
{
   class CCountPerSecondRadioActivity : public yApi::historization::CSingleHistorizableData<double>
   {
   public:
      explicit CCountPerSecondRadioActivity(const std::string& keywordName);
      virtual ~CCountPerSecondRadioActivity() = default;
   };
} // namespace specificHistorizers
