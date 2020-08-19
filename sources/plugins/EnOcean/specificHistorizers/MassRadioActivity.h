#pragma once
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>


namespace specificHistorizers
{
   class CMassRadioActivity : public shared::plugin::yPluginApi::historization::CSingleHistorizableData<double>
   {
   public:
      explicit CMassRadioActivity(const std::string& keywordName);
      virtual ~CMassRadioActivity() = default;
   };
} // namespace specificHistorizers
