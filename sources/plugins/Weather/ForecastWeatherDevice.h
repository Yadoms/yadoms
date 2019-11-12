#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;


class CForecastWeatherDevice
{
public:
   explicit CForecastWeatherDevice(std::string deviceName);
   virtual ~CForecastWeatherDevice() = default;

   void declareDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                      const std::string serviceName) const;
   void historize(boost::shared_ptr<yApi::IYPluginApi> api) const;
   void setForecastDatetime(const boost::posix_time::ptime& datetime);

   void setCondition(const yApi::historization::EWeatherCondition& condition);
   void setTemperature(double temperature);
   void setTemperatureMin(double temperature);
   void setTemperatureMax(double temperature);
   void setHumidity(double humidity);
   void setPressure(double pressure);
   void setWindSpeed(double speed);
   void setWindDirection(int direction);
   void setRainForNextPeriod(double rain);
   void setSnowForNextPeriod(double snow);
   void setVisibility(int distance);
   void setUV(double uvIndex);

private:
   const std::string m_deviceName;
   boost::shared_ptr<yApi::historization::CDateTime> m_forecastDatetime;
   boost::shared_ptr<yApi::historization::CWeatherCondition> m_condition;
   boost::shared_ptr<yApi::historization::CTemperature> m_temperature;
   boost::shared_ptr<yApi::historization::CTemperature> m_temperatureMin;
   boost::shared_ptr<yApi::historization::CTemperature> m_temperatureMax;
   boost::shared_ptr<yApi::historization::CHumidity> m_humidity;
   boost::shared_ptr<yApi::historization::CPressure> m_pressure;
   boost::shared_ptr<yApi::historization::CSpeed> m_windSpeed;
   boost::shared_ptr<yApi::historization::CDirection> m_windDirection;
   boost::shared_ptr<yApi::historization::CRain> m_rain;
   boost::shared_ptr<yApi::historization::CRain> m_snow;
   boost::shared_ptr<yApi::historization::CDistance> m_visibility;
   boost::shared_ptr<yApi::historization::CUv> m_uvIndex;
   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_allKeywords;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_keywords;
};
