#include "stdafx.h"
#include "ForecastHistorizer.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace specificHistorizer
{
   DECLARE_CAPACITY(ForecastCapacity, "Forecast", yApi::CStandardUnits::NoUnits(), yApi::EKeywordDataType::kJson);

   CForecastHistorizer::CForecastHistorizer(const std::string& keywordName,
                                            const yApi::EKeywordAccessMode& accessMode,
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

   const yApi::CStandardCapacity& CForecastHistorizer::getCapacity() const
   {
      return ForecastCapacity();
   }

   const yApi::EKeywordAccessMode& CForecastHistorizer::getAccessMode() const
   {
      return m_accessMode;
   }

   void CForecastHistorizer::addUnit(const std::string& unitName,
                                     const std::string& unitValue) const
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
                                       const std::string& snowDay) const
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

   void CForecastHistorizer::clearAllPeriods() const
   {
      m_content->clearAllPeriods();
   }

   std::string CForecastHistorizer::formatValue() const
   {
      if (!m_content)
         return std::string();

      return m_content->formatValue();
   }

   const yApi::historization::EMeasureType& CForecastHistorizer::getMeasureType() const
   {
      static const auto MeasureType(yApi::historization::EMeasureType::kAbsolute);
      return MeasureType;
   }

   shared::CDataContainer CForecastHistorizer::getTypeInfo() const
   {
      return shared::CDataContainer();
   }

   void CForecastHistorizer::setCityName(const std::string& cityName) const
   {
      m_content->setCityName(cityName);
   }
}