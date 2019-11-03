#include "stdafx.h"
#include "LiveWeatherDevice.h"


CLiveWeatherDevice::CLiveWeatherDevice(std::string deviceName)
   : m_deviceName(std::move(deviceName)),
     m_condition(boost::make_shared<yApi::historization::CWeatherCondition>("Condition")),
     m_temperature(boost::make_shared<yApi::historization::CTemperature>("Temperature")),
     m_temperatureMin(boost::make_shared<yApi::historization::CTemperature>("Temperature Min")),
     m_temperatureMax(boost::make_shared<yApi::historization::CTemperature>("Temperature Max")),
     m_humidity(boost::make_shared<yApi::historization::CHumidity>("Humidity")),
     m_pressure(boost::make_shared<yApi::historization::CPressure>("Pressure")),
     m_windSpeed(boost::make_shared<yApi::historization::CSpeed>("Wind speed")),
     m_windDirection(boost::make_shared<yApi::historization::CDirection>("Wind direction")),
     m_rain(boost::make_shared<yApi::historization::CRain>("Rain last 3h")),
     m_snow(boost::make_shared<yApi::historization::CRain>("Snow last 3h")),
     m_visibility(boost::make_shared<yApi::historization::CDistance>("Visibility")),
     m_allKeywords({
        m_condition,
        m_temperature,
        m_temperatureMin,
        m_temperatureMax,
        m_humidity,
        m_pressure,
        m_windSpeed,
        m_windDirection,
        m_rain,
        m_snow,
        m_visibility
     })
{
}

void CLiveWeatherDevice::declareDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                                       const std::string serviceName) const
{
   if (!api->deviceExists(m_deviceName))
   {
      api->declareDevice(m_deviceName,
                         m_deviceName,
                         serviceName,
                         m_allKeywords);
   }
}

void CLiveWeatherDevice::historize(boost::shared_ptr<yApi::IYPluginApi> api) const
{
   if (m_keywords.empty())
      return;

   api->historizeData(m_deviceName,
                      m_keywords);
}

void CLiveWeatherDevice::setCondition(const yApi::historization::EWeatherCondition& condition)
{
   m_condition->set(condition);
   m_keywords.emplace_back(m_condition);
}

void CLiveWeatherDevice::setTemperature(double temperature)
{
   m_temperature->set(temperature);
   m_keywords.emplace_back(m_temperature);
}

void CLiveWeatherDevice::setTemperatureMin(double temperature)
{
   m_temperatureMin->set(temperature);
   m_keywords.emplace_back(m_temperatureMin);
}

void CLiveWeatherDevice::setTemperatureMax(double temperature)
{
   m_temperatureMax->set(temperature);
   m_keywords.emplace_back(m_temperatureMax);
}

void CLiveWeatherDevice::setHumidity(double humidity)
{
   m_humidity->set(humidity);
   m_keywords.emplace_back(m_humidity);
}

void CLiveWeatherDevice::setPressure(double pressure)
{
   m_pressure->set(pressure);
   m_keywords.emplace_back(m_pressure);
}

void CLiveWeatherDevice::setWindSpeed(double speed)
{
   m_windSpeed->set(speed);
   m_keywords.emplace_back(m_windSpeed);
}

void CLiveWeatherDevice::setWindDirection(int direction)
{
   m_windDirection->set(direction);
   m_keywords.emplace_back(m_windDirection);
}

void CLiveWeatherDevice::setRainForLast3h(double rain)
{
   m_rain->set(rain);
   m_keywords.emplace_back(m_rain);
}

void CLiveWeatherDevice::setSnowForLast3h(double snow)
{
   m_snow->set(snow);
   m_keywords.emplace_back(m_snow);
}

void CLiveWeatherDevice::setVisibility(int distance)
{
   m_visibility->set(distance);
   m_keywords.emplace_back(m_visibility);
}
