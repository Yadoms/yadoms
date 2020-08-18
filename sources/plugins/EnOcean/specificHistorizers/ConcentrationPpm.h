#pragma once
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>


namespace specificHistorizers
{
   class CConcentrationPpm : public shared::plugin::yPluginApi::historization::CSingleHistorizableData<double>
   {
   public:
      explicit CConcentrationPpm(const std::string& keywordName);
      virtual ~CConcentrationPpm() = default;
   };
} // namespace specificHistorizers
