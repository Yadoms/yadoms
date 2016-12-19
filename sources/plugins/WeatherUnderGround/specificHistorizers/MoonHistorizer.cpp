#include "stdafx.h"
#include "MoonHistorizer.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            DECLARE_CAPACITY(MoonCapacity, "Moon", CStandardUnits::NoUnits, EKeywordDataType::kNoData);


            CMoonHistorizer::CMoonHistorizer(const std::string& keywordName,
                                             const EKeywordAccessMode& accessMode)
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

            const CStandardCapacity& CMoonHistorizer::getCapacity() const
            {
               return MoonCapacity();
            }

            const EKeywordAccessMode& CMoonHistorizer::getAccessMode() const
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

            const EMeasureType& CMoonHistorizer::getMeasureType() const
            {
               static const auto MeasureType(EMeasureType::kAbsolute);
               return MeasureType;
            }

            CDataContainer CMoonHistorizer::getTypeInfo() const
            {
               return CDataContainer();
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization


