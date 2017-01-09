#include "stdafx.h"
#include "MoonHistorizer.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace specificHistorizer
{
   const yApi::CStandardCapacity& MoonCapacity = yApi::CStandardCapacity("Moon",
                                                                         yApi::CStandardUnits::NoUnits,
                                                                         yApi::EKeywordDataType::kJson);


   CMoonHistorizer::CMoonHistorizer(const std::string& keywordName,
                                    const yApi::EKeywordAccessMode& accessMode)
      : m_keywordName(keywordName),
         m_accessMode(accessMode),
         m_content(boost::make_shared<CMoonFormatter>())
   {
   }

   CMoonHistorizer::~CMoonHistorizer()
   {
   }

   const std::string& CMoonHistorizer::getKeyword() const
   {
      return m_keywordName;
   }

   const yApi::CStandardCapacity& CMoonHistorizer::getCapacity() const
   {
      return MoonCapacity;
   }

   const yApi::EKeywordAccessMode& CMoonHistorizer::getAccessMode() const
   {
      return m_accessMode;
   }

   void CMoonHistorizer::addUnit(const std::string& unitName,
                                 const std::string& unitValue)
   {
      m_content->addUnit(unitName,
                           unitValue);
   }

   void CMoonHistorizer::setParameters(const std::string& illuminatedMoon,
                                       const std::string& dayofMoon)
   {
      m_content->setParameters(illuminatedMoon, dayofMoon);
   }

   std::string CMoonHistorizer::formatValue() const
   {
      if (!m_content)
         return std::string();

      return m_content->formatValue();
   }

   const yApi::historization::EMeasureType& CMoonHistorizer::getMeasureType() const
   {
      static const auto MeasureType(yApi::historization::EMeasureType::kAbsolute);
      return MeasureType;
   }

   shared::CDataContainer CMoonHistorizer::getTypeInfo() const
   {
      return shared::CDataContainer();
   }
}