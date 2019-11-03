#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;


class CLiveWeatherDevice
{
public:
   explicit CLiveWeatherDevice(std::string deviceName);
   virtual ~CLiveWeatherDevice() = default;

   void declareDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                      const std::string serviceName) const;
   void historize(boost::shared_ptr<yApi::IYPluginApi> api) const;

   void setCondition(const yApi::historization::EWeatherCondition& condition);
   void setTemperature(double temperature);
   void setTemperatureMin(double temperature);
   void setTemperatureMax(double temperature);
   void setHumidity(double humidity);
   void setPressure(double pressure);
   void setWindSpeed(double speed);
   void setWindDirection(int direction);
   void setRainForLast3h(double rain);
   void setSnowForLast3h(double snow);
   void setVisibility(int distance);

private:
   const std::string m_deviceName;
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
   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_allKeywords;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_keywords;
};
