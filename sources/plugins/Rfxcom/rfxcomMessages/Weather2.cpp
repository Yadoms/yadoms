#include "stdafx.h"
#include "Weather2.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CWeather2::CWeather2()
      : m_windDirection(boost::make_shared<yApi::historization::CDirection>("windDirection")),
        m_averageWind(boost::make_shared<yApi::historization::CSpeed>("averageWind")),
        m_gustWind(boost::make_shared<yApi::historization::CSpeed>("gustWind")),
        m_temperature(boost::make_shared<yApi::historization::CTemperature>("temperature")),
        m_humidity(boost::make_shared<yApi::historization::CHumidity>("humidity")),
        m_rainTotal(boost::make_shared<yApi::historization::CRain>("rainTotal")),
        m_uv(boost::make_shared<yApi::historization::CUv>("uv")),
        m_solar(boost::make_shared<yApi::historization::CIlluminationWm2>("illumination")),
        m_keywords({m_averageWind, m_gustWind, m_temperature, m_humidity, m_rainTotal, m_uv, m_solar})
   {
   }

   std::string CWeather2::getModel() const
   {
      return "Alecto WS5500, WH2900";
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CWeather2::keywords() const
   {
      return m_keywords;
   }

   void CWeather2::setFromProtocolState(unsigned short windDirection,
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
      m_windDirection->set(static_cast<short>(windDirection) - 180);
      m_averageWind->set(static_cast<double>(windSpeed) / 10);
      m_gustWind->set(static_cast<double>(windGust) / 10);
      m_temperature->set(static_cast<double>(temperature) / 10);
      m_humidity->set(static_cast<double>(humidity) / 10);
      m_rainTotal->set(static_cast<double>(rainTotal) * 0.3);
      m_uv->set(static_cast<double>(uv));
      m_solar->set(static_cast<double>(solar));
   }
} // namespace rfxcomMessages
