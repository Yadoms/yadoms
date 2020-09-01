#pragma once
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace specificHistorizers
{
   class CConcentrationPpm : public yApi::historization::CSingleHistorizableData<double>
   {
   public:
      explicit CConcentrationPpm(const std::string& keywordName);
      virtual ~CConcentrationPpm() = default;
   };
} // namespace specificHistorizers
