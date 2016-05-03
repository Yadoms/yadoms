#include "stdafx.h"
#include "FromPluginHistorizer.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace pluginSystem
{
   CFromPluginHistorizer::CFromPluginHistorizer(const toYadoms::Historizable& historizable)
      : m_keyword(historizable.name()),
        m_accessMode(shared::plugin::yPluginApi::EKeywordAccessMode(historizable.accessmode())),
        m_measureType(shared::plugin::yPluginApi::historization::EMeasureType(historizable.measure())),
        m_typeInfo(historizable.typeinfo().empty() ? shared::CDataContainer::EmptyContainer : shared::CDataContainer(historizable.typeinfo())),
        m_value(historizable.has_formattedvalue() ? historizable.formattedvalue() : std::string())
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
      static const shared::plugin::yPluginApi::CStandardCapacity NoCapacity("CFromPluginHistorizer",
                                                                            shared::plugin::yPluginApi::CStandardUnits::NoUnits,
                                                                            shared::plugin::yPluginApi::EKeywordDataType::kString);
      return NoCapacity;
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


