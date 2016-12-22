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
        m_measureType(shared::plugin::yPluginApi::historization::EMeasureType(historizable.measure())),
        m_typeInfo(historizable.typeinfo().empty() ? shared::CDataContainer::EmptyContainer : shared::CDataContainer(historizable.typeinfo())),
        m_value(formatValue)
   {
   }

   CFromPluginHistorizer::~CFromPluginHistorizer()
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

   const shared::plugin::yPluginApi::historization::EMeasureType& CFromPluginHistorizer::getMeasureType() const
   {
      return m_measureType;
   }

   shared::CDataContainer CFromPluginHistorizer::getTypeInfo() const
   {
      return m_typeInfo;
   }
} // namespace pluginSystem	


