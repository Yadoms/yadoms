#include "stdafx.h"
#include "Weather1.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CWeather1::CWeather1()
      : m_averageWind(boost::make_shared<yApi::historization::CSpeed>("averageWind")),
        m_gustWind(boost::make_shared<yApi::historization::CSpeed>("gustWind")),
        m_temperature(boost::make_shared<yApi::historization::CTemperature>("temperature")),
        m_rainTotal(boost::make_shared<yApi::historization::CRain>("rainTotal")),
        m_keywords({m_averageWind, m_gustWind, m_temperature, m_rainTotal})
   {
   }

   std::string CWeather1::getModel() const
   {
      return "Alecto ACH2010";
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CWeather1::keywords() const
   {
      return m_keywords;
   }

   void CWeather1::setFromProtocolState(unsigned short windDirection,
                                        unsigned short windSpeed,
                                        unsigned short windGust,
                                        short temperature,
                                        short chill,
                                        unsigned char humidity,
                                        unsigned short rainRate,
                                        unsigned int rainTotal,
                                        unsigned char uv,
                                        unsigned short solar,
                                        unsigned short pressure,
                                        EWeatherForecast forecast)
   {
      m_averageWind->set(static_cast<double>(windSpeed) / 10);
      m_gustWind->set(static_cast<double>(windGust) / 10);
      m_temperature->set(static_cast<double>(temperature) / 10);
      m_rainTotal->set(static_cast<double>(rainTotal) * 0.3);
   }
} // namespace rfxcomMessages
