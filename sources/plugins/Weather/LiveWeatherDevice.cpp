#include "stdafx.h"
#include "LiveWeatherDevice.h"


CLiveWeatherDevice::CLiveWeatherDevice(const std::string& deviceName)
   : m_deviceName(deviceName),
     m_temperature(boost::make_shared<yApi::historization::CTemperature>("Temperature")),
     /*TODO ajouter autres KW*/
     m_allKeywords({m_temperature})

{
}

void CLiveWeatherDevice::declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const
{
   if (!api->deviceExists(m_deviceName))
   {
      api->declareDevice(m_deviceName,
                         m_deviceName,
                         m_deviceName,
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

void CLiveWeatherDevice::setTemperature(double temperature)
{
   m_temperature->set(temperature);
   m_keywords.push_back(m_temperature);
}
