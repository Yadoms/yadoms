#include "stdafx.h"
#include "CommandHistorizer.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace pluginSystem
{
   CCommandHistorizer::CCommandHistorizer(boost::shared_ptr<const database::entities::CKeyword> keyword,
                                          const std::string& value)
      : m_keyword(keyword),
        m_value(value)
   {
   }

   const std::string& CCommandHistorizer::getKeyword() const
   {
      return m_keyword->Name();
   }

   const shared::plugin::yPluginApi::CStandardCapacity& CCommandHistorizer::getCapacity() const
   {
      static const shared::plugin::yPluginApi::CStandardCapacity NoCapacity("command", shared::plugin::yPluginApi::CStandardUnits::NoUnit(),
                                                                            shared::plugin::yPluginApi::EKeywordDataType::kString);
      return NoCapacity;
   }

   const shared::plugin::yPluginApi::EKeywordAccessMode& CCommandHistorizer::getAccessMode() const
   {
      return m_keyword->AccessMode();
   }

   std::string CCommandHistorizer::formatValue() const
   {
      return m_value;
   }

   const shared::plugin::yPluginApi::EMeasureType& CCommandHistorizer::getMeasureType() const
   {
      return m_keyword->Measure();
   }

   boost::shared_ptr<shared::CDataContainer> CCommandHistorizer::getTypeInfo() const
   {
      return shared::CDataContainer::make();
   }

   const shared::plugin::yPluginApi::EHistoryDepth& CCommandHistorizer::getHistoryDepth() const
   {
      return shared::plugin::yPluginApi::EHistoryDepth::kDefault;
   }
} // namespace pluginSystem	
