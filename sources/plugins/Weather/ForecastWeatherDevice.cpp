#include "stdafx.h"
#include "ForecastWeatherDevice.h"


CForecastWeatherDevice::CForecastWeatherDevice(std::string deviceName)
   : m_deviceName(std::move(deviceName)),
     m_forecastDatetime(boost::make_shared<yApi::historization::CDateTime>("Forecast datetime",
                                                                           yApi::EKeywordAccessMode::kGet,
                                                                           yApi::EMeasureType::kAbsolute,
                                                                           yApi::EHistoryDepth::kNoHistory)),
     m_condition(boost::make_shared<yApi::historization::CWeatherCondition>("Condition",
                                                                            yApi::EKeywordAccessMode::kGet,
                                                                            yApi::EMeasureType::kAbsolute,
                                                                            yApi::EHistoryDepth::kNoHistory)),
     m_temperature(boost::make_shared<yApi::historization::CTemperature>("Temperature",
                                                                         yApi::EKeywordAccessMode::kGet,
                                                                         yApi::EMeasureType::kAbsolute,
                                                                         yApi::typeInfo::CDoubleTypeInfo::Empty,
                                                                         yApi::EHistoryDepth::kNoHistory)),
     m_temperatureMin(boost::make_shared<yApi::historization::CTemperature>("Temperature Min",
                                                                            yApi::EKeywordAccessMode::kGet,
                                                                            yApi::EMeasureType::kAbsolute,
                                                                            yApi::typeInfo::CDoubleTypeInfo::Empty,
                                                                            yApi::EHistoryDepth::kNoHistory)),
     m_temperatureMax(boost::make_shared<yApi::historization::CTemperature>("Temperature Max",
                                                                            yApi::EKeywordAccessMode::kGet,
                                                                            yApi::EMeasureType::kAbsolute,
                                                                            yApi::typeInfo::CDoubleTypeInfo::Empty,
                                                                            yApi::EHistoryDepth::kNoHistory)),
     m_humidity(boost::make_shared<yApi::historization::CHumidity>("Humidity",
                                                                   yApi::EKeywordAccessMode::kGet,
                                                                   yApi::EMeasureType::kAbsolute,
                                                                   yApi::typeInfo::CDoubleTypeInfo::Empty,
                                                                   yApi::EHistoryDepth::kNoHistory)),
     m_pressure(boost::make_shared<yApi::historization::CPressure>("Pressure",
                                                                   yApi::EKeywordAccessMode::kGet,
                                                                   yApi::EMeasureType::kAbsolute,
                                                                   yApi::typeInfo::CDoubleTypeInfo::Empty,
                                                                   yApi::EHistoryDepth::kNoHistory)),
     m_windSpeed(boost::make_shared<yApi::historization::CSpeed>("Wind speed",
                                                                 yApi::EKeywordAccessMode::kGet,
                                                                 yApi::EMeasureType::kAbsolute,
                                                                 yApi::typeInfo::CDoubleTypeInfo::Empty,
                                                                 yApi::EHistoryDepth::kNoHistory)),
     m_windDirection(boost::make_shared<yApi::historization::CDirection>("Wind direction",
                                                                         yApi::EKeywordAccessMode::kGet,
                                                                         yApi::EMeasureType::kAbsolute,
                                                                         yApi::typeInfo::CIntTypeInfo::Empty,
                                                                         yApi::EHistoryDepth::kNoHistory)),
     m_visibility(boost::make_shared<yApi::historization::CDistance>("Visibility",
                                                                     yApi::EKeywordAccessMode::kGet,
                                                                     yApi::typeInfo::CDoubleTypeInfo::Empty,
                                                                     yApi::EHistoryDepth::kNoHistory)),
     m_allKeywords({
        m_forecastDatetime,
        m_condition,
        m_temperature,
        m_temperatureMin,
        m_temperatureMax,
        m_humidity,
        m_pressure,
        m_windSpeed,
        m_windDirection,
        m_visibility
     })
{
}

void CForecastWeatherDevice::declareDevice(boost::shared_ptr<yApi::IYPluginApi> api,
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

void CForecastWeatherDevice::historize(boost::shared_ptr<yApi::IYPluginApi> api) const
{
   if (m_keywords.empty())
      return;

   api->historizeData(m_deviceName,
                      m_keywords);
}

void CForecastWeatherDevice::setForecastDatetime(const boost::posix_time::ptime& datetime)
{
   m_forecastDatetime->set(datetime);
   m_keywords.emplace_back(m_forecastDatetime);
}

void CForecastWeatherDevice::setCondition(const yApi::historization::EWeatherCondition& condition)
{
   m_condition->set(condition);
   m_keywords.emplace_back(m_condition);
}

void CForecastWeatherDevice::setTemperature(double temperature)
{
   m_temperature->set(temperature);
   m_keywords.emplace_back(m_temperature);
}

void CForecastWeatherDevice::setTemperatureMin(double temperature)
{
   m_temperatureMin->set(temperature);
   m_keywords.emplace_back(m_temperatureMin);
}

void CForecastWeatherDevice::setTemperatureMax(double temperature)
{
   m_temperatureMax->set(temperature);
   m_keywords.emplace_back(m_temperatureMax);
}

void CForecastWeatherDevice::setHumidity(double humidity)
{
   m_humidity->set(humidity);
   m_keywords.emplace_back(m_humidity);
}

void CForecastWeatherDevice::setPressure(double pressure)
{
   m_pressure->set(pressure);
   m_keywords.emplace_back(m_pressure);
}

void CForecastWeatherDevice::setWindSpeed(double speed)
{
   m_windSpeed->set(speed);
   m_keywords.emplace_back(m_windSpeed);
}

void CForecastWeatherDevice::setWindDirection(int direction)
{
   m_windDirection->set(direction);
   m_keywords.emplace_back(m_windDirection);
}

void CForecastWeatherDevice::setVisibility(int distance)
{
   m_visibility->set(distance);
   m_keywords.emplace_back(m_visibility);
}
