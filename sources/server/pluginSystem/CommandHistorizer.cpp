#include "stdafx.h"
#include "CommandHistorizer.h"
#include <shared/plugin/yadomsApi/StandardUnits.h>

namespace pluginSystem
{

   CCommandHistorizer::CCommandHistorizer(const std::string& keywordName, const std::string& value)
      :m_keywordName(keywordName), m_value(value)
   {
   }

   CCommandHistorizer::~CCommandHistorizer()
   {
   }

   const std::string& CCommandHistorizer::getKeyword() const
   {
      return m_keywordName;
   }

   const shared::plugin::yadomsApi::CStandardCapacity& CCommandHistorizer::getCapacity() const
   {
      static const shared::plugin::yadomsApi::CStandardCapacity NoCapacity("command", shared::plugin::yadomsApi::CStandardUnits::NoUnits,
         shared::plugin::yadomsApi::kSet, shared::plugin::yadomsApi::kString);
      return NoCapacity;
   }

   const std::string CCommandHistorizer::formatValue() const
   {
      return m_value;
   }

   shared::plugin::yadomsApi::commands::IHistorizable::EMeasureType CCommandHistorizer::getMeasureType() const
   {
      return kAbsolute;
   }

} // namespace pluginSystem	
