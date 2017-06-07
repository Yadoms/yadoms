#include "stdafx.h"
#include "CommandHistorizer.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace pluginSystem
{
   CCommandHistorizer::CCommandHistorizer(const std::string& keywordName, const std::string& value)
      : m_keywordName(keywordName),
        m_value(value)
   {
   }

   CCommandHistorizer::~CCommandHistorizer()
   {
   }

   const std::string& CCommandHistorizer::getKeyword() const
   {
      return m_keywordName;
   }

   const shared::plugin::yPluginApi::CStandardCapacity& CCommandHistorizer::getCapacity() const
   {
      static const shared::plugin::yPluginApi::CStandardCapacity NoCapacity("command", shared::plugin::yPluginApi::CStandardUnits::NoUnits(),
                                                                            shared::plugin::yPluginApi::EKeywordDataType::kString);
      return NoCapacity;
   }

   const shared::plugin::yPluginApi::EKeywordAccessMode& CCommandHistorizer::getAccessMode() const
   {
      return shared::plugin::yPluginApi::EKeywordAccessMode::kGetSet;
   }

   std::string CCommandHistorizer::formatValue() const
   {
      return m_value;
   }

   const shared::plugin::yPluginApi::historization::EMeasureType& CCommandHistorizer::getMeasureType() const
   {
      static const auto MeasureType(shared::plugin::yPluginApi::historization::EMeasureType::kAbsolute);
      return MeasureType;
   }

   shared::CDataContainer CCommandHistorizer::getTypeInfo() const
   {
      return shared::CDataContainer();
   }
} // namespace pluginSystem	


