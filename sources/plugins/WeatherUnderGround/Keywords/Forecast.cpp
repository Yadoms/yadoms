#include "stdafx.h"
#include "Forecast.h"
#include <boost/lexical_cast.hpp>
#include "KeywordException.hpp"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

CForecast::CForecast(std::string pluginName,
                     std::string keyWordName,
                     const weatherunderground::helper::EPeriod& period)
   : m_pluginName(pluginName),
     m_forecast(boost::make_shared<yApi::historization::CForecastHistorizer>(keyWordName,
                                                                             yApi::EKeywordAccessMode::kGet,
                                                                             period))
{
}

CForecast::~CForecast()
{
}

void CForecast::addUnit(const std::string& unitName,
                        const std::string& unitValue) const
{
   m_forecast->addUnit(unitName,
                       unitValue);
}

void CForecast::addPeriod(const shared::CDataContainer& valueContainer,
                          const std::string& year,
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
   std::string weatherIconTemp;

   weatherunderground::helper::EnumValuesNames::const_iterator it = weatherunderground::helper::EEnumTypeNames.find(valueContainer.get<std::string>(weatherCondition));
   if (it != weatherunderground::helper::EEnumTypeNames.end())
   {
      weatherIconTemp = static_cast<yApi::historization::EWeatherCondition>(it->second).toString();
   }
   else
      throw CKeywordException("Value " + valueContainer.get<std::string>(weatherCondition) + " could not be set");

   m_forecast->addPeriod(valueContainer.get<std::string>(year),
                         valueContainer.get<std::string>(month),
                         valueContainer.get<std::string>(day),
                         weatherIconTemp,
                         valueContainer.get<std::string>(tempMax),
                         valueContainer.get<std::string>(tempMin),
                         boost::lexical_cast<std::string>(valueContainer.get<double>(maxWind) / 3.6), // Transform from Km/h -> m/s
                         boost::lexical_cast<std::string>(valueContainer.get<double>(aveWind) / 3.6), // Transform from Km/h -> m/s
                         valueContainer.get<std::string>(aveWindDegrees),
                         valueContainer.get<std::string>(aveHumidity),
                         valueContainer.get<std::string>(rainDay),
                         boost::lexical_cast<std::string>(valueContainer.get<double>(snowDay) * 10)); // Transform from cm -> mm
}

void CForecast::clearAllPeriods() const
{
   m_forecast->clearAllPeriods();
}

void CForecast::setCityName(const std::string& cityName) const
{
   m_forecast->setCityName(cityName);
}

boost::shared_ptr<yApi::historization::IHistorizable> CForecast::getHistorizable() const
{
   return m_forecast;
}

