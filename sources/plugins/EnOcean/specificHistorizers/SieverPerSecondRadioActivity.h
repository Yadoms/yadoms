#pragma once
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>


namespace specificHistorizers
{
   class CSieverPerSecondRadioActivity : public shared::plugin::yPluginApi::historization::CSingleHistorizableData<double>
   {
   public:
      explicit CSieverPerSecondRadioActivity(const std::string& keywordName);
      virtual ~CSieverPerSecondRadioActivity() = default;
   };
} // namespace specificHistorizers
