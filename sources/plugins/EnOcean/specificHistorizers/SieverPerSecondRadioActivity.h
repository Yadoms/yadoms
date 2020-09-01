#pragma once
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace specificHistorizers
{
   class CSieverPerSecondRadioActivity : public yApi::historization::CSingleHistorizableData<double>
   {
   public:
      explicit CSieverPerSecondRadioActivity(const std::string& keywordName);
      virtual ~CSieverPerSecondRadioActivity() = default;
   };
} // namespace specificHistorizers
