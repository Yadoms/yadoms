#include "stdafx.h"
#include "ForecastHistorizer.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            DECLARE_CAPACITY(ForecastCapacity, "Forecast", CStandardUnits::NoUnits, EKeywordDataType::kNoData);

            CForecastHistorizer::CForecastHistorizer(const std::string& keywordName,
                                                     const EKeywordAccessMode& accessMode,
                                                     const weatherunderground::helper::EPeriod& period)
               : m_keywordName(keywordName),
                 m_accessMode(accessMode)
            {
               m_content = boost::make_shared<CForecastFormatter>(period);
            }

            CForecastHistorizer::~CForecastHistorizer()
            {
            }

            const std::string& CForecastHistorizer::getKeyword() const
            {
               return m_keywordName;
            }

            const CStandardCapacity& CForecastHistorizer::getCapacity() const
            {
               return ForecastCapacity();
            }

            const EKeywordAccessMode& CForecastHistorizer::getAccessMode() const
            {
               return m_accessMode;
            }

            void CForecastHistorizer::addUnit(const std::string& unitName,
                                              const std::string& unitValue)
            {
               m_content->addUnit(unitName,
                                  unitValue);
            }

            void CForecastHistorizer::addPeriod(const std::string& year,
                                                const std::string& month,
                                                const std::string& day,
                                                const std::string& weatherCondition,
                                                const std::string& tempMax,
                                                const std::string& tempMin,
                                                const std::string& maxWind,
                                                const std::string& aveWind,
                                                const std::string& aveWindDegrees,
                                                const std::string& aveHumidity,
                                                const std::string& rainDay,
                                                const std::string& snowDay)
            {
               m_content->addPeriod(year,
                                    month,
                                    day,
                                    weatherCondition,
                                    tempMax,
                                    tempMin,
                                    maxWind,
                                    aveWind,
                                    aveWindDegrees,
                                    aveHumidity,
                                    rainDay,
                                    snowDay);
            }

            void CForecastHistorizer::clearAllPeriods()
            {
               m_content->clearAllPeriods();
            }

            std::string CForecastHistorizer::formatValue() const
            {
               if (!m_content)
                  return std::string();

               return m_content->formatValue();
            }

            const EMeasureType& CForecastHistorizer::getMeasureType() const
            {
               static const auto MeasureType(EMeasureType::kAbsolute);
               return MeasureType;
            }

            CDataContainer CForecastHistorizer::getTypeInfo() const
            {
               return CDataContainer();
            }

            void CForecastHistorizer::setCityName(const std::string& cityName)
            {
               m_content->setCityName(cityName);
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization


