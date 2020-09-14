#pragma once
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace specificHistorizers
{
   class CVolumicRadioActivity : public yApi::historization::CSingleHistorizableData<double>
   {
   public:
      explicit CVolumicRadioActivity(const std::string& keywordName);
      virtual ~CVolumicRadioActivity() = default;
   };
} // namespace specificHistorizers
