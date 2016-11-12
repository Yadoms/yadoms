#include "stdafx.h"
#include "Forecast.h"
#include <boost/lexical_cast.hpp>
#include "KeywordException.hpp"
#include "KeywordHelpers.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

CForecast::CForecast(const std::string& pluginName,
                     const std::string& keyWordName,
                     const weatherunderground::helper::EPeriod& period)
   : m_pluginName(pluginName),
     m_forecast(boost::make_shared<specificHistorizer::CForecastHistorizer>(keyWordName,
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

   auto it = weatherunderground::helper::EEnumTypeNames.find(valueContainer.get<std::string>(weatherCondition));
   if (it != weatherunderground::helper::EEnumTypeNames.end())
      weatherIconTemp = static_cast<yApi::historization::EWeatherCondition>(it->second).toString();
   else
      throw CKeywordException("Value [" + valueContainer.get<std::string>(weatherCondition) + "] could not be set");

   std::string stgmaxWind;
   std::string stgaveWind;
   std::string stgsnowday;
   
   convertKmhToMs(stgmaxWind, valueContainer, maxWind);
   convertKmhToMs(stgaveWind, valueContainer, aveWind);
   convertcmTomm(stgsnowday, valueContainer, snowDay);

   m_forecast->addPeriod(valueContainer.get<std::string>(year),
                           valueContainer.get<std::string>(month),
                           valueContainer.get<std::string>(day),
                           weatherIconTemp,
                           valueContainer.get<std::string>(tempMax),
                           valueContainer.get<std::string>(tempMin),
                           stgmaxWind,
                           stgaveWind,
                           valueContainer.get<std::string>(aveWindDegrees),
                           valueContainer.get<std::string>(aveHumidity),
                           valueContainer.get<std::string>(rainDay),
                           stgsnowday);
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