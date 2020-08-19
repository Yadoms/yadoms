#pragma once
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>


namespace specificHistorizers
{
   class CCountPerSecondRadioActivity : public shared::plugin::yPluginApi::historization::CSingleHistorizableData<double>
   {
   public:
      explicit CCountPerSecondRadioActivity(const std::string& keywordName);
      virtual ~CCountPerSecondRadioActivity() = default;
   };
} // namespace specificHistorizers
