#pragma once
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>


namespace specificHistorizers
{
   class CSetPoint : public shared::plugin::yPluginApi::historization::CSingleHistorizableData<unsigned int>
   {
   public:
      explicit CSetPoint(const std::string& keywordName);
      virtual ~CSetPoint() = default;
   };
} // namespace specificHistorizers
