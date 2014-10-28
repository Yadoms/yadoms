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
         shared::plugin::yadomsApi::EKeywordDataType::kString);
      return NoCapacity;
   }

   const shared::plugin::yadomsApi::EKeywordAccessMode& CCommandHistorizer::getAccessMode() const
   {
      return shared::plugin::yadomsApi::EKeywordAccessMode::kGetSet;
   }

   const std::string CCommandHistorizer::formatValue() const
   {
      return m_value;
   }

   const shared::plugin::yadomsApi::historization::EMeasureType& CCommandHistorizer::getMeasureType() const
   {
      static const shared::plugin::yadomsApi::historization::EMeasureType MeasureType(shared::plugin::yadomsApi::historization::EMeasureType::kAbsolute);
      return MeasureType;
   }

} // namespace pluginSystem	
