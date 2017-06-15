#include "stdafx.h"
#include "CustomEnumHistorizer.h"
#include <shared/plugin/yPluginApi/KeywordAccessMode.h>
#include <shared/plugin/yPluginApi/StandardCapacity.h>
#include "CustomEnumTypeInfo.h"

namespace pluginSystem
{
   namespace internalPlugin
   {
      CCustomEnumHistorizer::CCustomEnumHistorizer(const std::string& keywordName,
                                                   const shared::plugin::yPluginApi::CStandardCapacity& capacity,
                                                   const shared::plugin::yPluginApi::EKeywordAccessMode& keywordAccessMode,
                                                   const std::vector<std::string>& values)
         : CSingleHistorizableData<CCustomEnum>(keywordName,
                                                capacity,
                                                keywordAccessMode,
                                                shared::plugin::yPluginApi::historization::EMeasureType::kAbsolute,
                                                CCustomEnumTypeInfo(capacity.getName(),
                                                                    values))
      {
      }

      CCustomEnumHistorizer::~CCustomEnumHistorizer()
      {
      }
   }
} // namespace pluginSystem::internalPlugin


