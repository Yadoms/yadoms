#pragma once
#include "CustomEnum.h"
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace pluginSystem
{
   namespace internalPlugin
   {
      //-----------------------------------------------------
      ///\brief A custom enum historizer
      //-----------------------------------------------------
      class CCustomEnumHistorizer : public yApi::historization::CSingleHistorizableData<CCustomEnum>
      {
      public:
         CCustomEnumHistorizer(const std::string& keywordName,
                               const shared::plugin::yPluginApi::CStandardCapacity& capacity,
                               const shared::plugin::yPluginApi::EKeywordAccessMode& keywordAccessMode,
                               const std::vector<std::string>& values);
         virtual ~CCustomEnumHistorizer();
      };
   }
} // namespace pluginSystem::internalPlugin


