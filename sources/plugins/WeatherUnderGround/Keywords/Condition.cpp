#include "stdafx.h"
#include "Condition.h"
#include <boost/lexical_cast.hpp>
#include "KeywordException.hpp"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

CCondition::CCondition(std::string pluginName,
                       std::string keyWordName)
   : m_pluginName(pluginName),
     m_condition(boost::make_shared<CConditionHistorizer>(keyWordName, yApi::EKeywordAccessMode::kGet))
{
}

CCondition::~CCondition()
{
}

void CCondition::addUnit(const std::string& unitName,
                         const std::string& unitValue) const
{
   m_condition->addUnit(unitName,
                        unitValue);
}

void CCondition::setPeriod(const shared::CDataContainer& valueContainer,
                           const std::string& filterTime,
                           const std::string& filterWeatherCondition,
                           const std::string& filterTemp,
                           const std::string& filterPressure,
                           const std::string& filterVisibility,
                           const std::string& filterUV,
                           const std::string& filterDewPoint,
                           const std::string& filterMaxWind,
                           const std::string& filterAveWind,
                           const std::string& filterAveWindDegrees,
                           const std::string& filterAveHumidity,
                           const std::string& filterRainDay,
                           const std::string& filterFeelslike,
                           const std::string& filterWindChill) const
{
   std::string weatherIconTemp;

   auto it = weatherunderground::helper::EEnumTypeNames.find(valueContainer.get<std::string>(filterWeatherCondition));
   if (it != weatherunderground::helper::EEnumTypeNames.end())
   {
      weatherIconTemp = static_cast<yApi::historization::EWeatherCondition>(it->second).toString();
   }
   else
      throw CKeywordException("Value " + valueContainer.get<std::string>(filterWeatherCondition) + " could not be set");

   m_condition->setPeriod(valueContainer.get<std::string>(filterTime),
                          weatherIconTemp,
                          valueContainer.get<std::string>(filterTemp),
                          valueContainer.get<std::string>(filterPressure),
                          valueContainer.get<std::string>(filterVisibility),
                          valueContainer.get<std::string>(filterUV),
                          valueContainer.get<std::string>(filterDewPoint),
                          boost::lexical_cast<std::string>(valueContainer.get<double>(filterMaxWind) / 3.6), // Transform from Km/h -> m/s
                          boost::lexical_cast<std::string>(valueContainer.get<double>(filterAveWind) / 3.6), // Transform from Km/h -> m/s
                          valueContainer.get<std::string>(filterAveWindDegrees),
                          valueContainer.get<std::string>(filterAveHumidity),
                          valueContainer.get<std::string>(filterRainDay),
                          valueContainer.get<std::string>(filterFeelslike),
                          valueContainer.get<std::string>(filterWindChill));
}

void CCondition::setCityName(const std::string& cityName) const
{
   m_condition->setCityName(cityName);
}

boost::shared_ptr<yApi::historization::IHistorizable> CCondition::getHistorizable() const
{
   return m_condition;
}

