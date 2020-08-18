#pragma once
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>


namespace specificHistorizers
{
   class CVolumicRadioActivity : public shared::plugin::yPluginApi::historization::CSingleHistorizableData<double>
   {
   public:
      explicit CVolumicRadioActivity(const std::string& keywordName);
      virtual ~CVolumicRadioActivity() = default;
   };
} // namespace specificHistorizers
