#include "stdafx.h"
#include "FromPluginHistorizer.h"

namespace pluginSystem
{
   CFromPluginHistorizer::CFromPluginHistorizer(const plugin_IPC::toYadoms::Historizable& historizable,
                                                const std::string& formatValue)
      : m_keyword(historizable.name()),
        m_capacity(shared::plugin::yPluginApi::CStandardCapacity(historizable.capacity().name(),
                                                                 historizable.capacity().unit(),
                                                                 shared::plugin::yPluginApi::EKeywordDataType(historizable.capacity().type()))),
        m_accessMode(shared::plugin::yPluginApi::EKeywordAccessMode(historizable.accessmode())),
        m_measureType(shared::plugin::yPluginApi::EMeasureType(historizable.measure())),
        m_typeInfo(historizable.typeinfo().empty() ? shared::CDataContainer::make() : shared::CDataContainer::make(historizable.typeinfo())),
        m_historyDepth(historizable.historydepth().empty()
                          ? shared::plugin::yPluginApi::EHistoryDepth::kDefault
                          : shared::plugin::yPluginApi::EHistoryDepth(historizable.historydepth())),
        m_value(formatValue)
   {
   }

   const std::string& CFromPluginHistorizer::getKeyword() const
   {
      return m_keyword;
   }

   const shared::plugin::yPluginApi::CStandardCapacity& CFromPluginHistorizer::getCapacity() const
   {
      return m_capacity;
   }

   const shared::plugin::yPluginApi::EKeywordAccessMode& CFromPluginHistorizer::getAccessMode() const
   {
      return m_accessMode;
   }

   std::string CFromPluginHistorizer::formatValue() const
   {
      return m_value;
   }

   const shared::plugin::yPluginApi::EMeasureType& CFromPluginHistorizer::getMeasureType() const
   {
      return m_measureType;
   }

   boost::shared_ptr<shared::CDataContainer> CFromPluginHistorizer::getTypeInfo() const
   {
      return m_typeInfo;
   }

   const shared::plugin::yPluginApi::EHistoryDepth& CFromPluginHistorizer::getHistoryDepth() const
   {
      return m_historyDepth;
   }
} // namespace pluginSystem	
